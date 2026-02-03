#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "ship_physics.h"
#include "ship_telegraph.h"
#include "debug_tools.h"
#include "engine_audio.h"
#include "engine_camera.h"
#include "engine_config.h"
#include "engine_ecs.h"
#include "game_ecs.h"
#include <stdbool.h>

// =============================================================================
// Game State
// Central game state structure containing all game data
// =============================================================================

// Sound handles for the game
typedef struct GameSounds {
    SoundHandle telegraph_bell;
    SoundHandle engine_loop;
    SoundHandle water_ambient;
} GameSounds;

// Main game state
typedef struct GameState {
    // ECS World (new data-oriented approach)
    ECSWorld ecs_world;         // Engine-level ECS (transforms, velocities)
    GameEcsState game_ecs;      // Game-level ECS (ships, AI)
    Entity player_entity;       // Player ship entity in ECS
    bool use_ecs;               // Toggle between legacy and ECS physics
    
    // Legacy player ship (kept for compatibility during migration)
    ShipState player_ship;
    ShipTelegraph telegraph;
    ShipPhysicsConfig physics_config;
    
    // Camera
    CameraState camera;
    CameraConfig camera_config;
    
    // Audio
    AudioManager audio;
    AudioConfig audio_config;
    GameSounds sounds;
    
    // Debug
    DebugState debug;
    
    // Config
    const char* config_path;
    
    // State flags
    bool initialized;
    bool paused;
} GameState;

// Initialize game state (pass NULL for config to use defaults)
bool game_state_init(GameState* state, const ConfigFile* config);

// Shutdown game state (cleanup resources)
void game_state_shutdown(GameState* state);

// Reset game to initial conditions
void game_state_reset(GameState* state);

// Get global game state instance
GameState* game_get_state(void);

// Toggle between legacy physics and ECS physics
void game_state_toggle_ecs(GameState* state);

#endif // GAME_STATE_H
