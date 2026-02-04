#ifndef GAME_FOG_OF_WAR_H
#define GAME_FOG_OF_WAR_H

#include "engine_ecs.h"
#include "engine_spatial_hash.h"
#include "game_poi_ecs.h"
#include <stdbool.h>

// =============================================================================
// Fog of War System
// 
// Manages visibility in the archipelago. POIs and areas are revealed when
// ships get close enough. For the prototype, all POIs are visible but
// surrounded by fog that clears as ships approach.
// 
// Visibility states:
//   - Hidden: Never seen, completely fogged
//   - Discovered: Seen before, partially visible (can navigate toward)
//   - Visible: Currently in sight, fully clear
// =============================================================================

// =============================================================================
// Constants
// =============================================================================

#define FOG_REVEAL_RADIUS 200.0f      // Distance at which fog clears around ships
#define FOG_DISCOVERY_RADIUS 400.0f   // Distance at which areas become discovered
#define FOG_FADE_SPEED 2.0f           // How fast fog fades in/out

// Chunk-based fog grid (allocates memory only for explored regions)
#define FOG_CELL_SIZE 50.0f           // Size of each fog cell in world units
#define FOG_CHUNK_SIZE 32             // Cells per chunk dimension (32x32 = 1024 cells)
#define FOG_CHUNK_WORLD_SIZE (FOG_CELL_SIZE * FOG_CHUNK_SIZE)  // 1600 world units per chunk
#define FOG_MAX_CHUNKS 256            // Maximum allocated chunks (covers huge area)

// Rendering constants
#define FOG_COLOR_R 200               // Fog overlay color (gray-white sea mist)
#define FOG_COLOR_G 210
#define FOG_COLOR_B 220
#define FOG_COLOR_A 200
#define FOG_POI_MIN_ALPHA 0.15f       // Minimum POI visibility through fog

// Optimization constants
#define FOG_MAX_TRACKED_SHIPS 16      // Maximum ships to track for reveal optimization
#define FOG_MOVE_THRESHOLD_SQ 625.0f  // (cell_size/2)^2 - skip reveal if moved less

// =============================================================================
// Visibility State
// =============================================================================

typedef enum VisibilityState {
    VISIBILITY_HIDDEN = 0,      // Never seen
    VISIBILITY_DISCOVERED = 1,  // Seen before, can navigate to
    VISIBILITY_VISIBLE = 2      // Currently visible
} VisibilityState;

// =============================================================================
// Fog Chunk (dynamically allocated region)
// =============================================================================

typedef struct FogChunk {
    int chunk_x;                                    // Chunk coordinate (not world coord)
    int chunk_y;
    unsigned char cells[FOG_CHUNK_SIZE][FOG_CHUNK_SIZE];  // 1 = revealed, 0 = fogged
    bool allocated;
} FogChunk;

// =============================================================================
// Fog of War State
// =============================================================================

typedef struct FogOfWarState {
    // Per-POI visibility tracking
    VisibilityState poi_visibility[MAX_POIS];
    float poi_fog_alpha[MAX_POIS];  // 0.0 = clear, 1.0 = fully fogged
    
    // Chunk-based revealed area tracking (dynamically allocated)
    FogChunk chunks[FOG_MAX_CHUNKS];
    int chunk_count;
    
    // Spatial hash for O(1) chunk lookup
    SpatialHashMap chunk_map;
    
    // Ship position tracking for reveal optimization
    float last_reveal_x[FOG_MAX_TRACKED_SHIPS];
    float last_reveal_y[FOG_MAX_TRACKED_SHIPS];
    int tracked_ship_count;
    
    // Global settings
    float reveal_radius;
    float discovery_radius;
    bool enabled;
    
    // Prototype mode: all POIs visible on map (but still track fog for future use)
    bool prototype_mode;
    
    bool initialized;
} FogOfWarState;

// =============================================================================
// Lifecycle
// =============================================================================

// Initialize fog of war system
void fog_init(FogOfWarState* fog);

// Shutdown fog of war system
void fog_shutdown(FogOfWarState* fog);

// Reset fog (re-hide everything)
void fog_reset(FogOfWarState* fog);

// =============================================================================
// Configuration
// =============================================================================

// Enable/disable fog of war
void fog_set_enabled(FogOfWarState* fog, bool enabled);

// Set reveal radius (distance for full visibility)
void fog_set_reveal_radius(FogOfWarState* fog, float radius);

// Set discovery radius (distance for discovered state)
void fog_set_discovery_radius(FogOfWarState* fog, float radius);

// Enable prototype mode (all POIs visible)
void fog_set_prototype_mode(FogOfWarState* fog, bool enabled);

// =============================================================================
// Visibility Queries
// =============================================================================

// Get visibility state for a POI
VisibilityState fog_get_poi_visibility(const FogOfWarState* fog, int poi_index);

// Get fog alpha for rendering (0.0 = clear, 1.0 = fogged)
float fog_get_poi_alpha(const FogOfWarState* fog, int poi_index);

// Check if a world position is visible (near ship)
bool fog_is_position_visible(const FogOfWarState* fog, const ECSWorld* ecs_world,
                             ComponentMask ship_mask, float x, float y);

// Check if a world position has been revealed (persistent)
bool fog_is_position_revealed(const FogOfWarState* fog, float x, float y);

// =============================================================================
// Visibility Modification
// =============================================================================

// Reveal area around a position (marks grid cells as revealed)
void fog_reveal_area(FogOfWarState* fog, float x, float y, float radius);

// Manually reveal a POI (for story events, etc.)
void fog_reveal_poi(FogOfWarState* fog, int poi_index);

// Manually discover a POI (makes it navigable)
void fog_discover_poi(FogOfWarState* fog, int poi_index);

// Reveal all POIs (cheat/debug)
void fog_reveal_all(FogOfWarState* fog, const POIEcsWorld* poi_world);

// =============================================================================
// System Update
// =============================================================================

// Update fog of war based on ship positions
// This reveals areas around ships and updates visibility states
void fog_system_update(FogOfWarState* fog, const POIEcsWorld* poi_world,
                       const ECSWorld* ecs_world, ComponentMask ship_mask,
                       float delta_time);

// =============================================================================
// Rendering Helpers
// =============================================================================

// Get fog color for rendering
// Returns RGBA color with appropriate alpha based on state
void fog_get_render_color(const FogOfWarState* fog, int poi_index,
                          unsigned char* r, unsigned char* g,
                          unsigned char* b, unsigned char* a);

#endif // GAME_FOG_OF_WAR_H
