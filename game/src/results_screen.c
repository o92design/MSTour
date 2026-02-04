#include "results_screen.h"
#include "engine_ui.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// =============================================================================
// Global State
// =============================================================================

static ResultsScreenState g_results = {0};

// =============================================================================
// Constants
// =============================================================================

#define BUTTON_WIDTH 200.0f
#define BUTTON_HEIGHT 50.0f
#define BUTTON_SPACING 20.0f

// =============================================================================
// Results Screen Lifecycle
// =============================================================================

void results_screen_init(void) {
    memset(&g_results, 0, sizeof(ResultsScreenState));
    g_results.visible = false;
    g_results.poi_scores = NULL;
    g_results.selected_button = 0;
}

void results_screen_shutdown(void) {
    if (g_results.poi_scores) {
        free(g_results.poi_scores);
        g_results.poi_scores = NULL;
    }
    memset(&g_results, 0, sizeof(ResultsScreenState));
}

// =============================================================================
// Display Control
// =============================================================================

void results_screen_show(const char* route_name, const float* poi_scores, int num_pois, float duration) {
    // Copy route name
    strncpy(g_results.route_name, route_name, sizeof(g_results.route_name) - 1);
    g_results.route_name[sizeof(g_results.route_name) - 1] = '\0';
    
    // Allocate and copy POI scores
    g_results.num_pois = num_pois;
    if (g_results.poi_scores) {
        free(g_results.poi_scores);
    }
    
    if (num_pois > 0 && poi_scores != NULL) {
        g_results.poi_scores = (float*)malloc(sizeof(float) * num_pois);
        if (g_results.poi_scores) {
            memcpy(g_results.poi_scores, poi_scores, sizeof(float) * num_pois);
        }
        
        // Calculate average satisfaction
        float total = 0.0f;
        for (int i = 0; i < num_pois; i++) {
            total += poi_scores[i];
        }
        g_results.average_satisfaction = total / num_pois;
    } else {
        g_results.poi_scores = NULL;
        g_results.average_satisfaction = 0.0f;
    }
    
    // Calculate grade and star rating
    g_results.grade = calculate_grade(g_results.average_satisfaction);
    g_results.star_rating = get_star_rating(g_results.average_satisfaction);
    
    // Store and format duration
    g_results.duration = duration;
    format_duration(duration, g_results.duration_str);
    
    // Reset button states
    g_results.retry_hovered = false;
    g_results.menu_hovered = false;
    g_results.selected_button = 0;
    
    // Show the screen
    g_results.visible = true;
}

void results_screen_hide(void) {
    g_results.visible = false;
}

bool results_screen_is_visible(void) {
    return g_results.visible;
}

// =============================================================================
// Update & Render
// =============================================================================

void results_screen_render(void) {
    if (!g_results.visible) return;
    
    int screen_w = GetScreenWidth();
    int screen_h = GetScreenHeight();
    
    // Semi-transparent background overlay
    DrawRectangle(0, 0, screen_w, screen_h, (Color){0, 0, 0, 180});
    
    // Main panel
    Rectangle panel = {
        screen_w / 2.0f - 300,
        screen_h / 2.0f - 250,
        600,
        500
    };
    DrawRectangleRec(panel, (Color){30, 30, 40, 255});
    DrawRectangleLinesEx(panel, 2, (Color){100, 100, 120, 255});
    
    float y_offset = panel.y + 30;
    
    // Title
    const char* title = "Voyage Complete";
    DrawText(title, screen_w / 2 - MeasureText(title, 30) / 2, y_offset, 30, WHITE);
    y_offset += 50;
    
    // Route name
    DrawText(g_results.route_name, screen_w / 2 - MeasureText(g_results.route_name, 20) / 2, y_offset, 20, GRAY);
    y_offset += 40;
    
    // Grade display
    const char* grade_names[] = {"POOR", "FAIR", "GOOD", "EXCELLENT", "PERFECT"};
    Color grade_colors[] = {
        (Color){200, 50, 50, 255},    // POOR - red
        (Color){200, 150, 50, 255},   // FAIR - orange
        (Color){100, 200, 100, 255},  // GOOD - green
        (Color){50, 150, 255, 255},   // EXCELLENT - blue
        (Color){255, 215, 0, 255}     // PERFECT - gold
    };
    
    const char* grade_str = grade_names[g_results.grade];
    Color grade_color = grade_colors[g_results.grade];
    DrawText(grade_str, screen_w / 2 - MeasureText(grade_str, 40) / 2, y_offset, 40, grade_color);
    y_offset += 50;
    
    // Star rating
    for (int i = 0; i < 5; i++) {
        Color star_color = (i < g_results.star_rating) ? GOLD : DARKGRAY;
        Vector2 pos = {screen_w / 2.0f - 60.0f + i * 30.0f, y_offset};
        DrawText("*", pos.x, pos.y, 30, star_color);
    }
    y_offset += 50;
    
    // Average satisfaction percentage
    char sat_text[32];
    snprintf(sat_text, sizeof(sat_text), "%.0f%% Satisfaction", g_results.average_satisfaction * 100.0f);
    DrawText(sat_text, screen_w / 2 - MeasureText(sat_text, 20) / 2, y_offset, 20, LIGHTGRAY);
    y_offset += 40;
    
    // Duration
    char dur_text[64];
    snprintf(dur_text, sizeof(dur_text), "Duration: %s", g_results.duration_str);
    DrawText(dur_text, screen_w / 2 - MeasureText(dur_text, 18) / 2, y_offset, 18, LIGHTGRAY);
    y_offset += 50;
    
    // POI individual scores (if space permits)
    if (g_results.num_pois > 0 && g_results.num_pois <= 4) {
        for (int i = 0; i < g_results.num_pois; i++) {
            char poi_text[64];
            snprintf(poi_text, sizeof(poi_text), "POI %d: %.0f%%", i + 1, g_results.poi_scores[i] * 100.0f);
            DrawText(poi_text, screen_w / 2 - 100, y_offset, 16, GRAY);
            y_offset += 25;
        }
        y_offset += 15;
    }
    
    // Buttons
    float button_y = panel.y + panel.height - 80;
    float button_x_left = screen_w / 2.0f - BUTTON_WIDTH - BUTTON_SPACING / 2;
    float button_x_right = screen_w / 2.0f + BUTTON_SPACING / 2;
    
    // Retry button
    Rectangle retry_btn = {button_x_left, button_y, BUTTON_WIDTH, BUTTON_HEIGHT};
    Color retry_color = g_results.retry_hovered || g_results.selected_button == 0 ? 
                       (Color){70, 130, 70, 255} : (Color){50, 90, 50, 255};
    DrawRectangleRec(retry_btn, retry_color);
    DrawRectangleLinesEx(retry_btn, 2, (Color){100, 200, 100, 255});
    const char* retry_text = "Retry";
    DrawText(retry_text, retry_btn.x + BUTTON_WIDTH / 2 - MeasureText(retry_text, 20) / 2, 
             retry_btn.y + 15, 20, WHITE);
    
    // Menu button
    Rectangle menu_btn = {button_x_right, button_y, BUTTON_WIDTH, BUTTON_HEIGHT};
    Color menu_color = g_results.menu_hovered || g_results.selected_button == 1 ? 
                      (Color){130, 70, 70, 255} : (Color){90, 50, 50, 255};
    DrawRectangleRec(menu_btn, menu_color);
    DrawRectangleLinesEx(menu_btn, 2, (Color){200, 100, 100, 255});
    const char* menu_text = "Return to Menu";
    DrawText(menu_text, menu_btn.x + BUTTON_WIDTH / 2 - MeasureText(menu_text, 20) / 2, 
             menu_btn.y + 15, 20, WHITE);
}

bool results_screen_handle_input(bool* out_retry, bool* out_menu) {
    if (!g_results.visible) return false;
    
    *out_retry = false;
    *out_menu = false;
    
    Vector2 mouse_pos = GetMousePosition();
    int screen_w = GetScreenWidth();
    
    // Calculate button positions
    float button_y = GetScreenHeight() / 2.0f - 250 + 500 - 80;
    float button_x_left = screen_w / 2.0f - BUTTON_WIDTH - BUTTON_SPACING / 2;
    float button_x_right = screen_w / 2.0f + BUTTON_SPACING / 2;
    
    Rectangle retry_btn = {button_x_left, button_y, BUTTON_WIDTH, BUTTON_HEIGHT};
    Rectangle menu_btn = {button_x_right, button_y, BUTTON_WIDTH, BUTTON_HEIGHT};
    
    // Mouse hover detection
    g_results.retry_hovered = CheckCollisionPointRec(mouse_pos, retry_btn);
    g_results.menu_hovered = CheckCollisionPointRec(mouse_pos, menu_btn);
    
    // Mouse click handling
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (g_results.retry_hovered) {
            *out_retry = true;
            return true;
        }
        if (g_results.menu_hovered) {
            *out_menu = true;
            return true;
        }
    }
    
    // Keyboard navigation
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT)) {
        g_results.selected_button = 1 - g_results.selected_button;  // Toggle 0/1
    }
    
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
        if (g_results.selected_button == 0) {
            *out_retry = true;
        } else {
            *out_menu = true;
        }
        return true;
    }
    
    // ESC to close (same as menu)
    if (IsKeyPressed(KEY_ESCAPE)) {
        *out_menu = true;
        return true;
    }
    
    return false;
}

// =============================================================================
// Utility Functions
// =============================================================================

VoyageGrade calculate_grade(float satisfaction) {
    if (satisfaction >= 0.95f) return GRADE_PERFECT;
    if (satisfaction >= 0.85f) return GRADE_EXCELLENT;
    if (satisfaction >= 0.70f) return GRADE_GOOD;
    if (satisfaction >= 0.50f) return GRADE_FAIR;
    return GRADE_POOR;
}

int get_star_rating(float satisfaction) {
    if (satisfaction >= 0.95f) return 5;
    if (satisfaction >= 0.85f) return 4;
    if (satisfaction >= 0.70f) return 3;
    if (satisfaction >= 0.50f) return 2;
    if (satisfaction >= 0.25f) return 1;
    return 0;
}

void format_duration(float seconds, char* buffer) {
    int total_seconds = (int)seconds;
    int minutes = total_seconds / 60;
    int secs = total_seconds % 60;
    snprintf(buffer, 32, "%dm %02ds", minutes, secs);
}

const ResultsScreenState* results_screen_get_state(void) {
    return &g_results;
}
