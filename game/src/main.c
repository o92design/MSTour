#include "engine_core.h"
#include "engine_renderer.h"
#include <raylib.h>
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

    // Main game loop
    while (!engine_should_close()) {
        engine_begin_frame();

        // Clear screen with Raylib's SKYBLUE
        renderer_clear(SKYBLUE);

        // Draw simple UI
        renderer_draw_text("MS Tour - Archipelago Shipping Company", 20, 20, 30, WHITE);
        renderer_draw_text("Press ESC to exit", 20, 60, 20, WHITE);

        // Draw a simple ship placeholder
        renderer_draw_rectangle(600, 350, 80, 40, RED);
        renderer_draw_text("Ship", 610, 360, 20, WHITE);

        // Display frame info
        EngineState* state = engine_get_state();
        char fps_text[64];
        snprintf(fps_text, sizeof(fps_text), "FPS: %.1f | Frame: %llu", 
                 1.0 / state->delta_time, 
                 (unsigned long long)state->frame_count);
        renderer_draw_text(fps_text, 20, 680, 20, YELLOW);

        engine_end_frame();
    }

    // Cleanup
    renderer_shutdown();
    engine_shutdown();

    return 0;
}
