#include "engine_core.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

// Global engine state
static EngineState g_engine_state = {0};
static EngineConfig g_engine_config = {0};

// FPS smoothing factor
#define FPS_SMOOTH_FACTOR 0.9f

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
    g_engine_state.window_width = config->window_width;
    g_engine_state.window_height = config->window_height;
    
    // Initialize timing
    g_engine_state.timing.delta_time = 0.0;
    g_engine_state.timing.total_time = 0.0;
    g_engine_state.timing.fixed_delta_time = 1.0 / 60.0; // Default 60Hz fixed update
    g_engine_state.timing.fixed_accumulator = 0.0;
    g_engine_state.timing.frame_count = 0;
    g_engine_state.timing.fps = (float)config->target_fps;
    g_engine_state.timing.fps_smoothed = (float)config->target_fps;

    printf("Engine initialized: %s [%dx%d @ %d FPS]\n",
           config->window_title,
           config->window_width,
           config->window_height,
           config->target_fps);

    return true;
}

void engine_shutdown(void) {
    printf("Engine shutdown. Total frames: %llu, Total time: %.1f seconds\n", 
           (unsigned long long)g_engine_state.timing.frame_count,
           g_engine_state.timing.total_time);
    
    CloseWindow();
    g_engine_state.is_running = false;
}

bool engine_should_close(void) {
    return WindowShouldClose() || !g_engine_state.is_running;
}

void engine_begin_frame(void) {
    // Update timing
    double dt = GetFrameTime();
    g_engine_state.timing.delta_time = dt;
    g_engine_state.timing.total_time += dt;
    
    // Update FPS
    g_engine_state.timing.fps = (dt > 0.0) ? (float)(1.0 / dt) : 0.0f;
    g_engine_state.timing.fps_smoothed = 
        FPS_SMOOTH_FACTOR * g_engine_state.timing.fps_smoothed +
        (1.0f - FPS_SMOOTH_FACTOR) * g_engine_state.timing.fps;
    
    // Accumulate time for fixed timestep
    g_engine_state.timing.fixed_accumulator += dt;
    
    // Track window size changes
    g_engine_state.window_width = GetScreenWidth();
    g_engine_state.window_height = GetScreenHeight();
    
    BeginDrawing();
}

void engine_end_frame(void) {
    EndDrawing();
    g_engine_state.timing.frame_count++;
}

EngineState* engine_get_state(void) {
    return &g_engine_state;
}

// Timing functions
double engine_get_delta_time(void) {
    return g_engine_state.timing.delta_time;
}

double engine_get_total_time(void) {
    return g_engine_state.timing.total_time;
}

float engine_get_fps(void) {
    return g_engine_state.timing.fps_smoothed;
}

uint64_t engine_get_frame_count(void) {
    return g_engine_state.timing.frame_count;
}

// Fixed timestep support
void engine_set_fixed_timestep(double fixed_dt) {
    if (fixed_dt > 0.0) {
        g_engine_state.timing.fixed_delta_time = fixed_dt;
    }
}

bool engine_should_update_fixed(void) {
    if (g_engine_state.timing.fixed_accumulator >= g_engine_state.timing.fixed_delta_time) {
        g_engine_state.timing.fixed_accumulator -= g_engine_state.timing.fixed_delta_time;
        return true;
    }
    return false;
}

double engine_get_fixed_delta_time(void) {
    return g_engine_state.timing.fixed_delta_time;
}

// Window functions
int engine_get_window_width(void) {
    return g_engine_state.window_width;
}

int engine_get_window_height(void) {
    return g_engine_state.window_height;
}

void engine_set_window_size(int width, int height) {
    SetWindowSize(width, height);
    g_engine_state.window_width = width;
    g_engine_state.window_height = height;
}

bool engine_is_window_resized(void) {
    return IsWindowResized();
}
