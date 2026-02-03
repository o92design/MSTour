#ifndef ENGINE_CONFIG_H
#define ENGINE_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

// =============================================================================
// Generic INI Configuration Parser
// 
// Provides a reusable INI file parser for the engine.
// Supports sections [SectionName] and key = value pairs.
// Types: int, float, bool, string
// =============================================================================

// Maximum sizes
#define CONFIG_MAX_SECTIONS     16
#define CONFIG_MAX_KEYS         64
#define CONFIG_MAX_KEY_LEN      64
#define CONFIG_MAX_VALUE_LEN    256
#define CONFIG_MAX_LINE_LEN     512

// =============================================================================
// Config Value Types
// =============================================================================

typedef enum ConfigValueType {
    CONFIG_TYPE_INT,
    CONFIG_TYPE_FLOAT,
    CONFIG_TYPE_BOOL,
    CONFIG_TYPE_STRING
} ConfigValueType;

typedef struct ConfigValue {
    char key[CONFIG_MAX_KEY_LEN];
    char section[CONFIG_MAX_KEY_LEN];
    ConfigValueType type;
    union {
        int int_val;
        float float_val;
        bool bool_val;
        char string_val[CONFIG_MAX_VALUE_LEN];
    } data;
} ConfigValue;

// =============================================================================
// Config File Handle
// =============================================================================

typedef struct ConfigFile {
    ConfigValue values[CONFIG_MAX_KEYS];
    uint32_t value_count;
    char filepath[CONFIG_MAX_VALUE_LEN];
    bool loaded;
} ConfigFile;

// =============================================================================
// Window Configuration (common config structure)
// =============================================================================

typedef struct WindowConfig {
    int width;
    int height;
    int target_fps;
    char title[CONFIG_MAX_VALUE_LEN];
    bool fullscreen;
    bool vsync;
} WindowConfig;

// =============================================================================
// Audio Configuration
// =============================================================================

typedef struct AudioConfig {
    float master_volume;
    float engine_volume;
    float ambient_volume;
    float sfx_volume;
} AudioConfig;

// =============================================================================
// Core Config Functions
// =============================================================================

// Initialize an empty config file handle
void config_init(ConfigFile* config);

// Load config from file (returns true on success)
bool config_load(ConfigFile* config, const char* filepath);

// Save config to file (returns true on success)
bool config_save(const ConfigFile* config, const char* filepath);

// Reload config from same file
bool config_reload(ConfigFile* config);

// Clear all values
void config_clear(ConfigFile* config);

// =============================================================================
// Value Getters (with defaults)
// =============================================================================

// Get int value, returns default_val if not found
int config_get_int(const ConfigFile* config, const char* section, const char* key, int default_val);

// Get float value, returns default_val if not found
float config_get_float(const ConfigFile* config, const char* section, const char* key, float default_val);

// Get bool value, returns default_val if not found
bool config_get_bool(const ConfigFile* config, const char* section, const char* key, bool default_val);

// Get string value, returns default_val if not found (copies to buffer)
const char* config_get_string(const ConfigFile* config, const char* section, const char* key, const char* default_val);

// =============================================================================
// Value Setters
// =============================================================================

// Set int value (creates if doesn't exist)
void config_set_int(ConfigFile* config, const char* section, const char* key, int value);

// Set float value
void config_set_float(ConfigFile* config, const char* section, const char* key, float value);

// Set bool value
void config_set_bool(ConfigFile* config, const char* section, const char* key, bool value);

// Set string value
void config_set_string(ConfigFile* config, const char* section, const char* key, const char* value);

// =============================================================================
// Convenience Functions for Common Configs
// =============================================================================

// Get window config with defaults
WindowConfig config_get_window(const ConfigFile* config);

// Get audio config with defaults
AudioConfig config_get_audio(const ConfigFile* config);

// Get default window config
WindowConfig config_get_default_window(void);

// Get default audio config
AudioConfig config_get_default_audio(void);

#ifdef __cplusplus
}
#endif

#endif // ENGINE_CONFIG_H
