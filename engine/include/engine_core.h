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

// Engine state
typedef struct {
    bool is_running;
    double delta_time;
    uint64_t frame_count;
} EngineState;

// Core engine functions
bool engine_init(const EngineConfig* config);
void engine_shutdown(void);
bool engine_should_close(void);
void engine_begin_frame(void);
void engine_end_frame(void);
EngineState* engine_get_state(void);

#ifdef __cplusplus
}
#endif

#endif // ENGINE_CORE_H
