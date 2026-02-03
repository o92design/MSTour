#include "engine_camera.h"
#include "engine_math.h"
#include <raylib.h>

CameraConfig camera_get_default_config(void) {
    CameraConfig config = {
        .min_zoom = 0.25f,
        .max_zoom = 4.0f,
        .zoom_speed = 0.1f,
        .pan_speed = 200.0f,
        .smooth_follow = true,
        .follow_lerp_speed = 5.0f
    };
    return config;
}

void camera_init(CameraState* camera, float x, float y) {
    if (!camera) return;
    
    camera->position = (Vector2){x, y};
    camera->target = (Vector2){x, y};
    camera->zoom = 1.0f;
    camera->rotation = 0.0f;
    camera->offset = (Vector2){0.0f, 0.0f};
}

void camera_init_centered(CameraState* camera, float x, float y, int screen_width, int screen_height) {
    if (!camera) return;
    
    camera_init(camera, x, y);
    camera->offset = (Vector2){screen_width / 2.0f, screen_height / 2.0f};
}

void camera_update(CameraState* camera, const CameraConfig* config, float delta_time) {
    if (!camera || !config) return;
    
    if (config->smooth_follow) {
        // Smoothly interpolate position toward target
        float lerp_factor = math_clamp(config->follow_lerp_speed * delta_time, 0.0f, 1.0f);
        camera->position.x = math_lerp(camera->position.x, camera->target.x, lerp_factor);
        camera->position.y = math_lerp(camera->position.y, camera->target.y, lerp_factor);
    } else {
        // Instant follow
        camera->position = camera->target;
    }
}

void camera_set_target(CameraState* camera, float x, float y) {
    if (!camera) return;
    camera->target = (Vector2){x, y};
}

void camera_set_position(CameraState* camera, float x, float y) {
    if (!camera) return;
    camera->position = (Vector2){x, y};
    camera->target = (Vector2){x, y};
}

void camera_zoom(CameraState* camera, const CameraConfig* config, float zoom_delta) {
    if (!camera || !config) return;
    camera->zoom = math_clamp(camera->zoom + zoom_delta, config->min_zoom, config->max_zoom);
}

void camera_set_zoom(CameraState* camera, const CameraConfig* config, float zoom) {
    if (!camera || !config) return;
    camera->zoom = math_clamp(zoom, config->min_zoom, config->max_zoom);
}

void camera_rotate(CameraState* camera, float angle_delta) {
    if (!camera) return;
    camera->rotation = math_wrap_angle_360(camera->rotation + angle_delta);
}

void camera_set_rotation(CameraState* camera, float angle) {
    if (!camera) return;
    camera->rotation = math_wrap_angle_360(angle);
}

Vector2 camera_screen_to_world(const CameraState* camera, Vector2 screen_pos) {
    if (!camera) return screen_pos;
    
    Camera2D raylib_cam = camera_get_raylib_camera(camera);
    return GetScreenToWorld2D(screen_pos, raylib_cam);
}

Vector2 camera_world_to_screen(const CameraState* camera, Vector2 world_pos) {
    if (!camera) return world_pos;
    
    Camera2D raylib_cam = camera_get_raylib_camera(camera);
    return GetWorldToScreen2D(world_pos, raylib_cam);
}

Camera2D camera_get_raylib_camera(const CameraState* camera) {
    Camera2D raylib_cam = {0};
    
    if (camera) {
        raylib_cam.target = camera->position;
        raylib_cam.offset = camera->offset;
        raylib_cam.rotation = camera->rotation;
        raylib_cam.zoom = camera->zoom;
    }
    
    return raylib_cam;
}

void camera_begin(const CameraState* camera) {
    if (!camera) return;
    
    Camera2D raylib_cam = camera_get_raylib_camera(camera);
    BeginMode2D(raylib_cam);
}

void camera_end(void) {
    EndMode2D();
}

bool camera_is_rect_visible(const CameraState* camera, Rectangle world_rect, int screen_width, int screen_height) {
    if (!camera) return false;
    
    // Calculate visible world area
    float half_width = (screen_width / 2.0f) / camera->zoom;
    float half_height = (screen_height / 2.0f) / camera->zoom;
    
    Rectangle view_rect = {
        camera->position.x - half_width,
        camera->position.y - half_height,
        half_width * 2.0f,
        half_height * 2.0f
    };
    
    return CheckCollisionRecs(view_rect, world_rect);
}

bool camera_is_point_visible(const CameraState* camera, Vector2 world_pos, int screen_width, int screen_height) {
    if (!camera) return false;
    
    float half_width = (screen_width / 2.0f) / camera->zoom;
    float half_height = (screen_height / 2.0f) / camera->zoom;
    
    return (world_pos.x >= camera->position.x - half_width &&
            world_pos.x <= camera->position.x + half_width &&
            world_pos.y >= camera->position.y - half_height &&
            world_pos.y <= camera->position.y + half_height);
}
