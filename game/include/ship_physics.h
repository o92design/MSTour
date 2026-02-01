#ifndef SHIP_PHYSICS_H
#define SHIP_PHYSICS_H

#include <stdbool.h>

// Ship state (data-oriented design)
typedef struct ShipState {
    // Position & Orientation
    float pos_x;
    float pos_y;
    float heading;          // degrees (0 = north, clockwise)
    
    // Velocity
    float velocity_x;
    float velocity_y;
    float speed;            // magnitude
    float angular_velocity; // degrees/sec
    
    // Control Input (smoothed values)
    float throttle;         // -1 to 1 (negative = reverse)
    float rudder;           // -1 to 1 (negative = left)
    
    // Control Input (raw target values from input)
    float target_throttle;
    float target_rudder;
} ShipState;

// Ship physics configuration (will be loaded from config.ini later)
typedef struct ShipPhysicsConfig {
    // Linear Movement
    float max_speed;                  // units/second
    float acceleration;               // units/second²
    float deceleration;               // units/second²
    float reverse_speed_multiplier;   // 0-1, typically 0.35
    float reverse_accel_multiplier;   // 0-1, typically 0.5
    
    // Rotational Movement
    float max_turn_rate;              // degrees/second
    float turn_accel;                 // degrees/second²
    float speed_turn_factor;          // turn effectiveness at low speed (0-1)
    
    // Momentum & Drift
    float coast_friction;             // friction coefficient when throttle released
    float drift_factor;               // how much ship drifts outward in turns (0-1)
    float momentum_retention;         // per-frame momentum retention (0-1)
    
    // Responsiveness
    float throttle_response_time;     // seconds to respond to throttle input
    float steering_response_time;     // seconds to respond to steering input
} ShipPhysicsConfig;

// Initialize ship at position with default values
void ship_physics_init(ShipState* state, float x, float y, float heading);

// Get default physics configuration
ShipPhysicsConfig ship_physics_get_default_config(void);

// Update ship physics for one frame
void ship_physics_update(ShipState* state, const ShipPhysicsConfig* config, float delta_time);

// Process actions and update target throttle/rudder
void ship_physics_process_actions(ShipState* state, float throttle_input, float rudder_input);

// Individual update components (called by ship_physics_update)
void ship_physics_smooth_inputs(ShipState* state, const ShipPhysicsConfig* config, float delta_time);
void ship_physics_apply_acceleration(ShipState* state, const ShipPhysicsConfig* config, float delta_time);
void ship_physics_apply_rotation(ShipState* state, const ShipPhysicsConfig* config, float delta_time);
void ship_physics_apply_friction(ShipState* state, const ShipPhysicsConfig* config, float delta_time);
void ship_physics_update_position(ShipState* state, float delta_time);

#endif // SHIP_PHYSICS_H
