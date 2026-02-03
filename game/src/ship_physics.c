#include "ship_physics.h"
#include "engine_math.h"
#include "game_ship_ecs.h"
#include <raylib.h>
#include <math.h>
#include <string.h>

void ship_physics_init(ShipState* state, float x, float y, float heading) {
    memset(state, 0, sizeof(ShipState));
    state->pos_x = x;
    state->pos_y = y;
    state->heading = heading;
}

ShipPhysicsConfig ship_physics_get_default_config(void) {
    // Use game-layer ship ECS defaults as single source of truth
    ShipEcsConfig ecs_defaults = ship_ecs_get_default_config();
    
    ShipPhysicsConfig config = {
        // Linear Movement
        .max_speed = ecs_defaults.max_speed,
        .acceleration = ecs_defaults.acceleration,
        .deceleration = 2.0f,  // Not in ECS config (legacy only)
        .reverse_speed_multiplier = ecs_defaults.reverse_speed_mult,
        .reverse_accel_multiplier = ecs_defaults.reverse_accel_mult,
        
        // Rotational Movement
        .max_turn_rate = ecs_defaults.turn_rate,
        .turn_accel = 80.0f,  // Not in ECS config (legacy only)
        .speed_turn_factor = ecs_defaults.speed_turn_factor,
        
        // Momentum & Drift
        .coast_friction = ecs_defaults.coast_friction,
        .drift_factor = ecs_defaults.drift_factor,
        .momentum_retention = 0.96f,  // Not in ECS config (legacy only)
        
        // Responsiveness
        .throttle_response_time = ecs_defaults.throttle_response,
        .steering_response_time = ecs_defaults.steering_response
    };
    return config;
}

void ship_physics_process_actions(ShipState* state, float throttle_input, float rudder_input) {
    // Clamp inputs to valid range
    state->target_throttle = math_clamp(throttle_input, -1.0f, 1.0f);
    state->target_rudder = math_clamp(rudder_input, -1.0f, 1.0f);
}

void ship_physics_smooth_inputs(ShipState* state, const ShipPhysicsConfig* config, float delta_time) {
    // Calculate interpolation factor based on response time
    // response_time is in seconds, so we convert to per-frame rate
    float throttle_rate = (config->throttle_response_time > 0) 
        ? (delta_time / config->throttle_response_time) 
        : 1.0f;
    float steering_rate = (config->steering_response_time > 0) 
        ? (delta_time / config->steering_response_time) 
        : 1.0f;
    
    // Clamp rates to [0, 1] to avoid overshooting
    throttle_rate = math_clamp(throttle_rate, 0.0f, 1.0f);
    steering_rate = math_clamp(steering_rate, 0.0f, 1.0f);
    
    // Smoothly interpolate from current to target
    state->throttle = math_lerp(state->throttle, state->target_throttle, throttle_rate);
    state->rudder = math_lerp(state->rudder, state->target_rudder, steering_rate);
}

void ship_physics_apply_acceleration(ShipState* state, const ShipPhysicsConfig* config, float delta_time) {
    // Determine max speed and acceleration based on throttle direction
    float target_speed = state->throttle * config->max_speed;
    float accel = config->acceleration;
    
    // Reverse is slower and accelerates slower
    if (state->throttle < 0) {
        target_speed *= config->reverse_speed_multiplier;
        accel *= config->reverse_accel_multiplier;
    }
    
    // If throttle is zero or near-zero, apply coasting (handled in friction)
    if (fabs(state->throttle) < 0.01f) {
        // No acceleration, just friction
        return;
    }
    
    // Calculate speed difference
    float speed_diff = target_speed - state->speed;
    
    // Apply acceleration toward target speed
    if (fabs(speed_diff) > 0.1f) {
        float accel_amount = accel * delta_time;
        if (speed_diff > 0) {
            state->speed += accel_amount;
            if (state->speed > target_speed) {
                state->speed = target_speed;
            }
        } else {
            state->speed -= accel_amount;
            if (state->speed < target_speed) {
                state->speed = target_speed;
            }
        }
    }
}

void ship_physics_apply_rotation(ShipState* state, const ShipPhysicsConfig* config, float delta_time) {
    // Turning effectiveness based on speed
    // At zero speed, turning is harder (speed_turn_factor)
    // At max speed, turning is at full effectiveness
    float speed_ratio = fabs(state->speed) / config->max_speed;
    float turn_effectiveness = config->speed_turn_factor + 
                               (1.0f - config->speed_turn_factor) * speed_ratio;
    
    // Target angular velocity based on rudder input
    float target_angular_velocity = state->rudder * config->max_turn_rate * turn_effectiveness;
    
    // Smoothly accelerate toward target angular velocity
    float angular_accel = config->turn_accel * delta_time;
    float angular_diff = target_angular_velocity - state->angular_velocity;
    
    if (fabs(angular_diff) > 0.1f) {
        if (angular_diff > 0) {
            state->angular_velocity += angular_accel;
            if (state->angular_velocity > target_angular_velocity) {
                state->angular_velocity = target_angular_velocity;
            }
        } else {
            state->angular_velocity -= angular_accel;
            if (state->angular_velocity < target_angular_velocity) {
                state->angular_velocity = target_angular_velocity;
            }
        }
    }
    
    // Apply angular velocity to heading
    state->heading += state->angular_velocity * delta_time;
    
    // Keep heading in 0-360 range
    while (state->heading >= 360.0f) state->heading -= 360.0f;
    while (state->heading < 0.0f) state->heading += 360.0f;
    
    // Apply drift during turns (ship drifts outward)
    if (fabs(state->angular_velocity) > 0.1f && fabs(state->speed) > 0.1f) {
        // Calculate perpendicular direction (outward from turn)
        float drift_angle = state->heading + (state->angular_velocity > 0 ? 90.0f : -90.0f);
        float drift_rad = math_deg_to_rad(drift_angle);
        
        // Drift magnitude based on turn rate and speed
        float drift_magnitude = config->drift_factor * fabs(state->angular_velocity) * 
                                fabs(state->speed) * 0.01f * delta_time;
        
        // Apply drift to velocity
        state->velocity_x += cos(drift_rad) * drift_magnitude;
        state->velocity_y += sin(drift_rad) * drift_magnitude;
    }
}

void ship_physics_apply_friction(ShipState* state, const ShipPhysicsConfig* config, float delta_time) {
    // Apply friction when coasting (throttle near zero)
    if (fabs(state->throttle) < 0.01f) {
        // Exponential decay for smooth coasting
        state->speed *= powf(1.0f - config->coast_friction, delta_time * 60.0f);
        
        // Stop completely if very slow
        if (fabs(state->speed) < 0.5f) {
            state->speed = 0.0f;
        }
    }
    
    // Apply momentum retention to velocity
    state->velocity_x *= powf(config->momentum_retention, delta_time * 60.0f);
    state->velocity_y *= powf(config->momentum_retention, delta_time * 60.0f);
    
    // Decay angular velocity when not turning
    if (fabs(state->rudder) < 0.01f) {
        state->angular_velocity *= powf(0.9f, delta_time * 60.0f);
        if (fabs(state->angular_velocity) < 0.1f) {
            state->angular_velocity = 0.0f;
        }
    }
}

void ship_physics_update_position(ShipState* state, float delta_time) {
    // Convert heading to radians (0° = north, so we subtract 90° to align with standard coords)
    float heading_rad = math_deg_to_rad(state->heading - 90.0f);
    
    // Calculate forward velocity from speed and heading
    float forward_vel_x = cos(heading_rad) * state->speed;
    float forward_vel_y = sin(heading_rad) * state->speed;
    
    // Combine forward velocity with drift velocity
    state->velocity_x = forward_vel_x;
    state->velocity_y = forward_vel_y;
    
    // Update position
    state->pos_x += state->velocity_x * delta_time;
    state->pos_y += state->velocity_y * delta_time;
}

void ship_physics_update(ShipState* state, const ShipPhysicsConfig* config, float delta_time) {
    // Clamp delta_time to avoid instability
    if (delta_time > 0.1f) {
        delta_time = 0.1f;
    }
    
    // Update in order
    ship_physics_smooth_inputs(state, config, delta_time);
    ship_physics_apply_acceleration(state, config, delta_time);
    ship_physics_apply_rotation(state, config, delta_time);
    ship_physics_apply_friction(state, config, delta_time);
    ship_physics_update_position(state, delta_time);
}
