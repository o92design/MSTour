#ifndef GAME_CONSTANTS_H
#define GAME_CONSTANTS_H

// =============================================================================
// FALLBACK WINDOW CONSTANTS (use config.ini values when available)
// These are only used if config.ini cannot be loaded
// =============================================================================

#define FALLBACK_WINDOW_WIDTH   1280
#define FALLBACK_WINDOW_HEIGHT  720
#define FALLBACK_TARGET_FPS     60
#define FALLBACK_WINDOW_TITLE   "MS Tour - Archipelago Shipping"

// =============================================================================
// SHIP RENDERING CONSTANTS
// =============================================================================

// Ship dimensions for rendering
#define SHIP_LENGTH            80.0f
#define SHIP_WIDTH             40.0f
#define SHIP_CENTER_RADIUS     3.0f

// Ship rendering angles (bow offset from heading)
#define SHIP_BOW_LENGTH_MULT   0.6f
#define SHIP_STERN_WIDTH_MULT  0.6f
#define SHIP_STERN_ANGLE       150.0f

// =============================================================================
// UI LAYOUT CONSTANTS (sizes only - positions use engine_ui.h)
// =============================================================================

// Gauge sizes
#define UI_GAUGE_RADIUS        50.0f
#define UI_GAUGE_Y_MARGIN      120.0f  // Margin from bottom of screen
#define UI_COMPASS_X_MARGIN    100.0f  // Margin from left edge
#define UI_SPEED_X_MARGIN      100.0f  // Margin from right edge

// Telegraph UI dimensions
#define UI_TELEGRAPH_WIDTH     250
#define UI_TELEGRAPH_HEIGHT    80
#define UI_TELEGRAPH_Y_MARGIN  20

// Debug panel dimensions
#define UI_DEBUG_PANEL_WIDTH   260
#define UI_DEBUG_PANEL_HEIGHT  400
#define UI_DEBUG_PANEL_MARGIN  20      // Margin from right edge and top

// =============================================================================
// WATER/ENVIRONMENT COLORS
// =============================================================================

#define WATER_COLOR_R          65
#define WATER_COLOR_G          105
#define WATER_COLOR_B          225
#define WATER_COLOR_A          255

// =============================================================================
// PHYSICS CONSTANTS
// =============================================================================

// Delta time limits
#define PHYSICS_MAX_DELTA_TIME 0.1f

// Speed thresholds
#define PHYSICS_STOP_THRESHOLD 0.5f
#define PHYSICS_DRIFT_THRESHOLD 0.1f

#endif // GAME_CONSTANTS_H
