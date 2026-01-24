#include "engine_renderer.h"
#include <raylib.h>

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
