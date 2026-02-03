#ifndef DEBUG_TOOLS_H
#define DEBUG_TOOLS_H

#include "ship_physics.h"
#include <stdbool.h>

// Debug state
typedef struct DebugState {
    bool show_visualization;  // F1 toggle
    bool show_debug_panel;    // F2 toggle
    bool show_help;           // F3 toggle
} DebugState;

// Initialize debug tools
void debug_tools_init(DebugState* state);

// Update debug tools (handle input)
void debug_tools_update(DebugState* state);

// Draw debug visualization (velocity vectors, turn radius, etc.)
void debug_tools_draw_visualization(const DebugState* state, const ShipState* ship, const ShipPhysicsConfig* config);

// Draw debug UI panel (ship state values)
void debug_tools_draw_panel(const DebugState* state, const ShipState* ship, const ShipPhysicsConfig* config);

// Draw help overlay
void debug_tools_draw_help(const DebugState* state);

// Debug commands
void debug_tools_teleport_ship(ShipState* ship, float x, float y); // F4
void debug_tools_reset_ship(ShipState* ship);                      // F5

#endif // DEBUG_TOOLS_H
