#include "game_satisfaction.h"
#include <string.h>
#include <stdio.h>

// =============================================================================
// Tour Lifecycle
// =============================================================================

void satisfaction_tour_start(TourSatisfaction* tour, uint32_t total_pois) {
    if (!tour) return;
    
    memset(tour, 0, sizeof(TourSatisfaction));
    tour->active = true;
    tour->total_pois_target = total_pois;
}

void satisfaction_tour_end(TourSatisfaction* tour) {
    if (!tour || !tour->active) return;
    
    // Calculate final score
    tour->total_satisfaction = satisfaction_calculate_score(tour);
    tour->rating = satisfaction_get_rating(tour->total_satisfaction);
    tour->active = false;
}

bool satisfaction_tour_is_active(const TourSatisfaction* tour) {
    if (!tour) return false;
    return tour->active;
}

// =============================================================================
// POI Visit Recording
// =============================================================================

int satisfaction_record_poi_visit(TourSatisfaction* tour, const POIEcsWorld* poi_world,
                                  int poi_index) {
    if (!tour || !tour->active || !poi_world) return 0;
    if (!poi_ecs_is_valid(poi_world, poi_index)) return 0;
    if (tour->visited_count >= MAX_TOUR_POIS) return 0;
    
    // Check if already visited this tour
    if (satisfaction_was_poi_visited(tour, poi_index)) return 0;
    
    // Record the visit
    tour->visited_poi_indices[tour->visited_count] = poi_index;
    tour->visited_count++;
    
    // Get POI info
    POIType type = poi_ecs_get_type(poi_world, poi_index);
    POITier tier = poi_ecs_get_tier(poi_world, poi_index);
    (void)tier;  // Used below in tracking
    
    // Apply type tracking
    switch (type) {
        case POI_TYPE_NATURE:
            tour->has_nature = true;
            break;
        case POI_TYPE_HISTORICAL:
            tour->has_historical = true;
            break;
        case POI_TYPE_MILITARY:
            tour->has_military = true;
            break;
        default:
            break;
    }
    
    // Apply tier tracking
    if (tier == POI_TIER_SPECIAL) {
        tour->special_count++;
    } else {
        tour->general_count++;
    }
    
    // Calculate bonus with config
    SatisfactionConfig config = satisfaction_get_default_config();
    int bonus = satisfaction_calculate_poi_bonus(poi_world, poi_index, &config);
    
    tour->poi_bonus_total += bonus;
    
    // Update variety bonus
    int types_visited = 0;
    if (tour->has_nature) types_visited++;
    if (tour->has_historical) types_visited++;
    if (tour->has_military) types_visited++;
    
    if (types_visited >= 3) {
        tour->variety_bonus = config.variety_bonus_3_types;
    } else if (types_visited >= 2) {
        tour->variety_bonus = config.variety_bonus_2_types;
    }
    
    return bonus;
}

bool satisfaction_was_poi_visited(const TourSatisfaction* tour, int poi_index) {
    if (!tour) return false;
    
    for (uint32_t i = 0; i < tour->visited_count; i++) {
        if (tour->visited_poi_indices[i] == poi_index) {
            return true;
        }
    }
    return false;
}

// =============================================================================
// Score Calculation
// =============================================================================

int satisfaction_calculate_score(const TourSatisfaction* tour) {
    if (!tour) return 0;
    
    SatisfactionConfig config = satisfaction_get_default_config();
    
    int score = config.base_satisfaction;
    score += tour->poi_bonus_total;
    score += tour->variety_bonus;
    
    // Check for completion bonus (100% satisfaction if all planned POIs visited)
    if (tour->total_pois_target > 0 && tour->visited_count >= tour->total_pois_target) {
        if (score < 100) score = 100;
    }
    
    // Clamp to 0-100
    if (score < 0) score = 0;
    if (score > 100) score = 100;
    
    return score;
}

SatisfactionRating satisfaction_get_rating(int score) {
    if (score >= SATISFACTION_EXCELLENT) return RATING_OUTSTANDING;
    if (score >= SATISFACTION_GOOD) return RATING_EXCELLENT;
    if (score >= SATISFACTION_AVERAGE) return RATING_GOOD;
    if (score >= SATISFACTION_POOR) return RATING_AVERAGE;
    return RATING_POOR;
}

const char* satisfaction_rating_to_string(SatisfactionRating rating) {
    switch (rating) {
        case RATING_POOR:        return "Poor";
        case RATING_AVERAGE:     return "Average";
        case RATING_GOOD:        return "Good";
        case RATING_EXCELLENT:   return "Excellent";
        case RATING_OUTSTANDING: return "Outstanding";
        default:                 return "Unknown";
    }
}

// =============================================================================
// Bonuses and Multipliers
// =============================================================================

SatisfactionConfig satisfaction_get_default_config(void) {
    SatisfactionConfig config;
    
    config.nature_multiplier = 1.0f;
    config.historical_multiplier = 1.2f;
    config.military_multiplier = 1.1f;
    
    config.special_tier_multiplier = 1.5f;
    
    config.variety_bonus_2_types = 5;
    config.variety_bonus_3_types = 15;
    
    config.base_satisfaction = BASE_SATISFACTION;
    
    return config;
}

int satisfaction_calculate_poi_bonus(const POIEcsWorld* poi_world, int poi_index,
                                     const SatisfactionConfig* config) {
    if (!poi_world || !config) return 0;
    if (!poi_ecs_is_valid(poi_world, poi_index)) return 0;
    
    int base_bonus = poi_ecs_get_satisfaction_bonus(poi_world, poi_index);
    POIType type = poi_ecs_get_type(poi_world, poi_index);
    POITier tier = poi_ecs_get_tier(poi_world, poi_index);
    
    // Apply type multiplier
    float type_mult = 1.0f;
    switch (type) {
        case POI_TYPE_NATURE:
            type_mult = config->nature_multiplier;
            break;
        case POI_TYPE_HISTORICAL:
            type_mult = config->historical_multiplier;
            break;
        case POI_TYPE_MILITARY:
            type_mult = config->military_multiplier;
            break;
        default:
            break;
    }
    
    // Apply tier multiplier
    float tier_mult = 1.0f;
    if (tier == POI_TIER_SPECIAL) {
        tier_mult = config->special_tier_multiplier;
    }
    
    float bonus = (float)base_bonus * type_mult * tier_mult;
    return (int)(bonus + 0.5f);  // Round to nearest
}

// =============================================================================
// Statistics
// =============================================================================

TourStats satisfaction_get_tour_stats(const TourSatisfaction* tour) {
    TourStats stats = {0};
    
    if (!tour) return stats;
    
    stats.total_satisfaction = tour->active ? 
        satisfaction_calculate_score(tour) : tour->total_satisfaction;
    stats.rating = satisfaction_get_rating(stats.total_satisfaction);
    stats.pois_visited = tour->visited_count;
    stats.general_visited = tour->general_count;
    stats.special_visited = tour->special_count;
    stats.poi_bonus = tour->poi_bonus_total;
    stats.variety_bonus = tour->variety_bonus;
    
    // Count types (need to track separately if POI world not available)
    if (tour->has_nature) stats.nature_visited++;
    if (tour->has_historical) stats.historical_visited++;
    if (tour->has_military) stats.military_visited++;
    
    return stats;
}

// =============================================================================
// Debug
// =============================================================================

void satisfaction_debug_print(const TourSatisfaction* tour) {
    if (!tour) {
        printf("[Satisfaction] No tour data\n");
        return;
    }
    
    TourStats stats = satisfaction_get_tour_stats(tour);
    
    printf("[Satisfaction] Tour %s\n", tour->active ? "(Active)" : "(Complete)");
    printf("  Score: %d (%s)\n", stats.total_satisfaction, 
           satisfaction_rating_to_string(stats.rating));
    printf("  POIs Visited: %u / %u\n", stats.pois_visited, tour->total_pois_target);
    printf("  Target reached: %s\n", 
           (tour->total_pois_target > 0 && stats.pois_visited >= tour->total_pois_target) ? "YES" : "NO");
    printf("    Nature: %s, Historical: %s, Military: %s\n",
           tour->has_nature ? "Yes" : "No",
           tour->has_historical ? "Yes" : "No",
           tour->has_military ? "Yes" : "No");
    printf("    General: %u, Special: %u\n", stats.general_visited, stats.special_visited);
    printf("  Bonuses: POI=%d, Variety=%d\n", stats.poi_bonus, stats.variety_bonus);
}
