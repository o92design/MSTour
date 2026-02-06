#include "game_state.h"
#include "game_poi_loader.h"
#include "game_textures.h"
#include "voyage_manager.h"
#include "results_screen.h"
#include "config.h"
#include "game_constants.h"
#include "engine_core.h"
#include <string.h>
#include <stdio.h>

// Global game state instance
static GameState g_game_state = {0};

bool game_state_init(GameState* state, const ConfigFile* config) {
    if (!state) return false;
    
    memset(state, 0, sizeof(GameState));
    
    // Get window dimensions for initial positioning
    int window_width = engine_get_window_width();
    int window_height = engine_get_window_height();
    float center_x = window_width / 2.0f;
    float center_y = window_height / 2.0f;
    
    // Initialize ECS world (engine + game layers)
    game_ecs_init(&state->game_ecs, &state->ecs_world);
    state->use_ecs = true;  // Default to ECS mode
    
    // Create player ship entity in ECS
    state->player_entity = game_create_player_ship(&state->game_ecs, center_x, center_y, 0.0f);
    if (state->player_entity == INVALID_ENTITY) {
        fprintf(stderr, "Failed to create player ship entity!\n");
        return false;
    }
    
    // Initialize legacy ship (for compatibility/fallback)
    ship_physics_init(&state->player_ship, center_x, center_y, 0.0f);
    
    // Initialize telegraph
    ship_telegraph_init(&state->telegraph);
    
    // Load physics config
    state->config_path = "config.ini";
    if (!config_load_ship_physics(state->config_path, &state->physics_config)) {
        state->physics_config = ship_physics_get_default_config();
        config_save_ship_physics(state->config_path, &state->physics_config);
    }
    
    // Apply physics config to ECS ship
    game_ecs_apply_physics_config(&state->game_ecs, state->player_entity, &state->physics_config);
    
    // Initialize camera
    state->camera_config = camera_get_default_config();
    camera_init_centered(&state->camera, center_x, center_y, window_width, window_height);
    
    // Get audio configuration from loaded config (or defaults)
    state->audio_config = config_get_audio(config);
    
    // Initialize audio
    if (!audio_init(&state->audio)) {
        printf("Warning: Audio initialization failed\n");
    }
    
    // Load sounds with volumes from config
    state->sounds.telegraph_bell = audio_load_sound(&state->audio, 
        "assets/sounds/telegraph_bell.mp3", "telegraph_bell");
    if (state->sounds.telegraph_bell != INVALID_SOUND_HANDLE) {
        audio_set_volume(&state->audio, state->sounds.telegraph_bell, 
                         state->audio_config.sfx_volume * state->audio_config.master_volume);
    }
    
    state->sounds.engine_loop = audio_load_sound(&state->audio,
        "assets/sounds/engine_loop.mp3", "engine_loop");
    if (state->sounds.engine_loop != INVALID_SOUND_HANDLE) {
        audio_set_volume(&state->audio, state->sounds.engine_loop, 
                         state->audio_config.engine_volume * state->audio_config.master_volume);
        audio_set_looping(&state->audio, state->sounds.engine_loop, true);
        audio_play(&state->audio, state->sounds.engine_loop);
    }
    
    state->sounds.water_ambient = audio_load_sound(&state->audio,
        "assets/sounds/water_ambient.mp3", "water_ambient");
    if (state->sounds.water_ambient != INVALID_SOUND_HANDLE) {
        audio_set_volume(&state->audio, state->sounds.water_ambient, 
                         state->audio_config.ambient_volume * state->audio_config.master_volume);
        audio_set_looping(&state->audio, state->sounds.water_ambient, true);
        audio_play(&state->audio, state->sounds.water_ambient);
    }
    
    // Initialize debug tools
    debug_tools_init(&state->debug);
    
    // Load game textures (sprites)
    if (!game_textures_init()) {
        printf("Warning: Failed to load game textures, using fallback rendering\n");
    }
    
    // Load POIs
    if (!game_ecs_load_pois(&state->game_ecs, NULL)) {
        printf("Warning: Using default POIs\n");
    }
    
    // Initialize voyage manager with POI count
    int poi_count = state->game_ecs.poi_world.poi_count;
    voyage_init(poi_count);
    
    // Initialize results screen
    results_screen_init();
    
    // Start voyage and tour satisfaction tracking
    voyage_start();
    satisfaction_tour_start(&state->game_ecs.tour);
    state->voyage_active = true;
    state->results_showing = false;
    
    printf("Voyage started with %d POIs - visit them all to complete the tour!\n", poi_count);
    
    state->initialized = true;
    state->paused = false;
    
    printf("Game state initialized (ECS mode: %s)\n", state->use_ecs ? "ON" : "OFF");
    return true;
}

void game_state_shutdown(GameState* state) {
    if (!state || !state->initialized) return;
    
    results_screen_shutdown();
    voyage_shutdown();
    game_ecs_shutdown(&state->game_ecs);
    audio_shutdown(&state->audio);
    game_textures_cleanup();
    
    state->initialized = false;
    printf("Game state shutdown\n");
}

void game_state_reset(GameState* state) {
    if (!state) return;
    
    // Get current window dimensions
    int window_width = engine_get_window_width();
    int window_height = engine_get_window_height();
    float center_x = window_width / 2.0f;
    float center_y = window_height / 2.0f;
    
    // Reset ECS ship
    if (state->player_entity != INVALID_ENTITY) {
        ecs_set_position(&state->ecs_world, state->player_entity, center_x, center_y);
        ecs_set_rotation(&state->ecs_world, state->player_entity, 0.0f);
        ecs_set_velocity(&state->ecs_world, state->player_entity, 0.0f, 0.0f);
        state->ecs_world.velocities.angular_vel[state->player_entity] = 0.0f;
        state->ecs_world.velocities.speed[state->player_entity] = 0.0f;
        
        // Reset ship controls in game-layer ship world
        state->game_ecs.ship_world.ships.throttle[state->player_entity] = 0.0f;
        state->game_ecs.ship_world.ships.target_throttle[state->player_entity] = 0.0f;
        state->game_ecs.ship_world.ships.rudder[state->player_entity] = 0.0f;
        state->game_ecs.ship_world.ships.target_rudder[state->player_entity] = 0.0f;
    }
    
    // Reset legacy ship
    ship_physics_init(&state->player_ship, center_x, center_y, 0.0f);
    
    // Reset telegraph
    ship_telegraph_init(&state->telegraph);
    
    // Reset camera
    camera_set_position(&state->camera, center_x, center_y);
    
    // Reset POI visited states and fog of war
    poi_ecs_reset_all_visited(&state->game_ecs.poi_world);
    fog_reset(&state->game_ecs.fog);
    
    // Restart voyage and satisfaction tracking (results screen should already be hidden by caller)
    voyage_start();
    satisfaction_tour_start(&state->game_ecs.tour);
    state->voyage_active = true;
    
    printf("Game state reset - voyage restarted, POIs cleared, fog reset\n");
}

GameState* game_get_state(void) {
    return &g_game_state;
}

void game_state_toggle_ecs(GameState* state) {
    if (!state) return;
    
    state->use_ecs = !state->use_ecs;
    
    // Sync state between systems when toggling
    if (state->use_ecs) {
        // Copy legacy state to ECS
        game_ecs_from_ship_state(&state->game_ecs, state->player_entity, &state->player_ship);
    } else {
        // Copy ECS state to legacy
        game_ecs_to_ship_state(&state->game_ecs, state->player_entity, &state->player_ship);
    }
    
    printf("ECS mode: %s\n", state->use_ecs ? "ON" : "OFF");
}
