#include "engine_core.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

// Global engine state
static EngineState g_engine_state = {0};
static EngineConfig g_engine_config = {0};

bool engine_init(const EngineConfig* config) {
    if (!config) {
        fprintf(stderr, "ERROR: Null config passed to engine_init\n");
        return false;
    }

    // Store config
    g_engine_config = *config;

    // Initialize raylib window
    InitWindow(config->window_width, config->window_height, config->window_title);
    SetTargetFPS(config->target_fps);

    // Initialize engine state
    g_engine_state.is_running = true;
    g_engine_state.delta_time = 0.0;
    g_engine_state.frame_count = 0;

    printf("Engine initialized: %s [%dx%d @ %d FPS]\n",
           config->window_title,
           config->window_width,
           config->window_height,
           config->target_fps);

    return true;
}

void engine_shutdown(void) {
    printf("Engine shutdown. Total frames: %llu\n", 
           (unsigned long long)g_engine_state.frame_count);
    
    CloseWindow();
    g_engine_state.is_running = false;
}

bool engine_should_close(void) {
    return WindowShouldClose() || !g_engine_state.is_running;
}

void engine_begin_frame(void) {
    g_engine_state.delta_time = GetFrameTime();
    BeginDrawing();
}

void engine_end_frame(void) {
    EndDrawing();
    g_engine_state.frame_count++;
}

EngineState* engine_get_state(void) {
    return &g_engine_state;
}
