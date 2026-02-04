#include "voyage_manager.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// =============================================================================
// Global Voyage State
// =============================================================================

static VoyageProgress g_voyage = {0};

// =============================================================================
// Voyage Manager Lifecycle
// =============================================================================

void voyage_init(int num_pois) {
    // Reset all state
    memset(&g_voyage, 0, sizeof(VoyageProgress));
    
    g_voyage.state = VOYAGE_STATE_START;
    g_voyage.total_pois = num_pois;
    g_voyage.current_poi_index = 0;
    g_voyage.voyage_duration = 0.0f;
    g_voyage.voyage_active = false;
    g_voyage.state_timer = 0.0f;
    
    // Allocate satisfaction score array
    if (num_pois > 0) {
        g_voyage.poi_satisfaction_scores = (float*)malloc(sizeof(float) * num_pois);
        if (g_voyage.poi_satisfaction_scores) {
            // Initialize all scores to 0
            for (int i = 0; i < num_pois; i++) {
                g_voyage.poi_satisfaction_scores[i] = 0.0f;
            }
        } else {
            fprintf(stderr, "voyage_init: Failed to allocate satisfaction scores array\n");
        }
    } else {
        g_voyage.poi_satisfaction_scores = NULL;
    }
}

void voyage_shutdown(void) {
    // Free allocated memory
    if (g_voyage.poi_satisfaction_scores) {
        free(g_voyage.poi_satisfaction_scores);
        g_voyage.poi_satisfaction_scores = NULL;
    }
    
    // Reset state
    memset(&g_voyage, 0, sizeof(VoyageProgress));
}

// =============================================================================
// State Management
// =============================================================================

void voyage_update(float delta_time) {
    if (g_voyage.voyage_active) {
        g_voyage.voyage_duration += delta_time;
        g_voyage.state_timer += delta_time;
    }
}

void voyage_start(void) {
    if (g_voyage.state == VOYAGE_STATE_START) {
        g_voyage.state = VOYAGE_STATE_NAVIGATE;
        g_voyage.voyage_active = true;
        g_voyage.state_timer = 0.0f;
    }
}

VoyageState voyage_get_state(void) {
    return g_voyage.state;
}

void voyage_set_state(VoyageState new_state) {
    g_voyage.state = new_state;
    g_voyage.state_timer = 0.0f;
    
    // Update active flag based on state
    if (new_state == VOYAGE_STATE_START || new_state == VOYAGE_STATE_FINISH) {
        g_voyage.voyage_active = false;
    } else {
        g_voyage.voyage_active = true;
    }
}

// =============================================================================
// POI Tracking
// =============================================================================

void voyage_advance_to_next_poi(void) {
    if (g_voyage.current_poi_index < g_voyage.total_pois) {
        g_voyage.current_poi_index++;
    }
}

int voyage_get_current_poi_index(void) {
    return g_voyage.current_poi_index;
}

void voyage_record_poi_satisfaction(float satisfaction) {
    // Clamp satisfaction to valid range
    if (satisfaction < 0.0f) satisfaction = 0.0f;
    if (satisfaction > 1.0f) satisfaction = 1.0f;
    
    // Store satisfaction for current POI
    if (g_voyage.current_poi_index >= 0 && 
        g_voyage.current_poi_index < g_voyage.total_pois &&
        g_voyage.poi_satisfaction_scores != NULL) {
        g_voyage.poi_satisfaction_scores[g_voyage.current_poi_index] = satisfaction;
    }
}

// =============================================================================
// Metrics & Queries
// =============================================================================

float voyage_get_average_satisfaction(void) {
    if (g_voyage.total_pois <= 0 || g_voyage.poi_satisfaction_scores == NULL) {
        return 0.0f;
    }
    
    float total = 0.0f;
    int count = 0;
    
    // Sum up all recorded satisfactions
    for (int i = 0; i < g_voyage.total_pois; i++) {
        total += g_voyage.poi_satisfaction_scores[i];
        count++;
    }
    
    return (count > 0) ? (total / count) : 0.0f;
}

float voyage_get_duration(void) {
    return g_voyage.voyage_duration;
}

const VoyageProgress* voyage_get_progress(void) {
    return &g_voyage;
}

// =============================================================================
// State Queries
// =============================================================================

bool voyage_is_active(void) {
    return g_voyage.voyage_active;
}

bool voyage_is_complete(void) {
    return g_voyage.state == VOYAGE_STATE_FINISH;
}
