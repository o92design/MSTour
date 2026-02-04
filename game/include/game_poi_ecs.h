#ifndef GAME_POI_ECS_H
#define GAME_POI_ECS_H

#include "engine_ecs.h"
#include <stdbool.h>
#include <stdint.h>

// =============================================================================
// Game POI ECS
// 
// Point of Interest (POI) ECS components and systems. POIs are locations
// in the archipelago that provide passenger satisfaction bonuses when visited.
// 
// Two-tier system:
//   - General (Tier 1): Scenic views, beaches, basic landmarks
//   - Special (Tier 2): Famous historical sites, VIP-related locations
// 
// Three categories:
//   - Nature: Natural beauty, wildlife, scenic areas
//   - Historical: Historic buildings, monuments, cultural sites
//   - Military: Fortifications, naval sites, strategic points
// =============================================================================

// POI component flag (uses game-reserved bit from engine)
#define COMPONENT_POI COMPONENT_GAME_2

// =============================================================================
// POI Type Enums
// =============================================================================

typedef enum POIType {
    POI_TYPE_NATURE = 0,
    POI_TYPE_HISTORICAL = 1,
    POI_TYPE_MILITARY = 2,
    POI_TYPE_COUNT
} POIType;

typedef enum POITier {
    POI_TIER_GENERAL = 0,   // Tier 1: Common POIs
    POI_TIER_SPECIAL = 1,   // Tier 2: Rare/important POIs
    POI_TIER_COUNT
} POITier;

// =============================================================================
// POI Constants
// =============================================================================

#define POI_NAME_MAX_LENGTH 64
#define POI_DESCRIPTION_MAX_LENGTH 256
#define MAX_POIS 256

// Default satisfaction bonuses by tier
#define POI_SATISFACTION_GENERAL 5
#define POI_SATISFACTION_SPECIAL 15

// Default visit radius (world units)
#define POI_DEFAULT_RADIUS 50.0f

// =============================================================================
// POI Components (SoA Layout)
// =============================================================================

typedef struct POIComponents {
    // Identity
    char name[MAX_POIS][POI_NAME_MAX_LENGTH];
    char description[MAX_POIS][POI_DESCRIPTION_MAX_LENGTH];
    
    // Classification
    uint8_t type[MAX_POIS];         // POIType
    uint8_t tier[MAX_POIS];         // POITier
    
    // World position (separate from Transform for static POIs)
    float pos_x[MAX_POIS];
    float pos_y[MAX_POIS];
    
    // Interaction
    float radius[MAX_POIS];         // Visit detection radius
    int satisfaction_bonus[MAX_POIS];
    
    // State
    bool visited[MAX_POIS];         // Has player visited this POI?
    bool discovered[MAX_POIS];      // Is POI visible through fog of war?
    uint32_t visit_count[MAX_POIS]; // Total visits (for statistics)
} POIComponents;

// =============================================================================
// POI ECS World
// =============================================================================

typedef struct POIEcsWorld {
    POIComponents pois;
    uint32_t poi_count;             // Active POIs
    bool initialized;
} POIEcsWorld;

// =============================================================================
// POI ECS Lifecycle
// =============================================================================

// Initialize POI ECS world
void poi_ecs_init(POIEcsWorld* poi_world);

// Shutdown POI ECS world
void poi_ecs_shutdown(POIEcsWorld* poi_world);

// Clear all POIs (for reloading)
void poi_ecs_clear(POIEcsWorld* poi_world);

// =============================================================================
// POI Creation
// =============================================================================

// POI creation parameters
typedef struct POICreateParams {
    const char* name;
    const char* description;
    POIType type;
    POITier tier;
    float x;
    float y;
    float radius;                   // 0 = use default
    int satisfaction_bonus;         // 0 = use tier default
} POICreateParams;

// Create a new POI
// Returns POI index (0 to MAX_POIS-1), or -1 on failure
int poi_ecs_create(POIEcsWorld* poi_world, const POICreateParams* params);

// Destroy a POI by index
void poi_ecs_destroy(POIEcsWorld* poi_world, int poi_index);

// =============================================================================
// POI Accessors
// =============================================================================

// Get POI count
uint32_t poi_ecs_get_count(const POIEcsWorld* poi_world);

// Check if POI index is valid
bool poi_ecs_is_valid(const POIEcsWorld* poi_world, int poi_index);

// Get POI name
const char* poi_ecs_get_name(const POIEcsWorld* poi_world, int poi_index);

// Get POI type
POIType poi_ecs_get_type(const POIEcsWorld* poi_world, int poi_index);

// Get POI tier
POITier poi_ecs_get_tier(const POIEcsWorld* poi_world, int poi_index);

// Get POI position
void poi_ecs_get_position(const POIEcsWorld* poi_world, int poi_index, float* x, float* y);

// Get POI radius
float poi_ecs_get_radius(const POIEcsWorld* poi_world, int poi_index);

// Get satisfaction bonus
int poi_ecs_get_satisfaction_bonus(const POIEcsWorld* poi_world, int poi_index);

// Check if POI has been visited
bool poi_ecs_is_visited(const POIEcsWorld* poi_world, int poi_index);

// Check if POI is discovered (visible through fog)
bool poi_ecs_is_discovered(const POIEcsWorld* poi_world, int poi_index);

// =============================================================================
// POI State Modification
// =============================================================================

// Mark POI as visited
void poi_ecs_set_visited(POIEcsWorld* poi_world, int poi_index, bool visited);

// Reset all visited states (for restarting tours)
void poi_ecs_reset_all_visited(POIEcsWorld* poi_world);

// Mark POI as discovered
void poi_ecs_set_discovered(POIEcsWorld* poi_world, int poi_index, bool discovered);

// Increment visit count
void poi_ecs_increment_visits(POIEcsWorld* poi_world, int poi_index);

// =============================================================================
// POI Queries
// =============================================================================

// Find POI at world position (within radius)
// Returns POI index or -1 if none found
int poi_ecs_find_at_position(const POIEcsWorld* poi_world, float x, float y);

// Find all POIs within range of a position
// Fills out_indices array (up to max_results), returns count found
int poi_ecs_find_in_range(const POIEcsWorld* poi_world, float x, float y, float range,
                          int* out_indices, int max_results);

// Find POI by name (case-insensitive)
int poi_ecs_find_by_name(const POIEcsWorld* poi_world, const char* name);

// Check if a position is within a POI's visit radius
bool poi_ecs_check_visit(const POIEcsWorld* poi_world, int poi_index, float x, float y);

// =============================================================================
// POI System Update
// =============================================================================

// Callback for POI visit events
typedef void (*POIVisitCallback)(int poi_index, Entity visitor, void* user_data);

// Context for POI system update
typedef struct POISystemContext {
    POIVisitCallback on_visit;      // Called when a ship visits a POI
    void* user_data;                // User data for callback
} POISystemContext;

// Update POI system - checks for ship visits to POIs
// Requires ECSWorld for ship positions
void poi_ecs_system_update(POIEcsWorld* poi_world, const ECSWorld* ecs_world,
                           ComponentMask ship_mask, const POISystemContext* context);

// =============================================================================
// POI Statistics
// =============================================================================

typedef struct POIStats {
    uint32_t total_pois;
    uint32_t visited_pois;
    uint32_t discovered_pois;
    uint32_t nature_count;
    uint32_t historical_count;
    uint32_t military_count;
    uint32_t general_count;
    uint32_t special_count;
} POIStats;

// Get POI statistics
POIStats poi_ecs_get_stats(const POIEcsWorld* poi_world);

// =============================================================================
// Utility Functions
// =============================================================================

// Get type name as string
const char* poi_type_to_string(POIType type);

// Get tier name as string
const char* poi_tier_to_string(POITier tier);

// Parse type from string (returns POI_TYPE_NATURE on failure)
POIType poi_type_from_string(const char* str);

// Parse tier from string (returns POI_TIER_GENERAL on failure)
POITier poi_tier_from_string(const char* str);

#endif // GAME_POI_ECS_H
