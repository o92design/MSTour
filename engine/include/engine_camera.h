#ifndef ENGINE_CAMERA_H
#define ENGINE_CAMERA_H

#ifdef __cplusplus
extern "C" {
#endif

#include <raylib.h>

// =============================================================================
// 2D Camera System
// Provides camera abstraction for top-down/orthographic views
// =============================================================================

// Camera configuration
typedef struct CameraConfig {
    float min_zoom;
    float max_zoom;
    float zoom_speed;
    float pan_speed;
    bool smooth_follow;
    float follow_lerp_speed;
} CameraConfig;

// Camera state
typedef struct CameraState {
    Vector2 position;      // World position camera is looking at
    Vector2 target;        // Target position for smooth following
    float zoom;            // Zoom level (1.0 = normal)
    float rotation;        // Rotation in degrees
    Vector2 offset;        // Screen offset (usually center of screen)
} CameraState;

// Get default camera configuration
CameraConfig camera_get_default_config(void);

// Initialize camera at position
void camera_init(CameraState* camera, float x, float y);

// Initialize camera with screen dimensions (sets offset to center)
void camera_init_centered(CameraState* camera, float x, float y, int screen_width, int screen_height);

// Update camera (for smooth following)
void camera_update(CameraState* camera, const CameraConfig* config, float delta_time);

// Set camera target position (for smooth follow)
void camera_set_target(CameraState* camera, float x, float y);

// Set camera position immediately
void camera_set_position(CameraState* camera, float x, float y);

// Zoom camera
void camera_zoom(CameraState* camera, const CameraConfig* config, float zoom_delta);

// Set absolute zoom level
void camera_set_zoom(CameraState* camera, const CameraConfig* config, float zoom);

// Rotate camera
void camera_rotate(CameraState* camera, float angle_delta);

// Set absolute rotation
void camera_set_rotation(CameraState* camera, float angle);

// Convert screen coordinates to world coordinates
Vector2 camera_screen_to_world(const CameraState* camera, Vector2 screen_pos);

// Convert world coordinates to screen coordinates
Vector2 camera_world_to_screen(const CameraState* camera, Vector2 world_pos);

// Get Raylib Camera2D for rendering
Camera2D camera_get_raylib_camera(const CameraState* camera);

// Begin camera rendering mode
void camera_begin(const CameraState* camera);

// End camera rendering mode
void camera_end(void);

// Check if a world rectangle is visible on screen
bool camera_is_rect_visible(const CameraState* camera, Rectangle world_rect, int screen_width, int screen_height);

// Check if a world point is visible on screen
bool camera_is_point_visible(const CameraState* camera, Vector2 world_pos, int screen_width, int screen_height);

#ifdef __cplusplus
}
#endif

#endif // ENGINE_CAMERA_H
