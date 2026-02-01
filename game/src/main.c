#include "engine_core.h"
#include "engine_renderer.h"
#include "../include/input_actions.h"
#include "../include/ship_physics.h"
#include "../include/ship_telegraph.h"
#include "../include/config.h"
#include "../include/debug_tools.h"
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

int main(void) {
    printf("=== MS Tour - Gothenburg Archipelago Shipping Company ===\n");

    // Engine configuration
    EngineConfig config = {
        .window_title = "MS Tour - Archipelago Shipping",
        .window_width = 1280,
        .window_height = 720,
        .target_fps = 60
    };

    // Initialize engine
    if (!engine_init(&config)) {
        fprintf(stderr, "Failed to initialize engine!\n");
        return 1;
    }

    renderer_init();

    // Initialize input action system
    input_actions_init();
    printf("Input action system initialized\n");

    // Initialize ship
    ShipState ship;
    ship_physics_init(&ship, 640.0f, 360.0f, 0.0f); // Center of screen, facing north
    
    // Initialize telegraph (engine order system)
    ShipTelegraph telegraph;
    ship_telegraph_init(&telegraph);
    printf("Engine telegraph initialized at STOP\n");
    
    // Load physics configuration from config.ini
    ShipPhysicsConfig physics_config;
    const char* config_path = "config.ini";
    if (!config_load_ship_physics(config_path, &physics_config)) {
        // If load fails, save defaults to create the file
        physics_config = ship_physics_get_default_config();
        config_save_ship_physics(config_path, &physics_config);
    }
    
    // Initialize debug tools
    DebugState debug_state;
    debug_tools_init(&debug_state);
    printf("Debug tools initialized (F1-F6 for debug controls, F3 for help)\n");

    printf("Ship initialized at (%.1f, %.1f)\n", ship.pos_x, ship.pos_y);

    // Main game loop
    while (!engine_should_close()) {
        EngineState* engine_state = engine_get_state();
        float delta_time = engine_state->delta_time;

        engine_begin_frame();

        // === INPUT ===
        input_actions_update();
        
        // Update debug tools (handle F1-F6 keys)
        debug_tools_update(&debug_state);
        
        // Debug commands
        if (IsKeyPressed(KEY_F4)) {
            debug_tools_teleport_ship(&ship, 640.0f, 360.0f);
        }
        if (IsKeyPressed(KEY_F5)) {
            debug_tools_reset_ship(&ship);
            ship_telegraph_init(&telegraph); // Also reset telegraph
        }
        if (IsKeyPressed(KEY_F6)) {
            config_reload_ship_physics(config_path, &physics_config);
        }
        
        // Telegraph controls (key presses, not holds)
        if (input_action_pressed(SHIP_ACTION_THROTTLE_UP)) {
            ship_telegraph_ring_up(&telegraph);
        }
        if (input_action_pressed(SHIP_ACTION_THROTTLE_DOWN)) {
            ship_telegraph_ring_down(&telegraph);
        }
        
        // Update telegraph timer
        ship_telegraph_update(&telegraph, delta_time);
        
        // Get throttle from telegraph and steering from input
        float throttle_input = ship_telegraph_get_throttle(&telegraph);
        float steering_input = input_action_get_steering_axis();
        
        // Process actions and update ship physics
        ship_physics_process_actions(&ship, throttle_input, steering_input);
        ship_physics_update(&ship, &physics_config, delta_time);

        // === RENDERING ===
        renderer_clear(SKYBLUE);

        // Draw ocean/water effect (darker blue rectangle as water)
        DrawRectangle(0, 0, 1280, 720, (Color){65, 105, 225, 255}); // Royal blue for water

        // Draw simple UI (only if help is not shown)
        if (!debug_state.show_help) {
            renderer_draw_text("MS Tour - Ship Control Prototype", 20, 20, 30, WHITE);
            renderer_draw_text("Controls: W/S=Telegraph Orders | A/D=Turn | F3=Help", 20, 60, 20, LIGHTGRAY);
        }

        // === DRAW SHIP ===
        // Ship dimensions
        float ship_length = 80.0f;
        float ship_width = 40.0f;
        
        // Calculate ship vertices for triangle (pointing in heading direction)
        // Ship heading: 0° = north (up), rotating clockwise
        Vector2 ship_pos = {ship.pos_x, ship.pos_y};
        
        // Simple triangle ship representation
        // Front point (bow)
        float bow_angle = (ship.heading - 90.0f) * DEG2RAD;
        Vector2 bow = {
            ship_pos.x + cosf(bow_angle) * ship_length * 0.6f,
            ship_pos.y + sinf(bow_angle) * ship_length * 0.6f
        };
        
        // Rear points (stern)
        float stern_angle_left = (ship.heading - 90.0f + 150.0f) * DEG2RAD;
        float stern_angle_right = (ship.heading - 90.0f - 150.0f) * DEG2RAD;
        Vector2 stern_left = {
            ship_pos.x + cosf(stern_angle_left) * ship_width * 0.6f,
            ship_pos.y + sinf(stern_angle_left) * ship_width * 0.6f
        };
        Vector2 stern_right = {
            ship_pos.x + cosf(stern_angle_right) * ship_width * 0.6f,
            ship_pos.y + sinf(stern_angle_right) * ship_width * 0.6f
        };
        
        // Draw ship as triangle
        DrawTriangle(stern_left, stern_right, bow, RED);
        DrawTriangleLines(stern_left, stern_right, bow, DARKGRAY);
        
        // Draw ship center point
        DrawCircle((int)ship_pos.x, (int)ship_pos.y, 3.0f, YELLOW);

        // === DEBUG VISUALIZATION ===
        debug_tools_draw_visualization(&debug_state, &ship, &physics_config);

        // === DEBUG INFO (only if help and debug panel are not shown) ===
        if (!debug_state.show_help && !debug_state.show_debug_panel) {
            char debug_text[256];
            
            // Engine Telegraph Order (prominent display)
            const char* order_name = ship_telegraph_get_order_name(ship_telegraph_get_order(&telegraph));
            snprintf(debug_text, sizeof(debug_text), "Engine Order: %s", order_name);
            renderer_draw_text(debug_text, 20, 95, 22, GOLD);
            
            // Position
            snprintf(debug_text, sizeof(debug_text), "Position: (%.1f, %.1f)", ship.pos_x, ship.pos_y);
            renderer_draw_text(debug_text, 20, 130, 18, WHITE);
            
            // Heading
            snprintf(debug_text, sizeof(debug_text), "Heading: %.1f°", ship.heading);
            renderer_draw_text(debug_text, 20, 155, 18, WHITE);
            
            // Speed
            snprintf(debug_text, sizeof(debug_text), "Speed: %.1f u/s (%.1f%%)", 
                     ship.speed, (ship.speed / physics_config.max_speed) * 100.0f);
            renderer_draw_text(debug_text, 20, 180, 18, WHITE);

            // Display frame info
            snprintf(debug_text, sizeof(debug_text), "FPS: %.1f | Frame: %llu", 
                     1.0f / delta_time, 
                     (unsigned long long)engine_state->frame_count);
            renderer_draw_text(debug_text, 20, 680, 18, YELLOW);
        }

        // === DEBUG PANEL ===
        debug_tools_draw_panel(&debug_state, &ship, &physics_config);

        // === HELP OVERLAY (drawn last, on top of everything) ===
        debug_tools_draw_help(&debug_state);

        engine_end_frame();
    }

    // Cleanup
    renderer_shutdown();
    engine_shutdown();

    printf("Ship control prototype shutdown complete\n");

    return 0;
}
