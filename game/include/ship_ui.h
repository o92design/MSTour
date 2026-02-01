#ifndef SHIP_UI_H
#define SHIP_UI_H

#include "ship_physics.h"
#include "ship_telegraph.h"
#include <raylib.h>

// Initialize UI system
void ship_ui_init(void);

// Cleanup UI system
void ship_ui_cleanup(void);

// Render ship control UI (speed, heading, telegraph)
void ship_ui_render(const ShipState* ship, const ShipTelegraph* telegraph);

// Render speed gauge
void ship_ui_render_speed_gauge(float speed, float max_speed, Vector2 position, float radius);

// Render heading compass
void ship_ui_render_compass(float heading, Vector2 position, float radius);

// Render telegraph order indicator
void ship_ui_render_telegraph(const ShipTelegraph* telegraph, Vector2 position);

#endif // SHIP_UI_H
