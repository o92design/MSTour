#ifndef ENGINE_RENDERER_H
#define ENGINE_RENDERER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <raylib.h>

// Renderer functions
void renderer_init(void);
void renderer_shutdown(void);
void renderer_clear(Color color);
void renderer_draw_rectangle(int x, int y, int width, int height, Color color);
void renderer_draw_text(const char* text, int x, int y, int font_size, Color color);

#ifdef __cplusplus
}
#endif

#endif // ENGINE_RENDERER_H
