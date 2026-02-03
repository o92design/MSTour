#include "game_render.h"
#include "ship_render.h"
#include "ship_ui.h"
#include "game_constants.h"
#include "engine_core.h"
#include "engine_renderer.h"
#include "engine_ui.h"
#include <raylib.h>
#include <stdio.h>

void game_render_world(const GameState* state) {
    if (!state) return;
    
    // Clear to sky color
    renderer_clear(SKYBLUE);
    
    // Draw ocean/water effect (fill entire window)
    Color water_color = {WATER_COLOR_R, WATER_COLOR_G, WATER_COLOR_B, WATER_COLOR_A};
    DrawRectangle(0, 0, engine_get_window_width(), engine_get_window_height(), water_color);
}

void game_render_ships(const GameState* state) {
    if (!state) return;
    
    // Get player ship style
    ShipVisualStyle player_style = ship_render_get_player_style();
    
    // Draw wake first (behind ship)
    ship_render_draw_wake(&state->player_ship, 1.0f);
    
    // Draw player ship
    ship_render_draw(&state->player_ship, &player_style);
}

void game_render_ui(const GameState* state) {
    if (!state) return;
    
    // Only render UI if help and debug panel are not shown
    if (!state->debug.show_help && !state->debug.show_debug_panel) {
        // Title and controls hint (top-left with margin)
        int margin = 20;
        renderer_draw_text("MS Tour - Ship Control Prototype", margin, margin, 30, WHITE);
        renderer_draw_text("Controls: W/S=Telegraph Orders | A/D=Turn | F3=Help", margin, margin + 40, 20, LIGHTGRAY);
        
        // Ship UI (gauges and indicators) - uses engine_ui internally
        ship_ui_render(&state->player_ship, &state->telegraph);
        
        // Frame info (bottom-left)
        char debug_text[256];
        snprintf(debug_text, sizeof(debug_text), "FPS: %.1f | Frame: %llu", 
                 engine_get_fps(),
                 (unsigned long long)engine_get_frame_count());
        renderer_draw_text(debug_text, margin, (int)ui_from_bottom(40), 18, YELLOW);
    }
}

void game_render_debug(const GameState* state) {
    if (!state) return;
    
    // Debug visualization (velocity vectors, turn radius, etc.)
    debug_tools_draw_visualization(&state->debug, &state->player_ship, &state->physics_config);
    
    // Debug panel (ship state values)
    debug_tools_draw_panel(&state->debug, &state->player_ship, &state->physics_config);
    
    // Help overlay (drawn last, on top of everything)
    debug_tools_draw_help(&state->debug);
}

void game_render(const GameState* state) {
    if (!state || !state->initialized) return;
    
    // Render in layers
    game_render_world(state);
    game_render_ships(state);
    game_render_ui(state);
    game_render_debug(state);
}
