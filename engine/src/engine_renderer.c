#include "engine_renderer.h"
#include <raylib.h>
#include <rlgl.h>

void renderer_init(void) {
    // Renderer initialization (if needed beyond window init)
}

void renderer_shutdown(void) {
    // Renderer cleanup (if needed)
}

void renderer_clear(Color color) {
    ClearBackground(color);
}

void renderer_draw_rectangle(int x, int y, int width, int height, Color color) {
    DrawRectangle(x, y, width, height, color);
}

void renderer_draw_text(const char* text, int x, int y, int font_size, Color color) {
    DrawText(text, x, y, font_size, color);
}

// =============================================================================
// Batched Rectangle Rendering
// =============================================================================

void renderer_rect_batch_init(RectBatch* batch, Color color) {
    if (!batch) return;
    batch->count = 0;
    batch->color = color;
}

bool renderer_rect_batch_add(RectBatch* batch, int x, int y, int w, int h) {
    if (!batch || batch->count >= RECT_BATCH_MAX_RECTS) return false;
    
    uint32_t i = batch->count++;
    batch->x[i] = (int16_t)x;
    batch->y[i] = (int16_t)y;
    batch->w[i] = (int16_t)w;
    batch->h[i] = (int16_t)h;
    return true;
}

void renderer_rect_batch_flush(RectBatch* batch) {
    if (!batch || batch->count == 0) return;
    
    // Use rlgl for efficient batched quad rendering
    rlSetTexture(rlGetTextureIdDefault());
    rlBegin(RL_QUADS);
    
    rlColor4ub(batch->color.r, batch->color.g, batch->color.b, batch->color.a);
    
    for (uint32_t i = 0; i < batch->count; i++) {
        float x = (float)batch->x[i];
        float y = (float)batch->y[i];
        float w = (float)batch->w[i];
        float h = (float)batch->h[i];
        
        // Two triangles forming a quad (CCW winding)
        rlVertex2f(x, y);
        rlVertex2f(x, y + h);
        rlVertex2f(x + w, y + h);
        rlVertex2f(x + w, y);
    }
    
    rlEnd();
    rlSetTexture(0);
    
    batch->count = 0;
}

void renderer_rect_batch_clear(RectBatch* batch) {
    if (!batch) return;
    batch->count = 0;
}
