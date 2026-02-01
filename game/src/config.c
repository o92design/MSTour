#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper: Trim whitespace from both ends of a string
static char* trim(char* str) {
    char* end;
    
    // Trim leading space
    while (isspace((unsigned char)*str)) str++;
    
    if (*str == 0) return str;
    
    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    
    end[1] = '\0';
    return str;
}

// Helper: Parse a float value from a line
static bool parse_float(const char* line, const char* key, float* value) {
    char* equals = strchr(line, '=');
    if (!equals) return false;
    
    // Extract key name
    char key_buf[64];
    size_t key_len = equals - line;
    if (key_len >= sizeof(key_buf)) return false;
    
    strncpy(key_buf, line, key_len);
    key_buf[key_len] = '\0';
    char* trimmed_key = trim(key_buf);
    
    // Check if this is the key we're looking for
    if (strcmp(trimmed_key, key) != 0) return false;
    
    // Parse value
    char* value_str = trim(equals + 1);
    char* endptr;
    float parsed = strtof(value_str, &endptr);
    
    if (endptr == value_str) return false; // No conversion
    
    *value = parsed;
    return true;
}

bool config_load_ship_physics(const char* filepath, ShipPhysicsConfig* config) {
    FILE* file = fopen(filepath, "r");
    if (!file) {
        printf("Config: Could not open '%s', using defaults\n", filepath);
        *config = ship_physics_get_default_config();
        return false;
    }
    
    printf("Config: Loading from '%s'\n", filepath);
    
    // Start with defaults
    *config = ship_physics_get_default_config();
    
    char line[256];
    bool in_ship_physics_section = false;
    int values_loaded = 0;
    
    while (fgets(line, sizeof(line), file)) {
        char* trimmed = trim(line);
        
        // Skip empty lines and comments
        if (trimmed[0] == '\0' || trimmed[0] == '#' || trimmed[0] == ';') {
            continue;
        }
        
        // Check for section header
        if (trimmed[0] == '[') {
            in_ship_physics_section = (strstr(trimmed, "[ShipPhysics]") != NULL);
            continue;
        }
        
        // Parse values only in [ShipPhysics] section
        if (!in_ship_physics_section) continue;
        
        // Parse each field
        if (parse_float(trimmed, "max_speed", &config->max_speed)) values_loaded++;
        else if (parse_float(trimmed, "acceleration", &config->acceleration)) values_loaded++;
        else if (parse_float(trimmed, "deceleration", &config->deceleration)) values_loaded++;
        else if (parse_float(trimmed, "reverse_speed_multiplier", &config->reverse_speed_multiplier)) values_loaded++;
        else if (parse_float(trimmed, "reverse_accel_multiplier", &config->reverse_accel_multiplier)) values_loaded++;
        else if (parse_float(trimmed, "max_turn_rate", &config->max_turn_rate)) values_loaded++;
        else if (parse_float(trimmed, "turn_accel", &config->turn_accel)) values_loaded++;
        else if (parse_float(trimmed, "speed_turn_factor", &config->speed_turn_factor)) values_loaded++;
        else if (parse_float(trimmed, "coast_friction", &config->coast_friction)) values_loaded++;
        else if (parse_float(trimmed, "drift_factor", &config->drift_factor)) values_loaded++;
        else if (parse_float(trimmed, "momentum_retention", &config->momentum_retention)) values_loaded++;
        else if (parse_float(trimmed, "throttle_response_time", &config->throttle_response_time)) values_loaded++;
        else if (parse_float(trimmed, "steering_response_time", &config->steering_response_time)) values_loaded++;
    }
    
    fclose(file);
    
    printf("Config: Loaded %d values from '%s'\n", values_loaded, filepath);
    return true;
}

bool config_save_ship_physics(const char* filepath, const ShipPhysicsConfig* config) {
    FILE* file = fopen(filepath, "w");
    if (!file) {
        printf("Config: Could not write to '%s'\n", filepath);
        return false;
    }
    
    fprintf(file, "# MS Tour - Ship Physics Configuration\n");
    fprintf(file, "# Edit values and press F6 in-game to hot-reload\n\n");
    
    fprintf(file, "[ShipPhysics]\n\n");
    
    fprintf(file, "# Linear Movement\n");
    fprintf(file, "max_speed = %.2f              # units/second at full throttle\n", config->max_speed);
    fprintf(file, "acceleration = %.2f           # units/second² (reaches max in ~3 seconds)\n", config->acceleration);
    fprintf(file, "deceleration = %.2f           # units/second² (natural water friction)\n", config->deceleration);
    fprintf(file, "reverse_speed_multiplier = %.2f  # reverse is 35%% of forward speed\n", config->reverse_speed_multiplier);
    fprintf(file, "reverse_accel_multiplier = %.2f  # reverse acceleration is 50%% of forward\n\n", config->reverse_accel_multiplier);
    
    fprintf(file, "# Rotational Movement\n");
    fprintf(file, "max_turn_rate = %.2f          # degrees/second at full speed\n", config->max_turn_rate);
    fprintf(file, "turn_accel = %.2f             # degrees/second² (turn builds up smoothly)\n", config->turn_accel);
    fprintf(file, "speed_turn_factor = %.2f      # turning effectiveness at low speed (65%% at stop)\n\n", config->speed_turn_factor);
    
    fprintf(file, "# Momentum & Drift\n");
    fprintf(file, "coast_friction = %.3f        # friction coefficient when throttle released\n", config->coast_friction);
    fprintf(file, "drift_factor = %.2f           # how much ship drifts outward in turns (0=none, 1=full)\n", config->drift_factor);
    fprintf(file, "momentum_retention = %.2f     # per-frame momentum retention (creates coasting)\n\n", config->momentum_retention);
    
    fprintf(file, "# Responsiveness\n");
    fprintf(file, "throttle_response_time = %.2f # seconds to respond to throttle input\n", config->throttle_response_time);
    fprintf(file, "steering_response_time = %.2f # seconds to respond to steering input\n", config->steering_response_time);
    
    fclose(file);
    
    printf("Config: Saved to '%s'\n", filepath);
    return true;
}

bool config_reload_ship_physics(const char* filepath, ShipPhysicsConfig* config) {
    printf("Config: Hot-reloading from '%s'...\n", filepath);
    return config_load_ship_physics(filepath, config);
}
