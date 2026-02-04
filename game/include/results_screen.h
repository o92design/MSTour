#ifndef RESULTS_SCREEN_H
#define RESULTS_SCREEN_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// Results Screen
// 
// Post-voyage feedback UI displaying satisfaction scores, grades, and stats.
// =============================================================================

// =============================================================================
// Voyage Grade Enum
// =============================================================================

typedef enum VoyageGrade {
    GRADE_POOR = 0,       // < 50% satisfaction
    GRADE_FAIR = 1,       // 50-69% satisfaction
    GRADE_GOOD = 2,       // 70-84% satisfaction
    GRADE_EXCELLENT = 3,  // 85-94% satisfaction
    GRADE_PERFECT = 4     // >= 95% satisfaction
} VoyageGrade;

// =============================================================================
// Results Screen State
// =============================================================================

typedef struct ResultsScreenState {
    bool visible;
    
    // Route info
    char route_name[64];
    
    // POI scores
    int num_pois;
    float* poi_scores;  // Dynamic array
    
    // Aggregate stats
    float average_satisfaction;
    VoyageGrade grade;
    int star_rating;  // 0-5 stars
    float duration;   // seconds
    char duration_str[32];  // formatted "Xm Ys"
    
    // Button states
    bool retry_hovered;
    bool menu_hovered;
    int selected_button;  // 0 = retry, 1 = menu (for keyboard nav)
} ResultsScreenState;

// =============================================================================
// Results Screen Lifecycle
// =============================================================================

// Initialize results screen
void results_screen_init(void);

// Shutdown and cleanup resources
void results_screen_shutdown(void);

// =============================================================================
// Display Control
// =============================================================================

// Show results screen with voyage data
void results_screen_show(const char* route_name, const float* poi_scores, int num_pois, float duration);

// Hide results screen
void results_screen_hide(void);

// Check if results screen is visible
bool results_screen_is_visible(void);

// =============================================================================
// Update & Render
// =============================================================================

// Render results screen
void results_screen_render(void);

// Handle input (returns true if action taken)
// out_retry: set to true if retry button clicked
// out_menu: set to true if menu button clicked
bool results_screen_handle_input(bool* out_retry, bool* out_menu);

// =============================================================================
// Utility Functions
// =============================================================================

// Calculate grade from satisfaction (0.0 - 1.0)
VoyageGrade calculate_grade(float satisfaction);

// Get star rating from satisfaction (0-5 stars)
int get_star_rating(float satisfaction);

// Format duration in seconds to "Xm Ys" string
void format_duration(float seconds, char* buffer);

// Get read-only access to results state (for testing)
const ResultsScreenState* results_screen_get_state(void);

#ifdef __cplusplus
}
#endif

#endif // RESULTS_SCREEN_H
