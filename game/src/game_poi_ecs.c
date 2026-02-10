#include "game_poi_ecs.h"
#include <string.h>
#include <math.h>
#include <stdio.h>

#ifndef DEG2RAD
#define DEG2RAD (3.14159f / 180.0f)
#endif

// =============================================================================
// Internal Utilities
// =============================================================================

static float distance_squared(float x1, float y1, float x2, float y2) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    return dx * dx + dy * dy;
}

// =============================================================================
// POI ECS Lifecycle
// =============================================================================

void poi_ecs_init(POIEcsWorld* poi_world) {
    if (!poi_world) return;
    
    memset(poi_world, 0, sizeof(POIEcsWorld));
    poi_world->initialized = true;
    
    // Set all POIs as undiscovered by default
    for (int i = 0; i < MAX_POIS; i++) {
        poi_world->pois.discovered[i] = false;
        poi_world->pois.visited[i] = false;
        poi_world->pois.sprite_ids[i] = TEXTURE_NONE;
    }
}

void poi_ecs_shutdown(POIEcsWorld* poi_world) {
    if (!poi_world) return;
    
    poi_world->initialized = false;
    poi_world->poi_count = 0;
}

void poi_ecs_clear(POIEcsWorld* poi_world) {
    if (!poi_world || !poi_world->initialized) return;
    
    memset(&poi_world->pois, 0, sizeof(POIComponents));
    poi_world->poi_count = 0;
}

// =============================================================================
// POI Creation
// =============================================================================

int poi_ecs_create(POIEcsWorld* poi_world, const POICreateParams* params) {
    if (!poi_world || !poi_world->initialized || !params) return -1;
    if (poi_world->poi_count >= MAX_POIS) return -1;
    
    int idx = (int)poi_world->poi_count;
    
    // Copy name
    if (params->name) {
        strncpy(poi_world->pois.name[idx], params->name, POI_NAME_MAX_LENGTH - 1);
        poi_world->pois.name[idx][POI_NAME_MAX_LENGTH - 1] = '\0';
    } else {
        snprintf(poi_world->pois.name[idx], POI_NAME_MAX_LENGTH, "POI_%d", idx);
    }
    
    // Copy description
    if (params->description) {
        strncpy(poi_world->pois.description[idx], params->description, POI_DESCRIPTION_MAX_LENGTH - 1);
        poi_world->pois.description[idx][POI_DESCRIPTION_MAX_LENGTH - 1] = '\0';
    } else {
        poi_world->pois.description[idx][0] = '\0';
    }
    
    // Set classification
    poi_world->pois.type[idx] = (uint8_t)params->type;
    poi_world->pois.tier[idx] = (uint8_t)params->tier;
    
    // Set position
    poi_world->pois.pos_x[idx] = params->x;
    poi_world->pois.pos_y[idx] = params->y;
    
    // Set radius (use default if not specified)
    poi_world->pois.radius[idx] = (params->radius > 0.0f) ? params->radius : POI_DEFAULT_RADIUS;
    
    // Set satisfaction bonus (use tier default if not specified)
    if (params->satisfaction_bonus > 0) {
        poi_world->pois.satisfaction_bonus[idx] = params->satisfaction_bonus;
    } else {
        poi_world->pois.satisfaction_bonus[idx] = 
            (params->tier == POI_TIER_SPECIAL) ? POI_SATISFACTION_SPECIAL : POI_SATISFACTION_GENERAL;
    }
    
    // Initialize state
    poi_world->pois.visited[idx] = false;
    poi_world->pois.discovered[idx] = true;  // All visible for prototype
    poi_world->pois.visit_count[idx] = 0;
    
    // Pre-compute sprite ID at load time (optimization: avoid strstr in render loop)
    // Check if name contains "Lighthouse" or "lighthouse"
    if (params->name && (strstr(params->name, "Lighthouse") != NULL || strstr(params->name, "lighthouse") != NULL)) {
        poi_world->pois.sprite_ids[idx] = TEXTURE_LIGHTHOUSE;
    } else {
        poi_world->pois.sprite_ids[idx] = TEXTURE_NONE;
    }
    
    poi_world->poi_count++;
    return idx;
}

void poi_ecs_destroy(POIEcsWorld* poi_world, int poi_index) {
    if (!poi_world || !poi_world->initialized) return;
    if (poi_index < 0 || poi_index >= (int)poi_world->poi_count) return;
    
    // Swap with last element if not already last
    int last = (int)poi_world->poi_count - 1;
    if (poi_index != last) {
        // Copy last element to this position
        memcpy(poi_world->pois.name[poi_index], poi_world->pois.name[last], POI_NAME_MAX_LENGTH);
        memcpy(poi_world->pois.description[poi_index], poi_world->pois.description[last], POI_DESCRIPTION_MAX_LENGTH);
        poi_world->pois.type[poi_index] = poi_world->pois.type[last];
        poi_world->pois.tier[poi_index] = poi_world->pois.tier[last];
        poi_world->pois.pos_x[poi_index] = poi_world->pois.pos_x[last];
        poi_world->pois.pos_y[poi_index] = poi_world->pois.pos_y[last];
        poi_world->pois.radius[poi_index] = poi_world->pois.radius[last];
        poi_world->pois.satisfaction_bonus[poi_index] = poi_world->pois.satisfaction_bonus[last];
        poi_world->pois.visited[poi_index] = poi_world->pois.visited[last];
        poi_world->pois.discovered[poi_index] = poi_world->pois.discovered[last];
        poi_world->pois.visit_count[poi_index] = poi_world->pois.visit_count[last];
        poi_world->pois.sprite_ids[poi_index] = poi_world->pois.sprite_ids[last];
    }
    
    poi_world->poi_count--;
}

// =============================================================================
// POI Accessors
// =============================================================================

uint32_t poi_ecs_get_count(const POIEcsWorld* poi_world) {
    if (!poi_world || !poi_world->initialized) return 0;
    return poi_world->poi_count;
}

bool poi_ecs_is_valid(const POIEcsWorld* poi_world, int poi_index) {
    if (!poi_world || !poi_world->initialized) return false;
    return poi_index >= 0 && poi_index < (int)poi_world->poi_count;
}

const char* poi_ecs_get_name(const POIEcsWorld* poi_world, int poi_index) {
    if (!poi_ecs_is_valid(poi_world, poi_index)) return "";
    return poi_world->pois.name[poi_index];
}

POIType poi_ecs_get_type(const POIEcsWorld* poi_world, int poi_index) {
    if (!poi_ecs_is_valid(poi_world, poi_index)) return POI_TYPE_NATURE;
    return (POIType)poi_world->pois.type[poi_index];
}

POITier poi_ecs_get_tier(const POIEcsWorld* poi_world, int poi_index) {
    if (!poi_ecs_is_valid(poi_world, poi_index)) return POI_TIER_GENERAL;
    return (POITier)poi_world->pois.tier[poi_index];
}

void poi_ecs_get_position(const POIEcsWorld* poi_world, int poi_index, float* x, float* y) {
    if (!poi_ecs_is_valid(poi_world, poi_index)) {
        if (x) *x = 0.0f;
        if (y) *y = 0.0f;
        return;
    }
    if (x) *x = poi_world->pois.pos_x[poi_index];
    if (y) *y = poi_world->pois.pos_y[poi_index];
}

float poi_ecs_get_radius(const POIEcsWorld* poi_world, int poi_index) {
    if (!poi_ecs_is_valid(poi_world, poi_index)) return 0.0f;
    return poi_world->pois.radius[poi_index];
}

int poi_ecs_get_satisfaction_bonus(const POIEcsWorld* poi_world, int poi_index) {
    if (!poi_ecs_is_valid(poi_world, poi_index)) return 0;
    return poi_world->pois.satisfaction_bonus[poi_index];
}

bool poi_ecs_is_visited(const POIEcsWorld* poi_world, int poi_index) {
    if (!poi_ecs_is_valid(poi_world, poi_index)) return false;
    return poi_world->pois.visited[poi_index];
}

bool poi_ecs_is_discovered(const POIEcsWorld* poi_world, int poi_index) {
    if (!poi_ecs_is_valid(poi_world, poi_index)) return false;
    return poi_world->pois.discovered[poi_index];
}

TextureID poi_ecs_get_sprite_id(const POIEcsWorld* poi_world, int poi_index) {
    if (!poi_ecs_is_valid(poi_world, poi_index)) return TEXTURE_NONE;
    return poi_world->pois.sprite_ids[poi_index];
}

// =============================================================================
// POI Viewing Status
// =============================================================================

POIViewStatus poi_ecs_get_view_status(const POIEcsWorld* poi_world, int poi_index, 
                                     float ship_x, float ship_y,
                                     float ship_rot, float ship_speed) {
    POIViewStatus status = {0};
    
    if (!poi_ecs_is_valid(poi_world, poi_index)) return status;
    
    float poi_x = poi_world->pois.pos_x[poi_index];
    float poi_y = poi_world->pois.pos_y[poi_index];
    float poi_radius = poi_world->pois.radius[poi_index];
    
    // Distance check
    float dist_sq = distance_squared(ship_x, ship_y, poi_x, poi_y);
    status.distance = sqrtf(dist_sq);
    
    float view_radius = poi_radius * POI_VIEW_MAX_DIST_MULT;
    status.in_range = (dist_sq <= view_radius * view_radius);
    
    // Speed check
    status.current_speed = ship_speed;
    status.speed_ok = (ship_speed <= POI_VIEW_MAX_SPEED);
    
    // Angle check (calculate always to allow alignment from distance)
    if (1) {
        // Ship forward vector
        float rad = ship_rot * DEG2RAD;
        float dir_x = cosf(rad);
        float dir_y = sinf(rad);
        
        // Vector to POI
        float to_poi_x = poi_x - ship_x;
        float to_poi_y = poi_y - ship_y;
        float to_poi_len = status.distance;
        
        if (to_poi_len > 0.001f) {
            to_poi_x /= to_poi_len;
            to_poi_y /= to_poi_len;
            
            // Dot product
            float dot = dir_x * to_poi_x + dir_y * to_poi_y;
            // Abs dot: 1.0 = parallel (head-on/away), 0.0 = perpendicular (broadside)
            status.alignment = fabsf(dot);
            
            // We want broadside, so low alignment value
            status.angle_ok = (status.alignment <= POI_VIEW_ALIGNMENT_TOLERANCE);
        } else {
            status.angle_ok = true; // On top of it
            status.alignment = 0.0f;
        }
    }
    
    // Combined status
    status.is_viewable = status.in_range && status.speed_ok && status.angle_ok;
    
    // Calculate quality score (0.0 to 1.0)
    if (status.is_viewable) {
        // Closer is better (up to a point, say 1.0 radius)
        float dist_score = 1.0f - (status.distance / view_radius);
        if (status.distance < poi_radius) dist_score = 1.0f;
        
        // Slower is better
        float speed_score = 1.0f - (status.current_speed / POI_VIEW_MAX_SPEED);
        
        // Broadside is better (0.0 alignment is best)
        float angle_score = 1.0f - (status.alignment / POI_VIEW_ALIGNMENT_TOLERANCE);
        
        status.quality_score = (dist_score * 0.3f) + (speed_score * 0.3f) + (angle_score * 0.4f);
    } else {
        status.quality_score = 0.0f;
    }
    
    return status;
}

// =============================================================================
// POI State Modification
// =============================================================================

void poi_ecs_set_visited(POIEcsWorld* poi_world, int poi_index, bool visited) {
    if (!poi_ecs_is_valid(poi_world, poi_index)) return;
    poi_world->pois.visited[poi_index] = visited;
}

void poi_ecs_reset_all_visited(POIEcsWorld* poi_world) {
    if (!poi_world || !poi_world->initialized) return;
    
    for (uint32_t i = 0; i < poi_world->poi_count; i++) {
        poi_world->pois.visited[i] = false;
    }
}

void poi_ecs_set_discovered(POIEcsWorld* poi_world, int poi_index, bool discovered) {
    if (!poi_ecs_is_valid(poi_world, poi_index)) return;
    poi_world->pois.discovered[poi_index] = discovered;
}

void poi_ecs_increment_visits(POIEcsWorld* poi_world, int poi_index) {
    if (!poi_ecs_is_valid(poi_world, poi_index)) return;
    poi_world->pois.visit_count[poi_index]++;
}

// =============================================================================
// POI Queries
// =============================================================================

int poi_ecs_find_at_position(const POIEcsWorld* poi_world, float x, float y) {
    if (!poi_world || !poi_world->initialized) return -1;
    
    for (uint32_t i = 0; i < poi_world->poi_count; i++) {
        float radius = poi_world->pois.radius[i];
        float dist_sq = distance_squared(x, y, poi_world->pois.pos_x[i], poi_world->pois.pos_y[i]);
        if (dist_sq <= radius * radius) {
            return (int)i;
        }
    }
    return -1;
}

int poi_ecs_find_in_range(const POIEcsWorld* poi_world, float x, float y, float range,
                          int* out_indices, int max_results) {
    if (!poi_world || !poi_world->initialized || !out_indices || max_results <= 0) return 0;
    
    int count = 0;
    float range_sq = range * range;
    
    for (uint32_t i = 0; i < poi_world->poi_count && count < max_results; i++) {
        float dist_sq = distance_squared(x, y, poi_world->pois.pos_x[i], poi_world->pois.pos_y[i]);
        if (dist_sq <= range_sq) {
            out_indices[count++] = (int)i;
        }
    }
    return count;
}

int poi_ecs_find_by_name(const POIEcsWorld* poi_world, const char* name) {
    if (!poi_world || !poi_world->initialized || !name) return -1;
    
    for (uint32_t i = 0; i < poi_world->poi_count; i++) {
        // Case-insensitive comparison
        #ifdef _WIN32
        if (_stricmp(poi_world->pois.name[i], name) == 0) {
            return (int)i;
        }
        #else
        if (strcasecmp(poi_world->pois.name[i], name) == 0) {
            return (int)i;
        }
        #endif
    }
    return -1;
}

bool poi_ecs_check_visit(const POIEcsWorld* poi_world, int poi_index, float x, float y) {
    if (!poi_ecs_is_valid(poi_world, poi_index)) return false;
    
    float radius = poi_world->pois.radius[poi_index];
    float dist_sq = distance_squared(x, y, poi_world->pois.pos_x[poi_index], poi_world->pois.pos_y[poi_index]);
    return dist_sq <= radius * radius;
}

// =============================================================================
// POI System Update
// =============================================================================

void poi_ecs_system_update(POIEcsWorld* poi_world, const ECSWorld* ecs_world,
                           ComponentMask ship_mask, const POISystemContext* context) {
    if (!poi_world || !poi_world->initialized || !ecs_world) return;
    
    // Iterate all ships and check proximity to POIs
    for (Entity e = 1; e < MAX_ENTITIES; e++) {
        ComponentMask mask = ecs_get_mask(ecs_world, e);
        if ((mask & ship_mask) != ship_mask) continue;
        if (!(mask & COMPONENT_TRANSFORM)) continue;
        
        float ship_x = ecs_world->transforms.pos_x[e];
        float ship_y = ecs_world->transforms.pos_y[e];
        
        // Check each POI
        for (uint32_t i = 0; i < poi_world->poi_count; i++) {
            float radius = poi_world->pois.radius[i];
            float dist_sq = distance_squared(ship_x, ship_y, poi_world->pois.pos_x[i], poi_world->pois.pos_y[i]);
            
            bool is_close_visit = (dist_sq <= (radius * 0.25f) * (radius * 0.25f)); // Only visit if VERY close (collision)
            
            // Check advanced viewing
            float ship_speed = ecs_world->velocities.speed[e];
            float ship_rot = ecs_world->transforms.rotation[e];
            
            POIViewStatus status = poi_ecs_get_view_status(poi_world, (int)i, ship_x, ship_y, ship_rot, ship_speed);
            
            bool is_view_visit = status.is_viewable;
            
            if (is_close_visit || is_view_visit) {
                // Mark as visited if not already
                if (!poi_world->pois.visited[i]) {
                    poi_world->pois.visited[i] = true;
                    poi_world->pois.visit_count[i]++;
                    
                    // Fire callback
                    if (context && context->on_visit) {
                        context->on_visit((int)i, e, context->user_data);
                    }
                }
            }
        }
    }
}

// =============================================================================
// POI Statistics
// =============================================================================

POIStats poi_ecs_get_stats(const POIEcsWorld* poi_world) {
    POIStats stats = {0};
    
    if (!poi_world || !poi_world->initialized) return stats;
    
    stats.total_pois = poi_world->poi_count;
    
    for (uint32_t i = 0; i < poi_world->poi_count; i++) {
        if (poi_world->pois.visited[i]) stats.visited_pois++;
        if (poi_world->pois.discovered[i]) stats.discovered_pois++;
        
        switch (poi_world->pois.type[i]) {
            case POI_TYPE_NATURE:     stats.nature_count++; break;
            case POI_TYPE_HISTORICAL: stats.historical_count++; break;
            case POI_TYPE_MILITARY:   stats.military_count++; break;
            default: break;
        }
        
        switch (poi_world->pois.tier[i]) {
            case POI_TIER_GENERAL: stats.general_count++; break;
            case POI_TIER_SPECIAL: stats.special_count++; break;
            default: break;
        }
    }
    
    return stats;
}

// =============================================================================
// Utility Functions
// =============================================================================

const char* poi_type_to_string(POIType type) {
    switch (type) {
        case POI_TYPE_NATURE:     return "nature";
        case POI_TYPE_HISTORICAL: return "historical";
        case POI_TYPE_MILITARY:   return "military";
        default:                  return "unknown";
    }
}

const char* poi_tier_to_string(POITier tier) {
    switch (tier) {
        case POI_TIER_GENERAL: return "general";
        case POI_TIER_SPECIAL: return "special";
        default:               return "unknown";
    }
}

POIType poi_type_from_string(const char* str) {
    if (!str) return POI_TYPE_NATURE;
    
    #ifdef _WIN32
    if (_stricmp(str, "nature") == 0) return POI_TYPE_NATURE;
    if (_stricmp(str, "historical") == 0) return POI_TYPE_HISTORICAL;
    if (_stricmp(str, "military") == 0) return POI_TYPE_MILITARY;
    #else
    if (strcasecmp(str, "nature") == 0) return POI_TYPE_NATURE;
    if (strcasecmp(str, "historical") == 0) return POI_TYPE_HISTORICAL;
    if (strcasecmp(str, "military") == 0) return POI_TYPE_MILITARY;
    #endif
    
    return POI_TYPE_NATURE;
}

POITier poi_tier_from_string(const char* str) {
    if (!str) return POI_TIER_GENERAL;
    
    #ifdef _WIN32
    if (_stricmp(str, "general") == 0) return POI_TIER_GENERAL;
    if (_stricmp(str, "special") == 0) return POI_TIER_SPECIAL;
    #else
    if (strcasecmp(str, "general") == 0) return POI_TIER_GENERAL;
    if (strcasecmp(str, "special") == 0) return POI_TIER_SPECIAL;
    #endif
    
    return POI_TIER_GENERAL;
}
