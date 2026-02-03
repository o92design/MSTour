#ifndef GAME_RENDER_H
#define GAME_RENDER_H

#include "game_state.h"

// =============================================================================
// Game Render
// Handles all game rendering per frame
// =============================================================================

// Render all game visuals
void game_render(const GameState* state);

// Render world elements (water, islands, etc.)
void game_render_world(const GameState* state);

// Render all ships
void game_render_ships(const GameState* state);

// Render UI elements
void game_render_ui(const GameState* state);

// Render debug overlays
void game_render_debug(const GameState* state);

#endif // GAME_RENDER_H
