#ifndef GAME_SATISFACTION_H
#define GAME_SATISFACTION_H

#include "game_poi_ecs.h"
#include <stdbool.h>
#include <stdint.h>

// =============================================================================
// Passenger Satisfaction System
// 
// Calculates passenger satisfaction based on POI visits during tours.
// Different POI types and tiers contribute differently to satisfaction.
// 
// Base satisfaction + POI bonuses + variety bonuses - penalties = Final score
// =============================================================================

// =============================================================================
// Constants
// =============================================================================

#define MAX_TOUR_POIS 64            // Maximum POIs in a single tour
#define BASE_SATISFACTION 50        // Starting satisfaction for a tour
#define VARIETY_BONUS_THRESHOLD 3   // Number of different types for variety bonus

// Satisfaction score thresholds
#define SATISFACTION_POOR 40
#define SATISFACTION_AVERAGE 60
#define SATISFACTION_GOOD 75
#define SATISFACTION_EXCELLENT 90

// =============================================================================
// Satisfaction Rating
// =============================================================================

typedef enum SatisfactionRating {
    RATING_POOR = 0,        // < 40
    RATING_AVERAGE = 1,     // 40-59
    RATING_GOOD = 2,        // 60-74
    RATING_EXCELLENT = 3,   // 75-89
    RATING_OUTSTANDING = 4  // 90+
} SatisfactionRating;

// =============================================================================
// Tour Tracking
// =============================================================================

typedef struct TourSatisfaction {
    // POIs visited during this tour
    int visited_poi_indices[MAX_TOUR_POIS];
    uint32_t visited_count;
    
    // Running totals
    int poi_bonus_total;            // Sum of POI satisfaction bonuses
    int variety_bonus;              // Bonus for visiting different types
    int special_bonus;              // Bonus for special tier POIs
    
    // Type tracking for variety bonus
    bool has_nature;
    bool has_historical;
    bool has_military;
    
    // Tier tracking
    uint32_t general_count;
    uint32_t special_count;
    
    // Target tracking
    uint32_t total_pois_target;     // Total POIs available/planned (for completion bonus)
    
    // Final calculation
    int total_satisfaction;
    SatisfactionRating rating;
    
    bool active;                    // Is a tour in progress?
} TourSatisfaction;

// =============================================================================
// Tour Lifecycle
// =============================================================================

// Start a new tour (resets tracking)
// total_pois: Number of POIs available to visit (pass 0 if unknown)
void satisfaction_tour_start(TourSatisfaction* tour, uint32_t total_pois);

// End current tour (calculates final score)
void satisfaction_tour_end(TourSatisfaction* tour);

// Check if tour is active
bool satisfaction_tour_is_active(const TourSatisfaction* tour);

// =============================================================================
// POI Visit Recording
// =============================================================================

// Record a POI visit during the tour
// Returns the satisfaction bonus gained from this visit
int satisfaction_record_poi_visit(TourSatisfaction* tour, const POIEcsWorld* poi_world,
                                  int poi_index);

// Check if a POI was already visited this tour
bool satisfaction_was_poi_visited(const TourSatisfaction* tour, int poi_index);

// =============================================================================
// Score Calculation
// =============================================================================

// Calculate current satisfaction score (can be called mid-tour)
int satisfaction_calculate_score(const TourSatisfaction* tour);

// Get satisfaction rating from score
SatisfactionRating satisfaction_get_rating(int score);

// Get rating as string
const char* satisfaction_rating_to_string(SatisfactionRating rating);

// =============================================================================
// Bonuses and Multipliers
// =============================================================================

// Type-based bonus multipliers
typedef struct SatisfactionConfig {
    float nature_multiplier;        // Default: 1.0
    float historical_multiplier;    // Default: 1.2
    float military_multiplier;      // Default: 1.1
    
    float special_tier_multiplier;  // Default: 1.5
    
    int variety_bonus_2_types;      // Default: 5
    int variety_bonus_3_types;      // Default: 15
    
    int base_satisfaction;          // Default: 50
} SatisfactionConfig;

// Get default configuration
SatisfactionConfig satisfaction_get_default_config(void);

// Calculate bonus for a specific POI with config
int satisfaction_calculate_poi_bonus(const POIEcsWorld* poi_world, int poi_index,
                                     const SatisfactionConfig* config);

// =============================================================================
// Statistics
// =============================================================================

typedef struct TourStats {
    int total_satisfaction;
    SatisfactionRating rating;
    uint32_t pois_visited;
    uint32_t nature_visited;
    uint32_t historical_visited;
    uint32_t military_visited;
    uint32_t general_visited;
    uint32_t special_visited;
    int poi_bonus;
    int variety_bonus;
} TourStats;

// Get tour statistics
TourStats satisfaction_get_tour_stats(const TourSatisfaction* tour);

// =============================================================================
// Debug
// =============================================================================

// Print tour satisfaction summary
void satisfaction_debug_print(const TourSatisfaction* tour);

#endif // GAME_SATISFACTION_H
