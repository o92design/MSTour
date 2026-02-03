#ifndef ENGINE_CORE_H
#define ENGINE_CORE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

// Engine configuration
typedef struct {
    const char* window_title;
    int window_width;
    int window_height;
    int target_fps;
} EngineConfig;

// Timing state
typedef struct {
    double delta_time;           // Time since last frame (seconds)
    double total_time;           // Total time since engine start (seconds)
    double fixed_delta_time;     // Fixed timestep for physics (seconds)
    double fixed_accumulator;    // Accumulator for fixed timestep
    uint64_t frame_count;        // Total frames rendered
    float fps;                   // Current frames per second
    float fps_smoothed;          // Smoothed FPS (rolling average)
} TimingState;

// Engine state
typedef struct {
    bool is_running;
    TimingState timing;
    int window_width;
    int window_height;
} EngineState;

// Core engine functions
bool engine_init(const EngineConfig* config);
void engine_shutdown(void);
bool engine_should_close(void);
void engine_begin_frame(void);
void engine_end_frame(void);
EngineState* engine_get_state(void);

// Timing functions
double engine_get_delta_time(void);
double engine_get_total_time(void);
float engine_get_fps(void);
uint64_t engine_get_frame_count(void);

// Fixed timestep support (for physics)
void engine_set_fixed_timestep(double fixed_dt);
bool engine_should_update_fixed(void);  // Call in loop until returns false
double engine_get_fixed_delta_time(void);

// Window functions
int engine_get_window_width(void);
int engine_get_window_height(void);
void engine_set_window_size(int width, int height);
bool engine_is_window_resized(void);

#ifdef __cplusplus
}
#endif

#endif // ENGINE_CORE_H
