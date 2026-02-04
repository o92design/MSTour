#include "game_poi_loader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include cJSON
#include "cJSON.h"

// =============================================================================
// Constants
// =============================================================================

#define DEFAULT_POI_FILEPATH "assets/data/pois.json"
#define MAX_FILE_SIZE (1024 * 1024)  // 1 MB max file size

// =============================================================================
// Helper Functions
// =============================================================================

static char* read_file_contents(const char* filepath, long* out_size) {
    FILE* file = fopen(filepath, "rb");
    if (!file) return NULL;
    
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    if (size <= 0 || size > MAX_FILE_SIZE) {
        fclose(file);
        return NULL;
    }
    
    char* buffer = (char*)malloc(size + 1);
    if (!buffer) {
        fclose(file);
        return NULL;
    }
    
    size_t read = fread(buffer, 1, size, file);
    fclose(file);
    
    if (read != (size_t)size) {
        free(buffer);
        return NULL;
    }
    
    buffer[size] = '\0';
    if (out_size) *out_size = size;
    return buffer;
}

static bool parse_poi_from_json(cJSON* poi_json, POICreateParams* params) {
    if (!poi_json || !params) return false;
    
    memset(params, 0, sizeof(POICreateParams));
    
    // Required: name
    cJSON* name = cJSON_GetObjectItem(poi_json, "name");
    if (!cJSON_IsString(name)) return false;
    params->name = name->valuestring;
    
    // Optional: description
    cJSON* desc = cJSON_GetObjectItem(poi_json, "description");
    if (cJSON_IsString(desc)) {
        params->description = desc->valuestring;
    }
    
    // Required: type
    cJSON* type = cJSON_GetObjectItem(poi_json, "type");
    if (!cJSON_IsString(type)) return false;
    params->type = poi_type_from_string(type->valuestring);
    
    // Required: tier
    cJSON* tier = cJSON_GetObjectItem(poi_json, "tier");
    if (!cJSON_IsString(tier)) return false;
    params->tier = poi_tier_from_string(tier->valuestring);
    
    // Required: position
    cJSON* position = cJSON_GetObjectItem(poi_json, "position");
    if (!cJSON_IsObject(position)) return false;
    
    cJSON* pos_x = cJSON_GetObjectItem(position, "x");
    cJSON* pos_y = cJSON_GetObjectItem(position, "y");
    if (!cJSON_IsNumber(pos_x) || !cJSON_IsNumber(pos_y)) return false;
    
    params->x = (float)pos_x->valuedouble;
    params->y = (float)pos_y->valuedouble;
    
    // Optional: radius
    cJSON* radius = cJSON_GetObjectItem(poi_json, "radius");
    if (cJSON_IsNumber(radius)) {
        params->radius = (float)radius->valuedouble;
    }
    
    // Optional: satisfaction_bonus
    cJSON* bonus = cJSON_GetObjectItem(poi_json, "satisfaction_bonus");
    if (cJSON_IsNumber(bonus)) {
        params->satisfaction_bonus = bonus->valueint;
    }
    
    return true;
}

// =============================================================================
// Error Messages
// =============================================================================

const char* poi_load_result_to_string(POILoadResult result) {
    switch (result) {
        case POI_LOAD_SUCCESS:              return "Success";
        case POI_LOAD_FILE_NOT_FOUND:       return "File not found";
        case POI_LOAD_PARSE_ERROR:          return "JSON parse error";
        case POI_LOAD_INVALID_SCHEMA:       return "Invalid schema";
        case POI_LOAD_WORLD_NOT_INITIALIZED: return "POI world not initialized";
        case POI_LOAD_OUT_OF_MEMORY:        return "Out of memory";
        default:                            return "Unknown error";
    }
}

// =============================================================================
// Loading Functions
// =============================================================================

POILoadResult poi_load_from_string(POIEcsWorld* poi_world, const char* json_string) {
    if (!poi_world || !poi_world->initialized) {
        return POI_LOAD_WORLD_NOT_INITIALIZED;
    }
    
    if (!json_string) {
        return POI_LOAD_PARSE_ERROR;
    }
    
    // Parse JSON
    cJSON* root = cJSON_Parse(json_string);
    if (!root) {
        return POI_LOAD_PARSE_ERROR;
    }
    
    // Validate version (optional but recommended)
    cJSON* version = cJSON_GetObjectItem(root, "version");
    if (version && cJSON_IsString(version)) {
        // Currently we accept any version
    }
    
    // Get POIs array
    cJSON* pois = cJSON_GetObjectItem(root, "pois");
    if (!cJSON_IsArray(pois)) {
        cJSON_Delete(root);
        return POI_LOAD_INVALID_SCHEMA;
    }
    
    // Clear existing POIs
    poi_ecs_clear(poi_world);
    
    // Load each POI
    int poi_count = cJSON_GetArraySize(pois);
    int loaded = 0;
    
    for (int i = 0; i < poi_count; i++) {
        cJSON* poi_json = cJSON_GetArrayItem(pois, i);
        POICreateParams params;
        
        if (parse_poi_from_json(poi_json, &params)) {
            int idx = poi_ecs_create(poi_world, &params);
            if (idx >= 0) {
                loaded++;
            }
        }
    }
    
    cJSON_Delete(root);
    
    // Log result
    printf("[POI Loader] Loaded %d POIs from JSON\n", loaded);
    
    return POI_LOAD_SUCCESS;
}

POILoadResult poi_load_from_file(POIEcsWorld* poi_world, const char* filepath) {
    if (!poi_world || !poi_world->initialized) {
        return POI_LOAD_WORLD_NOT_INITIALIZED;
    }
    
    if (!filepath) {
        filepath = DEFAULT_POI_FILEPATH;
    }
    
    // Read file
    long file_size = 0;
    char* contents = read_file_contents(filepath, &file_size);
    if (!contents) {
        printf("[POI Loader] Failed to read file: %s\n", filepath);
        return POI_LOAD_FILE_NOT_FOUND;
    }
    
    printf("[POI Loader] Loading POIs from: %s (%ld bytes)\n", filepath, file_size);
    
    // Parse and load
    POILoadResult result = poi_load_from_string(poi_world, contents);
    
    free(contents);
    return result;
}

// =============================================================================
// Validation
// =============================================================================

POILoadResult poi_validate_file(const char* filepath) {
    if (!filepath) return POI_LOAD_FILE_NOT_FOUND;
    
    long file_size = 0;
    char* contents = read_file_contents(filepath, &file_size);
    if (!contents) return POI_LOAD_FILE_NOT_FOUND;
    
    cJSON* root = cJSON_Parse(contents);
    free(contents);
    
    if (!root) return POI_LOAD_PARSE_ERROR;
    
    // Check for pois array
    cJSON* pois = cJSON_GetObjectItem(root, "pois");
    if (!cJSON_IsArray(pois)) {
        cJSON_Delete(root);
        return POI_LOAD_INVALID_SCHEMA;
    }
    
    cJSON_Delete(root);
    return POI_LOAD_SUCCESS;
}

// =============================================================================
// Default Data
// =============================================================================

void poi_load_defaults(POIEcsWorld* poi_world) {
    if (!poi_world || !poi_world->initialized) return;
    
    printf("[POI Loader] Loading default POIs\n");
    
    // Create some default POIs as fallback
    POICreateParams params[] = {
        {
            .name = "Vinga Lighthouse",
            .description = "Historic lighthouse marking the entrance to Gothenburg harbor",
            .type = POI_TYPE_HISTORICAL,
            .tier = POI_TIER_SPECIAL,
            .x = 800.0f, .y = 200.0f,
            .radius = 60.0f,
            .satisfaction_bonus = 20
        },
        {
            .name = "Älvsborg Fortress",
            .description = "Famous fortress guarding the entrance to Gothenburg",
            .type = POI_TYPE_MILITARY,
            .tier = POI_TIER_SPECIAL,
            .x = 150.0f, .y = 150.0f,
            .radius = 65.0f,
            .satisfaction_bonus = 22
        },
        {
            .name = "Vrångö Nature Reserve",
            .description = "Pristine nature reserve with rare bird species",
            .type = POI_TYPE_NATURE,
            .tier = POI_TIER_SPECIAL,
            .x = 600.0f, .y = 500.0f,
            .radius = 70.0f,
            .satisfaction_bonus = 18
        },
        {
            .name = "Styrsö Village",
            .description = "Traditional fishing village with charming wooden houses",
            .type = POI_TYPE_HISTORICAL,
            .tier = POI_TIER_GENERAL,
            .x = 400.0f, .y = 350.0f,
            .radius = 50.0f,
            .satisfaction_bonus = 8
        },
        {
            .name = "Brännö Beach",
            .description = "Popular sandy beach with calm waters",
            .type = POI_TYPE_NATURE,
            .tier = POI_TIER_GENERAL,
            .x = 480.0f, .y = 300.0f,
            .radius = 40.0f,
            .satisfaction_bonus = 5
        }
    };
    
    int count = sizeof(params) / sizeof(params[0]);
    for (int i = 0; i < count; i++) {
        poi_ecs_create(poi_world, &params[i]);
    }
    
    printf("[POI Loader] Loaded %d default POIs\n", count);
}

const char* poi_get_default_filepath(void) {
    return DEFAULT_POI_FILEPATH;
}
