#include "engine_config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Platform-specific case-insensitive string comparison
#ifdef _WIN32
#define strcasecmp _stricmp
#endif

// =============================================================================
// Helper Functions
// =============================================================================

// Trim whitespace from both ends of a string (in-place, returns pointer to start)
static char* trim_whitespace(char* str) {
    if (!str) return str;
    
    // Trim leading
    while (isspace((unsigned char)*str)) str++;
    
    if (*str == '\0') return str;
    
    // Trim trailing
    char* end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
    
    return str;
}

// Find a value by section and key
static ConfigValue* find_value(ConfigFile* config, const char* section, const char* key) {
    for (uint32_t i = 0; i < config->value_count; i++) {
        if (strcmp(config->values[i].section, section) == 0 &&
            strcmp(config->values[i].key, key) == 0) {
            return &config->values[i];
        }
    }
    return NULL;
}

// Find or create a value slot
static ConfigValue* find_or_create_value(ConfigFile* config, const char* section, const char* key) {
    ConfigValue* existing = find_value(config, section, key);
    if (existing) return existing;
    
    if (config->value_count >= CONFIG_MAX_KEYS) {
        fprintf(stderr, "Config: Maximum keys (%d) reached\n", CONFIG_MAX_KEYS);
        return NULL;
    }
    
    ConfigValue* new_val = &config->values[config->value_count++];
    strncpy(new_val->section, section, CONFIG_MAX_KEY_LEN - 1);
    new_val->section[CONFIG_MAX_KEY_LEN - 1] = '\0';
    strncpy(new_val->key, key, CONFIG_MAX_KEY_LEN - 1);
    new_val->key[CONFIG_MAX_KEY_LEN - 1] = '\0';
    
    return new_val;
}

// Parse a boolean string
static bool parse_bool(const char* str) {
    if (!str) return false;
    
    // True values
    if (strcasecmp(str, "true") == 0 ||
        strcasecmp(str, "yes") == 0 ||
        strcasecmp(str, "on") == 0 ||
        strcmp(str, "1") == 0) {
        return true;
    }
    
    return false;
}

// =============================================================================
// Core Config Functions
// =============================================================================

void config_init(ConfigFile* config) {
    if (!config) return;
    memset(config, 0, sizeof(ConfigFile));
}

bool config_load(ConfigFile* config, const char* filepath) {
    if (!config || !filepath) return false;
    
    FILE* file = fopen(filepath, "r");
    if (!file) {
        fprintf(stderr, "Config: Could not open '%s'\n", filepath);
        return false;
    }
    
    // Store filepath for reload
    strncpy(config->filepath, filepath, CONFIG_MAX_VALUE_LEN - 1);
    config->filepath[CONFIG_MAX_VALUE_LEN - 1] = '\0';
    
    char line[CONFIG_MAX_LINE_LEN];
    char current_section[CONFIG_MAX_KEY_LEN] = "";
    
    while (fgets(line, sizeof(line), file)) {
        char* trimmed = trim_whitespace(line);
        
        // Skip empty lines and comments
        if (trimmed[0] == '\0' || trimmed[0] == '#' || trimmed[0] == ';') {
            continue;
        }
        
        // Check for section header [SectionName]
        if (trimmed[0] == '[') {
            char* end = strchr(trimmed, ']');
            if (end) {
                size_t len = end - trimmed - 1;
                if (len < CONFIG_MAX_KEY_LEN) {
                    strncpy(current_section, trimmed + 1, len);
                    current_section[len] = '\0';
                }
            }
            continue;
        }
        
        // Parse key = value
        char* equals = strchr(trimmed, '=');
        if (!equals) continue;
        
        // Extract key
        char key[CONFIG_MAX_KEY_LEN];
        size_t key_len = equals - trimmed;
        if (key_len >= CONFIG_MAX_KEY_LEN) key_len = CONFIG_MAX_KEY_LEN - 1;
        strncpy(key, trimmed, key_len);
        key[key_len] = '\0';
        char* trimmed_key = trim_whitespace(key);
        
        // Extract value (everything after '=', before any '#' comment)
        char* value_start = trim_whitespace(equals + 1);
        char value[CONFIG_MAX_VALUE_LEN];
        strncpy(value, value_start, CONFIG_MAX_VALUE_LEN - 1);
        value[CONFIG_MAX_VALUE_LEN - 1] = '\0';
        
        // Remove inline comments
        char* comment = strchr(value, '#');
        if (comment) {
            *comment = '\0';
            trim_whitespace(value);
        }
        
        // Store value (auto-detect type)
        ConfigValue* val = find_or_create_value(config, current_section, trimmed_key);
        if (!val) continue;
        
        // Try to parse as int first
        char* endptr;
        long int_val = strtol(value, &endptr, 10);
        if (*endptr == '\0' && endptr != value && strchr(value, '.') == NULL) {
            val->type = CONFIG_TYPE_INT;
            val->data.int_val = (int)int_val;
            continue;
        }
        
        // Try to parse as float
        float float_val = strtof(value, &endptr);
        if (*endptr == '\0' && endptr != value) {
            val->type = CONFIG_TYPE_FLOAT;
            val->data.float_val = float_val;
            continue;
        }
        
        // Check for bool
        if (strcasecmp(value, "true") == 0 || strcasecmp(value, "false") == 0 ||
            strcasecmp(value, "yes") == 0 || strcasecmp(value, "no") == 0 ||
            strcasecmp(value, "on") == 0 || strcasecmp(value, "off") == 0) {
            val->type = CONFIG_TYPE_BOOL;
            val->data.bool_val = parse_bool(value);
            continue;
        }
        
        // Default to string
        val->type = CONFIG_TYPE_STRING;
        strncpy(val->data.string_val, value, CONFIG_MAX_VALUE_LEN - 1);
        val->data.string_val[CONFIG_MAX_VALUE_LEN - 1] = '\0';
    }
    
    fclose(file);
    config->loaded = true;
    
    printf("Config: Loaded %u values from '%s'\n", config->value_count, filepath);
    return true;
}

bool config_save(const ConfigFile* config, const char* filepath) {
    if (!config || !filepath) return false;
    
    FILE* file = fopen(filepath, "w");
    if (!file) {
        fprintf(stderr, "Config: Could not write to '%s'\n", filepath);
        return false;
    }
    
    fprintf(file, "# MS Tour Configuration\n");
    fprintf(file, "# Auto-generated - edit values as needed\n\n");
    
    // Group values by section
    char current_section[CONFIG_MAX_KEY_LEN] = "";
    
    for (uint32_t i = 0; i < config->value_count; i++) {
        const ConfigValue* val = &config->values[i];
        
        // Write section header if changed
        if (strcmp(current_section, val->section) != 0) {
            if (current_section[0] != '\0') {
                fprintf(file, "\n");
            }
            fprintf(file, "[%s]\n", val->section);
            strncpy(current_section, val->section, CONFIG_MAX_KEY_LEN - 1);
            current_section[CONFIG_MAX_KEY_LEN - 1] = '\0';
        }
        
        // Write key = value
        switch (val->type) {
            case CONFIG_TYPE_INT:
                fprintf(file, "%s = %d\n", val->key, val->data.int_val);
                break;
            case CONFIG_TYPE_FLOAT:
                fprintf(file, "%s = %.2f\n", val->key, val->data.float_val);
                break;
            case CONFIG_TYPE_BOOL:
                fprintf(file, "%s = %s\n", val->key, val->data.bool_val ? "true" : "false");
                break;
            case CONFIG_TYPE_STRING:
                fprintf(file, "%s = %s\n", val->key, val->data.string_val);
                break;
        }
    }
    
    fclose(file);
    printf("Config: Saved to '%s'\n", filepath);
    return true;
}

bool config_reload(ConfigFile* config) {
    if (!config || config->filepath[0] == '\0') return false;
    
    char filepath[CONFIG_MAX_VALUE_LEN];
    strncpy(filepath, config->filepath, CONFIG_MAX_VALUE_LEN - 1);
    filepath[CONFIG_MAX_VALUE_LEN - 1] = '\0';
    
    config_clear(config);
    return config_load(config, filepath);
}

void config_clear(ConfigFile* config) {
    if (!config) return;
    config->value_count = 0;
    config->loaded = false;
    // Keep filepath for potential reload
}

// =============================================================================
// Value Getters
// =============================================================================

int config_get_int(const ConfigFile* config, const char* section, const char* key, int default_val) {
    if (!config) return default_val;
    
    const ConfigValue* val = find_value((ConfigFile*)config, section, key);
    if (!val) return default_val;
    
    switch (val->type) {
        case CONFIG_TYPE_INT:
            return val->data.int_val;
        case CONFIG_TYPE_FLOAT:
            return (int)val->data.float_val;
        case CONFIG_TYPE_BOOL:
            return val->data.bool_val ? 1 : 0;
        case CONFIG_TYPE_STRING:
            return atoi(val->data.string_val);
    }
    
    return default_val;
}

float config_get_float(const ConfigFile* config, const char* section, const char* key, float default_val) {
    if (!config) return default_val;
    
    const ConfigValue* val = find_value((ConfigFile*)config, section, key);
    if (!val) return default_val;
    
    switch (val->type) {
        case CONFIG_TYPE_FLOAT:
            return val->data.float_val;
        case CONFIG_TYPE_INT:
            return (float)val->data.int_val;
        case CONFIG_TYPE_BOOL:
            return val->data.bool_val ? 1.0f : 0.0f;
        case CONFIG_TYPE_STRING:
            return (float)atof(val->data.string_val);
    }
    
    return default_val;
}

bool config_get_bool(const ConfigFile* config, const char* section, const char* key, bool default_val) {
    if (!config) return default_val;
    
    const ConfigValue* val = find_value((ConfigFile*)config, section, key);
    if (!val) return default_val;
    
    switch (val->type) {
        case CONFIG_TYPE_BOOL:
            return val->data.bool_val;
        case CONFIG_TYPE_INT:
            return val->data.int_val != 0;
        case CONFIG_TYPE_FLOAT:
            return val->data.float_val != 0.0f;
        case CONFIG_TYPE_STRING:
            return parse_bool(val->data.string_val);
    }
    
    return default_val;
}

const char* config_get_string(const ConfigFile* config, const char* section, const char* key, const char* default_val) {
    if (!config) return default_val;
    
    const ConfigValue* val = find_value((ConfigFile*)config, section, key);
    if (!val) return default_val;
    
    if (val->type == CONFIG_TYPE_STRING) {
        return val->data.string_val;
    }
    
    // For non-string types, return default
    return default_val;
}

// =============================================================================
// Value Setters
// =============================================================================

void config_set_int(ConfigFile* config, const char* section, const char* key, int value) {
    if (!config) return;
    
    ConfigValue* val = find_or_create_value(config, section, key);
    if (val) {
        val->type = CONFIG_TYPE_INT;
        val->data.int_val = value;
    }
}

void config_set_float(ConfigFile* config, const char* section, const char* key, float value) {
    if (!config) return;
    
    ConfigValue* val = find_or_create_value(config, section, key);
    if (val) {
        val->type = CONFIG_TYPE_FLOAT;
        val->data.float_val = value;
    }
}

void config_set_bool(ConfigFile* config, const char* section, const char* key, bool value) {
    if (!config) return;
    
    ConfigValue* val = find_or_create_value(config, section, key);
    if (val) {
        val->type = CONFIG_TYPE_BOOL;
        val->data.bool_val = value;
    }
}

void config_set_string(ConfigFile* config, const char* section, const char* key, const char* value) {
    if (!config || !value) return;
    
    ConfigValue* val = find_or_create_value(config, section, key);
    if (val) {
        val->type = CONFIG_TYPE_STRING;
        strncpy(val->data.string_val, value, CONFIG_MAX_VALUE_LEN - 1);
        val->data.string_val[CONFIG_MAX_VALUE_LEN - 1] = '\0';
    }
}

// =============================================================================
// Convenience Functions
// =============================================================================

WindowConfig config_get_default_window(void) {
    WindowConfig wc;
    wc.width = 1280;
    wc.height = 720;
    wc.target_fps = 60;
    strncpy(wc.title, "MS Tour - Archipelago Shipping", CONFIG_MAX_VALUE_LEN - 1);
    wc.title[CONFIG_MAX_VALUE_LEN - 1] = '\0';
    wc.fullscreen = false;
    wc.vsync = true;
    return wc;
}

AudioConfig config_get_default_audio(void) {
    AudioConfig ac;
    ac.master_volume = 1.0f;
    ac.engine_volume = 0.3f;
    ac.ambient_volume = 0.2f;
    ac.sfx_volume = 0.7f;
    return ac;
}

WindowConfig config_get_window(const ConfigFile* config) {
    WindowConfig wc = config_get_default_window();
    
    if (!config || !config->loaded) return wc;
    
    wc.width = config_get_int(config, "Window", "width", wc.width);
    wc.height = config_get_int(config, "Window", "height", wc.height);
    wc.target_fps = config_get_int(config, "Window", "target_fps", wc.target_fps);
    
    const char* title = config_get_string(config, "Window", "title", wc.title);
    if (title != wc.title) {
        strncpy(wc.title, title, CONFIG_MAX_VALUE_LEN - 1);
        wc.title[CONFIG_MAX_VALUE_LEN - 1] = '\0';
    }
    
    wc.fullscreen = config_get_bool(config, "Window", "fullscreen", wc.fullscreen);
    wc.vsync = config_get_bool(config, "Window", "vsync", wc.vsync);
    
    return wc;
}

AudioConfig config_get_audio(const ConfigFile* config) {
    AudioConfig ac = config_get_default_audio();
    
    if (!config || !config->loaded) return ac;
    
    ac.master_volume = config_get_float(config, "Audio", "master_volume", ac.master_volume);
    ac.engine_volume = config_get_float(config, "Audio", "engine_volume", ac.engine_volume);
    ac.ambient_volume = config_get_float(config, "Audio", "ambient_volume", ac.ambient_volume);
    ac.sfx_volume = config_get_float(config, "Audio", "sfx_volume", ac.sfx_volume);
    
    return ac;
}
