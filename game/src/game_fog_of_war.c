#include "game_fog_of_war.h"
#include <string.h>
#include <math.h>

// =============================================================================
// Helper Functions
// =============================================================================

static float distance_squared(float x1, float y1, float x2, float y2) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    return dx * dx + dy * dy;
}

static float clamp_f(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

static float lerp_f(float a, float b, float t) {
    return a + (b - a) * t;
}

// =============================================================================
// Lifecycle
// =============================================================================

void fog_init(FogOfWarState* fog) {
    if (!fog) return;
    
    memset(fog, 0, sizeof(FogOfWarState));
    
    fog->reveal_radius = FOG_REVEAL_RADIUS;
    fog->discovery_radius = FOG_DISCOVERY_RADIUS;
    fog->enabled = true;
    fog->prototype_mode = true;  // Default to prototype mode (POIs shown on map but fogged)
    fog->initialized = true;
    fog->chunk_count = 0;
    
    // Start all POIs as hidden with full fog
    // (prototype_mode makes them visible on map but still fogged until approached)
    for (int i = 0; i < MAX_POIS; i++) {
        fog->poi_visibility[i] = VISIBILITY_HIDDEN;
        fog->poi_fog_alpha[i] = 1.0f;
    }
}

void fog_shutdown(FogOfWarState* fog) {
    if (!fog) return;
    fog->initialized = false;
}

void fog_reset(FogOfWarState* fog) {
    if (!fog || !fog->initialized) return;
    
    for (int i = 0; i < MAX_POIS; i++) {
        fog->poi_visibility[i] = VISIBILITY_HIDDEN;
        fog->poi_fog_alpha[i] = 1.0f;
    }
    
    // Reset all chunks
    for (int i = 0; i < fog->chunk_count; i++) {
        fog->chunks[i].allocated = false;
    }
    fog->chunk_count = 0;
    fog->tracked_ship_count = 0;
}

// =============================================================================
// Chunk Helpers
// =============================================================================

// Convert world position to chunk coordinates
static void world_to_chunk(float world_x, float world_y, int* chunk_x, int* chunk_y) {
    // Use floor division to handle negative coordinates correctly
    *chunk_x = (int)floorf(world_x / FOG_CHUNK_WORLD_SIZE);
    *chunk_y = (int)floorf(world_y / FOG_CHUNK_WORLD_SIZE);
}

// Convert world position to cell within a chunk
static void world_to_cell_in_chunk(float world_x, float world_y, int chunk_x, int chunk_y,
                                    int* cell_x, int* cell_y) {
    float chunk_origin_x = chunk_x * FOG_CHUNK_WORLD_SIZE;
    float chunk_origin_y = chunk_y * FOG_CHUNK_WORLD_SIZE;
    *cell_x = (int)((world_x - chunk_origin_x) / FOG_CELL_SIZE);
    *cell_y = (int)((world_y - chunk_origin_y) / FOG_CELL_SIZE);
    
    // Clamp to valid range
    if (*cell_x < 0) *cell_x = 0;
    if (*cell_x >= FOG_CHUNK_SIZE) *cell_x = FOG_CHUNK_SIZE - 1;
    if (*cell_y < 0) *cell_y = 0;
    if (*cell_y >= FOG_CHUNK_SIZE) *cell_y = FOG_CHUNK_SIZE - 1;
}

// Find chunk by coordinates, returns NULL if not allocated
static FogChunk* fog_find_chunk(FogOfWarState* fog, int chunk_x, int chunk_y) {
    for (int i = 0; i < fog->chunk_count; i++) {
        if (fog->chunks[i].allocated && 
            fog->chunks[i].chunk_x == chunk_x && 
            fog->chunks[i].chunk_y == chunk_y) {
            return &fog->chunks[i];
        }
    }
    return NULL;
}

// Find chunk (const version)
static const FogChunk* fog_find_chunk_const(const FogOfWarState* fog, int chunk_x, int chunk_y) {
    for (int i = 0; i < fog->chunk_count; i++) {
        if (fog->chunks[i].allocated && 
            fog->chunks[i].chunk_x == chunk_x && 
            fog->chunks[i].chunk_y == chunk_y) {
            return &fog->chunks[i];
        }
    }
    return NULL;
}

// Get or create chunk at coordinates
static FogChunk* fog_get_or_create_chunk(FogOfWarState* fog, int chunk_x, int chunk_y) {
    // Try to find existing
    FogChunk* chunk = fog_find_chunk(fog, chunk_x, chunk_y);
    if (chunk) return chunk;
    
    // Allocate new if space available
    if (fog->chunk_count >= FOG_MAX_CHUNKS) {
        return NULL;  // Out of chunks
    }
    
    chunk = &fog->chunks[fog->chunk_count++];
    chunk->chunk_x = chunk_x;
    chunk->chunk_y = chunk_y;
    chunk->allocated = true;
    memset(chunk->cells, 0, sizeof(chunk->cells));  // All cells start fogged
    
    return chunk;
}

// =============================================================================
// Configuration
// =============================================================================

void fog_set_enabled(FogOfWarState* fog, bool enabled) {
    if (!fog || !fog->initialized) return;
    fog->enabled = enabled;
}

void fog_set_reveal_radius(FogOfWarState* fog, float radius) {
    if (!fog || !fog->initialized) return;
    fog->reveal_radius = radius;
}

void fog_set_discovery_radius(FogOfWarState* fog, float radius) {
    if (!fog || !fog->initialized) return;
    fog->discovery_radius = radius;
}

void fog_set_prototype_mode(FogOfWarState* fog, bool enabled) {
    if (!fog || !fog->initialized) return;
    fog->prototype_mode = enabled;
}

// =============================================================================
// Visibility Queries
// =============================================================================

VisibilityState fog_get_poi_visibility(const FogOfWarState* fog, int poi_index) {
    if (!fog || !fog->initialized) return VISIBILITY_HIDDEN;
    if (poi_index < 0 || poi_index >= MAX_POIS) return VISIBILITY_HIDDEN;
    
    // In prototype mode, POIs are always at least "on the map" for navigation
    // but visibility state still tracks whether they've been approached
    // Return actual visibility state - the rendering code handles fogged icons
    return fog->poi_visibility[poi_index];
}

float fog_get_poi_alpha(const FogOfWarState* fog, int poi_index) {
    if (!fog || !fog->initialized) return 1.0f;
    if (poi_index < 0 || poi_index >= MAX_POIS) return 1.0f;
    
    if (!fog->enabled) return 0.0f;  // No fog when disabled
    
    // Return actual fog alpha - prototype mode just affects rendering (always show icon)
    return fog->poi_fog_alpha[poi_index];
}

bool fog_is_position_visible(const FogOfWarState* fog, const ECSWorld* ecs_world,
                             ComponentMask ship_mask, float x, float y) {
    if (!fog || !fog->initialized || !ecs_world) return true;
    if (!fog->enabled) return true;
    
    float reveal_radius_sq = fog->reveal_radius * fog->reveal_radius;
    
    // Check distance to all ships
    for (Entity e = 1; e < MAX_ENTITIES; e++) {
        ComponentMask mask = ecs_get_mask(ecs_world, e);
        if ((mask & ship_mask) != ship_mask) continue;
        if (!(mask & COMPONENT_TRANSFORM)) continue;
        
        float ship_x = ecs_world->transforms.pos_x[e];
        float ship_y = ecs_world->transforms.pos_y[e];
        
        float dist_sq = distance_squared(x, y, ship_x, ship_y);
        if (dist_sq <= reveal_radius_sq) {
            return true;
        }
    }
    
    return false;
}

bool fog_is_position_revealed(const FogOfWarState* fog, float x, float y) {
    if (!fog || !fog->initialized) return true;
    if (!fog->enabled) return true;
    
    int chunk_x, chunk_y;
    world_to_chunk(x, y, &chunk_x, &chunk_y);
    
    const FogChunk* chunk = fog_find_chunk_const(fog, chunk_x, chunk_y);
    if (!chunk) return false;  // Unallocated chunks are fogged
    
    int cell_x, cell_y;
    world_to_cell_in_chunk(x, y, chunk_x, chunk_y, &cell_x, &cell_y);
    
    return chunk->cells[cell_y][cell_x] != 0;
}

// =============================================================================
// Visibility Modification
// =============================================================================

void fog_reveal_area(FogOfWarState* fog, float x, float y, float radius) {
    if (!fog || !fog->initialized) return;
    
    // Calculate bounding box in world coords
    float min_x = x - radius;
    float max_x = x + radius;
    float min_y = y - radius;
    float max_y = y + radius;
    
    // Find chunk range
    int min_chunk_x, min_chunk_y, max_chunk_x, max_chunk_y;
    world_to_chunk(min_x, min_y, &min_chunk_x, &min_chunk_y);
    world_to_chunk(max_x, max_y, &max_chunk_x, &max_chunk_y);
    
    float radius_sq = radius * radius;
    
    // Iterate over all affected chunks
    for (int cy = min_chunk_y; cy <= max_chunk_y; cy++) {
        for (int cx = min_chunk_x; cx <= max_chunk_x; cx++) {
            FogChunk* chunk = fog_get_or_create_chunk(fog, cx, cy);
            if (!chunk) continue;  // Out of chunks
            
            // Calculate cell range within this chunk
            float chunk_origin_x = cx * FOG_CHUNK_WORLD_SIZE;
            float chunk_origin_y = cy * FOG_CHUNK_WORLD_SIZE;
            
            int cell_min_x = (int)((min_x - chunk_origin_x) / FOG_CELL_SIZE);
            int cell_max_x = (int)((max_x - chunk_origin_x) / FOG_CELL_SIZE);
            int cell_min_y = (int)((min_y - chunk_origin_y) / FOG_CELL_SIZE);
            int cell_max_y = (int)((max_y - chunk_origin_y) / FOG_CELL_SIZE);
            
            // Clamp to chunk bounds
            if (cell_min_x < 0) cell_min_x = 0;
            if (cell_min_y < 0) cell_min_y = 0;
            if (cell_max_x >= FOG_CHUNK_SIZE) cell_max_x = FOG_CHUNK_SIZE - 1;
            if (cell_max_y >= FOG_CHUNK_SIZE) cell_max_y = FOG_CHUNK_SIZE - 1;
            
            // Mark cells within radius
            for (int cell_y = cell_min_y; cell_y <= cell_max_y; cell_y++) {
                for (int cell_x = cell_min_x; cell_x <= cell_max_x; cell_x++) {
                    // Cell center in world coords
                    float cell_world_x = chunk_origin_x + (cell_x + 0.5f) * FOG_CELL_SIZE;
                    float cell_world_y = chunk_origin_y + (cell_y + 0.5f) * FOG_CELL_SIZE;
                    
                    float dist_sq = distance_squared(x, y, cell_world_x, cell_world_y);
                    if (dist_sq <= radius_sq) {
                        chunk->cells[cell_y][cell_x] = 1;
                    }
                }
            }
        }
    }
}

void fog_reveal_poi(FogOfWarState* fog, int poi_index) {
    if (!fog || !fog->initialized) return;
    if (poi_index < 0 || poi_index >= MAX_POIS) return;
    
    fog->poi_visibility[poi_index] = VISIBILITY_VISIBLE;
    fog->poi_fog_alpha[poi_index] = 0.0f;
}

void fog_discover_poi(FogOfWarState* fog, int poi_index) {
    if (!fog || !fog->initialized) return;
    if (poi_index < 0 || poi_index >= MAX_POIS) return;
    
    // Only upgrade from hidden
    if (fog->poi_visibility[poi_index] == VISIBILITY_HIDDEN) {
        fog->poi_visibility[poi_index] = VISIBILITY_DISCOVERED;
    }
}

void fog_reveal_all(FogOfWarState* fog, const POIEcsWorld* poi_world) {
    if (!fog || !fog->initialized || !poi_world) return;
    
    uint32_t count = poi_ecs_get_count(poi_world);
    for (uint32_t i = 0; i < count; i++) {
        fog->poi_visibility[i] = VISIBILITY_VISIBLE;
        fog->poi_fog_alpha[i] = 0.0f;
    }
}

// =============================================================================
// System Update
// =============================================================================

void fog_system_update(FogOfWarState* fog, const POIEcsWorld* poi_world,
                       const ECSWorld* ecs_world, ComponentMask ship_mask,
                       float delta_time) {
    if (!fog || !fog->initialized || !poi_world || !ecs_world) return;
    if (!fog->enabled) return;
    
    float reveal_radius_sq = fog->reveal_radius * fog->reveal_radius;
    float discovery_radius_sq = fog->discovery_radius * fog->discovery_radius;
    
    // Cache ship positions once (avoid repeated entity iteration)
    typedef struct { float x, y; } ShipPos;
    ShipPos ships[FOG_MAX_TRACKED_SHIPS];
    int ship_count = 0;
    
    for (Entity e = 1; e < MAX_ENTITIES && ship_count < FOG_MAX_TRACKED_SHIPS; e++) {
        ComponentMask mask = ecs_get_mask(ecs_world, e);
        if ((mask & ship_mask) != ship_mask) continue;
        if (!(mask & COMPONENT_TRANSFORM)) continue;
        
        ships[ship_count].x = ecs_world->transforms.pos_x[e];
        ships[ship_count].y = ecs_world->transforms.pos_y[e];
        ship_count++;
    }
    
    // Reveal areas around ships (with stationary optimization)
    for (int i = 0; i < ship_count; i++) {
        float dx = ships[i].x - fog->last_reveal_x[i];
        float dy = ships[i].y - fog->last_reveal_y[i];
        float dist_sq = dx * dx + dy * dy;
        
        // Only reveal if ship moved more than half a cell
        if (dist_sq > FOG_MOVE_THRESHOLD_SQ || fog->tracked_ship_count != ship_count) {
            fog_reveal_area(fog, ships[i].x, ships[i].y, fog->reveal_radius);
            fog->last_reveal_x[i] = ships[i].x;
            fog->last_reveal_y[i] = ships[i].y;
        }
    }
    fog->tracked_ship_count = ship_count;
    
    uint32_t poi_count = poi_ecs_get_count(poi_world);
    
    // For each POI, check distance to cached ship positions
    for (uint32_t i = 0; i < poi_count; i++) {
        float poi_x, poi_y;
        poi_ecs_get_position(poi_world, (int)i, &poi_x, &poi_y);
        
        float min_dist_sq = 1e10f;  // Track closest ship
        
        // Find closest ship to this POI (using cached positions)
        for (int s = 0; s < ship_count; s++) {
            float dist_sq = distance_squared(poi_x, poi_y, ships[s].x, ships[s].y);
            if (dist_sq < min_dist_sq) {
                min_dist_sq = dist_sq;
            }
        }
        
        // Update visibility based on distance
        if (min_dist_sq <= reveal_radius_sq) {
            // Fully visible
            fog->poi_visibility[i] = VISIBILITY_VISIBLE;
        } else if (min_dist_sq <= discovery_radius_sq) {
            // Discovered but not fully visible
            if (fog->poi_visibility[i] == VISIBILITY_HIDDEN) {
                fog->poi_visibility[i] = VISIBILITY_DISCOVERED;
            }
        }
        
        // Update fog alpha for smooth transitions
        float target_alpha;
        switch (fog->poi_visibility[i]) {
            case VISIBILITY_VISIBLE:
                target_alpha = 0.0f;
                break;
            case VISIBILITY_DISCOVERED:
                target_alpha = 0.5f;
                break;
            case VISIBILITY_HIDDEN:
            default:
                target_alpha = 1.0f;
                break;
        }
        
        // Smooth interpolation
        float current_alpha = fog->poi_fog_alpha[i];
        float new_alpha = lerp_f(current_alpha, target_alpha, FOG_FADE_SPEED * delta_time);
        fog->poi_fog_alpha[i] = clamp_f(new_alpha, 0.0f, 1.0f);
    }
}

// =============================================================================
// Rendering Helpers
// =============================================================================

void fog_get_render_color(const FogOfWarState* fog, int poi_index,
                          unsigned char* r, unsigned char* g,
                          unsigned char* b, unsigned char* a) {
    // Default fog color (dark blue-gray)
    if (r) *r = 40;
    if (g) *g = 50;
    if (b) *b = 70;
    if (a) *a = 255;
    
    if (!fog || !fog->initialized) return;
    if (poi_index < 0 || poi_index >= MAX_POIS) return;
    
    float alpha = fog_get_poi_alpha(fog, poi_index);
    if (a) *a = (unsigned char)(alpha * 255.0f);
}
