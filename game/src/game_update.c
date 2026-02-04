#include "game_update.h"
#include "game_ecs.h"
#include "voyage_manager.h"
#include "results_screen.h"
#include "input_actions.h"
#include "config.h"
#include "engine_math.h"
#include "engine_ecs.h"
#include "engine_core.h"
#include "engine_ui.h"
#include <raylib.h>
#include <stdio.h>

void game_update_input(GameState* state) {
    if (!state) return;
    
    input_actions_update();
    
    // Telegraph controls (key presses, not holds)
    if (input_action_pressed(SHIP_ACTION_THROTTLE_UP)) {
        ship_telegraph_ring_up(&state->telegraph);
        if (state->sounds.telegraph_bell != INVALID_SOUND_HANDLE) {
            audio_play(&state->audio, state->sounds.telegraph_bell);
        }
    }
    if (input_action_pressed(SHIP_ACTION_THROTTLE_DOWN)) {
        ship_telegraph_ring_down(&state->telegraph);
        if (state->sounds.telegraph_bell != INVALID_SOUND_HANDLE) {
            audio_play(&state->audio, state->sounds.telegraph_bell);
        }
    }
}

void game_update_ship(GameState* state, float delta_time) {
    if (!state) return;
    
    // Update telegraph timer
    ship_telegraph_update(&state->telegraph, delta_time);
    
    // Get throttle from telegraph and steering from input
    float throttle_input = ship_telegraph_get_throttle(&state->telegraph);
    float steering_input = input_action_get_steering_axis();
    
    if (state->use_ecs) {
        // ECS physics update
        game_ecs_set_throttle(&state->game_ecs, state->player_entity, throttle_input);
        game_ecs_set_steering(&state->game_ecs, state->player_entity, steering_input);
        
        // Run all game ECS systems (ship physics, AI, movement)
        game_ecs_update(&state->game_ecs, delta_time);
        
        // Sync ECS state to legacy struct (for rendering/UI compatibility)
        game_ecs_to_ship_state(&state->game_ecs, state->player_entity, &state->player_ship);
    } else {
        // Legacy physics update
        ship_physics_process_actions(&state->player_ship, throttle_input, steering_input);
        ship_physics_update(&state->player_ship, &state->physics_config, delta_time);
    }
}

void game_update_audio(GameState* state) {
    if (!state) return;
    
    // Update looping sounds
    audio_update(&state->audio);
    
    // Update engine sound pitch based on speed
    if (state->sounds.engine_loop != INVALID_SOUND_HANDLE) {
        float speed = state->use_ecs 
            ? game_ecs_get_ship_speed(&state->game_ecs, state->player_entity)
            : state->player_ship.speed;
        float speed_percent = speed / state->physics_config.max_speed;
        float pitch = 0.8f + (speed_percent * 0.6f); // Range: 0.8 to 1.4
        audio_set_pitch(&state->audio, state->sounds.engine_loop, pitch);
    }
}

void game_update_camera(GameState* state, float delta_time) {
    if (!state) return;
    
    // Get ship position (from ECS or legacy)
    float ship_x, ship_y;
    if (state->use_ecs) {
        game_ecs_get_ship_position(&state->game_ecs, state->player_entity, &ship_x, &ship_y);
    } else {
        ship_x = state->player_ship.pos_x;
        ship_y = state->player_ship.pos_y;
    }
    
    // Set camera target to follow ship
    camera_set_target(&state->camera, ship_x, ship_y);
    
    // Handle zoom input
    float scroll = GetMouseWheelMove();
    if (scroll != 0.0f) {
        camera_zoom(&state->camera, &state->camera_config, scroll * 0.1f);
    }
    
    // Keyboard zoom controls
    if (IsKeyDown(KEY_EQUAL) || IsKeyDown(KEY_KP_ADD)) {
        camera_zoom(&state->camera, &state->camera_config, 1.0f * delta_time);
    }
    if (IsKeyDown(KEY_MINUS) || IsKeyDown(KEY_KP_SUBTRACT)) {
        camera_zoom(&state->camera, &state->camera_config, -1.0f * delta_time);
    }
    
    // Reset zoom with 0
    if (IsKeyPressed(KEY_ZERO) || IsKeyPressed(KEY_KP_0)) {
        camera_set_zoom(&state->camera, &state->camera_config, 1.0f);
    }
    
    camera_update(&state->camera, &state->camera_config, delta_time);
}

void game_update_debug(GameState* state) {
    if (!state) return;
    
    // Update debug tools (handle F1-F3 keys)
    debug_tools_update(&state->debug);
    
    // Debug commands
    if (IsKeyPressed(KEY_F4)) {
        // Teleport ship to center
        float center_x = ui_center_x();
        float center_y = ui_center_y();
        
        if (state->use_ecs) {
            ecs_set_position(&state->ecs_world, state->player_entity, center_x, center_y);
            game_ecs_to_ship_state(&state->game_ecs, state->player_entity, &state->player_ship);
        } else {
            debug_tools_teleport_ship(&state->player_ship, center_x, center_y);
        }
        printf("Debug: Teleported ship to (%.1f, %.1f)\n", center_x, center_y);
    }
    if (IsKeyPressed(KEY_F5)) {
        // Reset ship state
        game_state_reset(state);
    }
    if (IsKeyPressed(KEY_F6)) {
        // Hot-reload config
        config_reload_ship_physics(state->config_path, &state->physics_config);
        // Apply new config to ECS ship
        game_ecs_apply_physics_config(&state->game_ecs, state->player_entity, &state->physics_config);
    }
    if (IsKeyPressed(KEY_F7)) {
        // Toggle ECS mode
        game_state_toggle_ecs(state);
    }
    if (IsKeyPressed(KEY_F8)) {
        // Debug: Complete voyage early (for testing results screen)
        if (state->voyage_active) {
            printf("Debug: Force completing voyage...\n");
            voyage_set_state(VOYAGE_STATE_FINISH);
        }
    }
    if (IsKeyPressed(KEY_F9)) {
        // Debug: Toggle results screen visibility
        if (state->results_showing) {
            printf("Debug: Hiding results screen\n");
            state->results_showing = false;
            results_screen_hide();
        } else {
            printf("Debug: Showing test results screen\n");
            // Create test data for results screen
            float test_scores[] = {85.0f, 92.0f};
            results_screen_show("Debug Test Route", test_scores, 2, 123.5f);
            state->results_showing = true;
        }
    }
}

void game_update(GameState* state, float delta_time) {
    if (!state || !state->initialized) return;
    if (state->paused) return;
    
    // Handle results screen if showing
    if (state->results_showing) {
        bool retry_clicked = false;
        bool menu_clicked = false;
        bool action_taken = results_screen_handle_input(&retry_clicked, &menu_clicked);
        
        if (action_taken) {
            if (retry_clicked) {
                // Retry - restart the voyage
                printf("Retrying voyage...\n");
                game_state_reset(state);
            } else if (menu_clicked) {
                // Menu - for now just hide results
                printf("Returning to menu...\n");
                state->results_showing = false;
                results_screen_hide();
                // Could transition to main menu here in the future
            }
        }
        return; // Don't update game while results showing
    }
    
    game_update_input(state);
    game_update_debug(state);
    game_update_ship(state, delta_time);
    game_update_voyage(state, delta_time);
    game_update_audio(state);
    game_update_camera(state, delta_time);
}

void game_update_voyage(GameState* state, float delta_time) {
    if (!state || !state->voyage_active) return;
    
    // Update voyage duration
    voyage_update(delta_time);
    
    // Check if voyage is complete
    if (voyage_is_complete()) {
        printf("Voyage complete! Showing results...\n");
        
        // End satisfaction tour
        satisfaction_tour_end(&state->game_ecs.tour);
        
        // Get voyage data
        const VoyageProgress* progress = voyage_get_progress();
        
        // Show results screen with voyage data
        results_screen_show(
            "Vinga Lighthouse Tour",  // Route name from config
            progress->poi_satisfaction_scores,
            progress->total_pois,
            progress->voyage_duration
        );
        
        state->voyage_active = false;
        state->results_showing = true;
    }
}
