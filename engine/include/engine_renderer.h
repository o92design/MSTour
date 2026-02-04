#ifndef ENGINE_RENDERER_H
#define ENGINE_RENDERER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <raylib.h>
#include <stdint.h>

// =============================================================================
// Basic Renderer Functions
// =============================================================================

void renderer_init(void);
void renderer_shutdown(void);
void renderer_clear(Color color);
void renderer_draw_rectangle(int x, int y, int width, int height, Color color);
void renderer_draw_text(const char* text, int x, int y, int font_size, Color color);

// =============================================================================
// Batched Rectangle Rendering
// 
// For rendering many same-colored rectangles efficiently (e.g., fog cells).
// Collects rectangles and renders them in a single batch to reduce draw calls.
// =============================================================================

#define RECT_BATCH_MAX_RECTS 2048

typedef struct RectBatch {
    int16_t x[RECT_BATCH_MAX_RECTS];
    int16_t y[RECT_BATCH_MAX_RECTS];
    int16_t w[RECT_BATCH_MAX_RECTS];
    int16_t h[RECT_BATCH_MAX_RECTS];
    uint32_t count;
    Color color;
} RectBatch;

// Initialize a rectangle batch with a color
void renderer_rect_batch_init(RectBatch* batch, Color color);

// Add a rectangle to the batch (returns false if batch is full)
bool renderer_rect_batch_add(RectBatch* batch, int x, int y, int w, int h);

// Flush all rectangles in the batch (draws them efficiently)
void renderer_rect_batch_flush(RectBatch* batch);

// Clear the batch without drawing
void renderer_rect_batch_clear(RectBatch* batch);

#ifdef __cplusplus
}
#endif

#endif // ENGINE_RENDERER_H
