#ifndef GAME_POI_LOADER_H
#define GAME_POI_LOADER_H

#include "game_poi_ecs.h"
#include <stdbool.h>

// =============================================================================
// POI Data Loader
// 
// Loads POI data from JSON files for moddability.
// JSON schema:
// {
//   "version": "1.0",
//   "pois": [
//     {
//       "id": "poi_vinga",
//       "name": "Vinga Lighthouse",
//       "type": "historical",
//       "tier": "special",
//       "position": { "x": 1200.0, "y": 800.0 },
//       "radius": 50.0,
//       "satisfaction_bonus": 15,
//       "description": "Historic lighthouse on Vinga island"
//     }
//   ]
// }
// =============================================================================

// =============================================================================
// Load Result
// =============================================================================

typedef enum POILoadResult {
    POI_LOAD_SUCCESS = 0,
    POI_LOAD_FILE_NOT_FOUND,
    POI_LOAD_PARSE_ERROR,
    POI_LOAD_INVALID_SCHEMA,
    POI_LOAD_WORLD_NOT_INITIALIZED,
    POI_LOAD_OUT_OF_MEMORY
} POILoadResult;

// Get human-readable error message for load result
const char* poi_load_result_to_string(POILoadResult result);

// =============================================================================
// Loading Functions
// =============================================================================

// Load POIs from JSON file into POI world
// Returns POI_LOAD_SUCCESS on success, error code otherwise
POILoadResult poi_load_from_file(POIEcsWorld* poi_world, const char* filepath);

// Load POIs from JSON string (for testing or embedded data)
POILoadResult poi_load_from_string(POIEcsWorld* poi_world, const char* json_string);

// =============================================================================
// Validation
// =============================================================================

// Validate a JSON file without loading (checks schema)
POILoadResult poi_validate_file(const char* filepath);

// =============================================================================
// Default Data
// =============================================================================

// Load built-in default POIs (fallback if no file found)
void poi_load_defaults(POIEcsWorld* poi_world);

// Get the default POI data file path
const char* poi_get_default_filepath(void);

#endif // GAME_POI_LOADER_H
