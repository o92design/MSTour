#include "engine_core.h"
#include "engine_config.h"
#include "engine_renderer.h"
#include "game_state.h"
#include "game_update.h"
#include "game_render.h"
#include "input_actions.h"
#include "ship_ui.h"
#include "game_constants.h"
#include <raylib.h>
#include <stdio.h>

// Global config file for hot-reload support
static ConfigFile g_config;

int main(void) {
    printf("=== MS Tour - Gothenburg Archipelago Shipping Company ===\n");

    // Load configuration from file
    config_init(&g_config);
    if (!config_load(&g_config, "config.ini")) {
        printf("Using default configuration\n");
    }

    // Get window configuration (uses defaults if not loaded)
    WindowConfig window_cfg = config_get_window(&g_config);
    
    // Engine configuration from loaded config
    EngineConfig config = {
        .window_title = window_cfg.title,
        .window_width = window_cfg.width,
        .window_height = window_cfg.height,
        .target_fps = window_cfg.target_fps
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

    // Initialize game state (pass config for audio settings)
    GameState* game = game_get_state();
    if (!game_state_init(game, &g_config)) {
        fprintf(stderr, "Failed to initialize game state!\n");
        engine_shutdown();
        return 1;
    }

    // Initialize ship UI
    ship_ui_init();
    printf("Ship UI system initialized\n");

    printf("Ship initialized at (%.1f, %.1f)\n", 
           game->player_ship.pos_x, game->player_ship.pos_y);
    printf("Debug tools: F1-F6 for controls, F3 for help\n");

    // Main game loop
    while (!engine_should_close()) {
        float delta_time = (float)engine_get_delta_time();

        engine_begin_frame();

        // Update game systems
        game_update(game, delta_time);

        // Render game
        game_render(game);

        engine_end_frame();
    }

    // Cleanup
    ship_ui_cleanup();
    game_state_shutdown(game);
    renderer_shutdown();
    engine_shutdown();

    printf("MS Tour shutdown complete\n");

    return 0;
}
