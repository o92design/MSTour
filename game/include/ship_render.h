#ifndef SHIP_RENDER_H
#define SHIP_RENDER_H

#include "ship_physics.h"
#include <raylib.h>

// =============================================================================
// Ship Rendering
// Handles drawing ships to the screen
// =============================================================================

// Ship visual style
typedef struct ShipVisualStyle {
    Color hull_color;
    Color outline_color;
    Color center_color;
    float length;
    float width;
} ShipVisualStyle;

// Get default ship visual style
ShipVisualStyle ship_render_get_default_style(void);

// Get style for player ship
ShipVisualStyle ship_render_get_player_style(void);

// Get style for AI ship
ShipVisualStyle ship_render_get_ai_style(void);

// Draw a single ship
void ship_render_draw(const ShipState* ship, const ShipVisualStyle* style);

// Draw ship with default style
void ship_render_draw_default(const ShipState* ship);

// Draw ship at specific position (for preview/UI)
void ship_render_draw_at(float x, float y, float heading, const ShipVisualStyle* style);

// Draw ship wake effect (trail behind ship)
void ship_render_draw_wake(const ShipState* ship, float intensity);

#endif // SHIP_RENDER_H
