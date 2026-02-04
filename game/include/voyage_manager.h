#ifndef VOYAGE_MANAGER_H
#define VOYAGE_MANAGER_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// Voyage Manager
// 
// MS Tour-specific voyage state machine for managing tour progression,
// POI tracking, and satisfaction scoring.
// =============================================================================

// =============================================================================
// Voyage State Enum
// =============================================================================

typedef enum VoyageState {
    VOYAGE_STATE_START = 0,       // Initial state, before voyage begins
    VOYAGE_STATE_NAVIGATE = 1,    // Traveling to next POI
    VOYAGE_STATE_POSITIONING = 2, // Approaching POI, entering cone
    VOYAGE_STATE_HOLDING = 3,     // Holding position, accumulating satisfaction
    VOYAGE_STATE_COMPLETE = 4,    // POI complete, advancing to next
    VOYAGE_STATE_FINISH = 5       // All POIs complete, end voyage
} VoyageState;

// =============================================================================
// Voyage Progress Structure
// =============================================================================

typedef struct VoyageProgress {
    VoyageState state;
    int current_poi_index;
    int total_pois;
    float voyage_duration;           // Total time elapsed (seconds)
    float* poi_satisfaction_scores;  // Dynamic array: satisfaction per POI
    bool voyage_active;
    float state_timer;               // Time in current state
} VoyageProgress;

// =============================================================================
// Voyage Manager Lifecycle
// =============================================================================

// Initialize voyage manager with number of POIs
// Must be called before any other voyage functions
void voyage_init(int num_pois);

// Shutdown and cleanup voyage manager resources
void voyage_shutdown(void);

// =============================================================================
// State Management
// =============================================================================

// Update voyage timers (call each frame)
void voyage_update(float delta_time);

// Start the voyage (transition from START to NAVIGATE)
void voyage_start(void);

// Get current voyage state
VoyageState voyage_get_state(void);

// Set voyage state (for state transitions)
void voyage_set_state(VoyageState new_state);

// =============================================================================
// POI Tracking
// =============================================================================

// Advance to next POI (increments current_poi_index)
void voyage_advance_to_next_poi(void);

// Get current POI index (0-based)
int voyage_get_current_poi_index(void);

// Record satisfaction score for current POI (0.0 - 1.0)
void voyage_record_poi_satisfaction(float satisfaction);

// =============================================================================
// Metrics & Queries
// =============================================================================

// Calculate average satisfaction across all completed POIs
float voyage_get_average_satisfaction(void);

// Get total voyage duration in seconds
float voyage_get_duration(void);

// Get pointer to voyage progress (read-only access)
const VoyageProgress* voyage_get_progress(void);

// =============================================================================
// State Queries
// =============================================================================

// Check if voyage is currently active (not in START or FINISH)
bool voyage_is_active(void);

// Check if voyage is complete (in FINISH state)
bool voyage_is_complete(void);

#ifdef __cplusplus
}
#endif

#endif // VOYAGE_MANAGER_H
