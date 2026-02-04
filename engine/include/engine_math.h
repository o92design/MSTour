#ifndef ENGINE_MATH_H
#define ENGINE_MATH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>

// Constants
#ifndef PI
#define PI 3.14159265358979323846f
#endif

#define DEG2RAD_CONST (PI / 180.0f)
#define RAD2DEG_CONST (180.0f / PI)

// Inline math utilities for performance-critical code

// Linear interpolation between two values
static inline float math_lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

// Clamp value between min and max
static inline float math_clamp(float value, float min_val, float max_val) {
    if (value < min_val) return min_val;
    if (value > max_val) return max_val;
    return value;
}

// Clamp integer value between min and max
static inline int math_clamp_int(int value, int min_val, int max_val) {
    if (value < min_val) return min_val;
    if (value > max_val) return max_val;
    return value;
}

// Convert degrees to radians
static inline float math_deg_to_rad(float degrees) {
    return degrees * DEG2RAD_CONST;
}

// Convert radians to degrees
static inline float math_rad_to_deg(float radians) {
    return radians * RAD2DEG_CONST;
}

// Minimum of two floats
static inline float math_min(float a, float b) {
    return (a < b) ? a : b;
}

// Maximum of two floats
static inline float math_max(float a, float b) {
    return (a > b) ? a : b;
}

// Minimum of two integers
static inline int math_min_int(int a, int b) {
    return (a < b) ? a : b;
}

// Maximum of two integers
static inline int math_max_int(int a, int b) {
    return (a > b) ? a : b;
}

// Absolute value (float)
static inline float math_abs(float value) {
    return (value < 0.0f) ? -value : value;
}

// Squared distance between two 2D points (avoids sqrt for comparisons)
static inline float math_distance_sq(float x1, float y1, float x2, float y2) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    return dx * dx + dy * dy;
}

// Distance between two 2D points
static inline float math_distance(float x1, float y1, float x2, float y2) {
    return sqrtf(math_distance_sq(x1, y1, x2, y2));
}

// Sign of a value (-1, 0, or 1)
static inline float math_sign(float value) {
    if (value > 0.0f) return 1.0f;
    if (value < 0.0f) return -1.0f;
    return 0.0f;
}

// Smooth step interpolation (ease in/out)
static inline float math_smoothstep(float edge0, float edge1, float x) {
    float t = math_clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
    return t * t * (3.0f - 2.0f * t);
}

// Move value toward target by max_delta
static inline float math_move_toward(float current, float target, float max_delta) {
    if (math_abs(target - current) <= max_delta) {
        return target;
    }
    return current + math_sign(target - current) * max_delta;
}

// Wrap angle to 0-360 range
static inline float math_wrap_angle_360(float angle) {
    while (angle >= 360.0f) angle -= 360.0f;
    while (angle < 0.0f) angle += 360.0f;
    return angle;
}

// Wrap angle to -180 to 180 range
static inline float math_wrap_angle_180(float angle) {
    while (angle > 180.0f) angle -= 360.0f;
    while (angle <= -180.0f) angle += 360.0f;
    return angle;
}

// Calculate shortest angle difference (for smooth rotation)
static inline float math_angle_diff(float from, float to) {
    float diff = math_wrap_angle_180(to - from);
    return diff;
}

#ifdef __cplusplus
}
#endif

#endif // ENGINE_MATH_H
