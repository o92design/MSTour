#ifndef GAME_UPDATE_H
#define GAME_UPDATE_H

#include "game_state.h"

// =============================================================================
// Game Update
// Handles all game logic updates per frame
// =============================================================================

// Update all game systems for one frame
void game_update(GameState* state, float delta_time);

// Update input handling
void game_update_input(GameState* state);

// Update ship physics and controls
void game_update_ship(GameState* state, float delta_time);

// Update audio systems
void game_update_audio(GameState* state);

// Update camera following ship
void game_update_camera(GameState* state, float delta_time);

// Handle debug key presses
void game_update_debug(GameState* state);

// Update voyage progression (check completion, transitions)
void game_update_voyage(GameState* state, float delta_time);

#endif // GAME_UPDATE_H
