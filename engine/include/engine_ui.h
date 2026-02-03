#ifndef ENGINE_UI_H
#define ENGINE_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "engine_core.h"

// =============================================================================
// UI Anchor and Positioning Utilities
// 
// Provides relative positioning for UI elements that adapts to window size.
// All positions are calculated at runtime based on current window dimensions.
// =============================================================================

// =============================================================================
// Anchor Types
// =============================================================================

typedef enum UIAnchor {
    UI_ANCHOR_TOP_LEFT,
    UI_ANCHOR_TOP_CENTER,
    UI_ANCHOR_TOP_RIGHT,
    UI_ANCHOR_CENTER_LEFT,
    UI_ANCHOR_CENTER,
    UI_ANCHOR_CENTER_RIGHT,
    UI_ANCHOR_BOTTOM_LEFT,
    UI_ANCHOR_BOTTOM_CENTER,
    UI_ANCHOR_BOTTOM_RIGHT
} UIAnchor;

// =============================================================================
// Position Result
// =============================================================================

typedef struct UIPosition {
    float x;
    float y;
} UIPosition;

// =============================================================================
// Anchored Positioning
// 
// Returns absolute position based on anchor point and offset.
// Positive offsets move right/down from anchor, negative move left/up.
// =============================================================================

// Get position anchored to window corner/edge with pixel offset
static inline UIPosition ui_get_anchored_pos(UIAnchor anchor, float offset_x, float offset_y) {
    UIPosition pos = {0, 0};
    int w = engine_get_window_width();
    int h = engine_get_window_height();
    
    // Horizontal positioning
    switch (anchor) {
        case UI_ANCHOR_TOP_LEFT:
        case UI_ANCHOR_CENTER_LEFT:
        case UI_ANCHOR_BOTTOM_LEFT:
            pos.x = offset_x;
            break;
        case UI_ANCHOR_TOP_CENTER:
        case UI_ANCHOR_CENTER:
        case UI_ANCHOR_BOTTOM_CENTER:
            pos.x = (float)w / 2.0f + offset_x;
            break;
        case UI_ANCHOR_TOP_RIGHT:
        case UI_ANCHOR_CENTER_RIGHT:
        case UI_ANCHOR_BOTTOM_RIGHT:
            pos.x = (float)w + offset_x;  // offset should be negative
            break;
    }
    
    // Vertical positioning
    switch (anchor) {
        case UI_ANCHOR_TOP_LEFT:
        case UI_ANCHOR_TOP_CENTER:
        case UI_ANCHOR_TOP_RIGHT:
            pos.y = offset_y;
            break;
        case UI_ANCHOR_CENTER_LEFT:
        case UI_ANCHOR_CENTER:
        case UI_ANCHOR_CENTER_RIGHT:
            pos.y = (float)h / 2.0f + offset_y;
            break;
        case UI_ANCHOR_BOTTOM_LEFT:
        case UI_ANCHOR_BOTTOM_CENTER:
        case UI_ANCHOR_BOTTOM_RIGHT:
            pos.y = (float)h + offset_y;  // offset should be negative
            break;
    }
    
    return pos;
}

// =============================================================================
// Percentage-Based Positioning
// 
// Returns position as percentage of window size (0.0 to 1.0).
// =============================================================================

// Get position as percentage of window size
static inline UIPosition ui_get_relative_pos(float percent_x, float percent_y) {
    UIPosition pos;
    pos.x = (float)engine_get_window_width() * percent_x;
    pos.y = (float)engine_get_window_height() * percent_y;
    return pos;
}

// Get X position as percentage of window width
static inline float ui_get_relative_x(float percent) {
    return (float)engine_get_window_width() * percent;
}

// Get Y position as percentage of window height
static inline float ui_get_relative_y(float percent) {
    return (float)engine_get_window_height() * percent;
}

// =============================================================================
// Convenience Functions
// =============================================================================

// Get position for element that should stay a fixed distance from an edge
static inline float ui_from_right(float offset) {
    return (float)engine_get_window_width() - offset;
}

static inline float ui_from_bottom(float offset) {
    return (float)engine_get_window_height() - offset;
}

static inline float ui_center_x(void) {
    return (float)engine_get_window_width() / 2.0f;
}

static inline float ui_center_y(void) {
    return (float)engine_get_window_height() / 2.0f;
}

// Get window dimensions (convenience wrappers)
static inline float ui_get_width(void) {
    return (float)engine_get_window_width();
}

static inline float ui_get_height(void) {
    return (float)engine_get_window_height();
}

// =============================================================================
// Layout Helpers
// =============================================================================

// Calculate position to center an element of given size
static inline UIPosition ui_center_element(float element_width, float element_height) {
    UIPosition pos;
    pos.x = ((float)engine_get_window_width() - element_width) / 2.0f;
    pos.y = ((float)engine_get_window_height() - element_height) / 2.0f;
    return pos;
}

// Calculate position to place element at bottom-center with margin
static inline UIPosition ui_bottom_center(float element_width, float margin_bottom) {
    UIPosition pos;
    pos.x = ((float)engine_get_window_width() - element_width) / 2.0f;
    pos.y = (float)engine_get_window_height() - margin_bottom;
    return pos;
}

#ifdef __cplusplus
}
#endif

#endif // ENGINE_UI_H
