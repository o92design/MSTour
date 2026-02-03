#include "game_ship_ecs.h"
#include "engine_math.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

// =============================================================================
// Ship Configuration
// =============================================================================

ShipEcsConfig ship_ecs_get_default_config(void) {
    ShipEcsConfig config;
    config.max_speed = 150.0f;
    config.acceleration = 45.0f;
    config.turn_rate = 40.0f;
    config.throttle_response = 0.4f;
    config.steering_response = 0.25f;
    config.coast_friction = 0.015f;
    config.drift_factor = 0.25f;
    config.reverse_speed_mult = 0.35f;
    config.reverse_accel_mult = 0.5f;
    config.speed_turn_factor = 0.65f;
    return config;
}

// =============================================================================
// Ship ECS Lifecycle
// =============================================================================

void ship_ecs_init(ShipEcsWorld* ship_world) {
    if (!ship_world) return;
    
    memset(ship_world, 0, sizeof(ShipEcsWorld));
    ship_world->initialized = true;
    
    printf("Ship ECS: Initialized\n");
}

void ship_ecs_shutdown(ShipEcsWorld* ship_world) {
    if (!ship_world) return;
    
    ship_world->initialized = false;
    printf("Ship ECS: Shutdown\n");
}

// =============================================================================
// Ship Component Access
// =============================================================================

void ship_ecs_set_config(ShipEcsWorld* ship_world, Entity e, const ShipEcsConfig* config) {
    if (!ship_world || !config || e == INVALID_ENTITY || e >= MAX_ENTITIES) return;
    
    ship_world->ships.max_speed[e] = config->max_speed;
    ship_world->ships.acceleration[e] = config->acceleration;
    ship_world->ships.turn_rate[e] = config->turn_rate;
    ship_world->ships.throttle_response[e] = config->throttle_response;
    ship_world->ships.steering_response[e] = config->steering_response;
    ship_world->ships.coast_friction[e] = config->coast_friction;
    ship_world->ships.drift_factor[e] = config->drift_factor;
    ship_world->ships.reverse_speed_mult[e] = config->reverse_speed_mult;
    ship_world->ships.reverse_accel_mult[e] = config->reverse_accel_mult;
    ship_world->ships.speed_turn_factor[e] = config->speed_turn_factor;
}

void ship_ecs_set_throttle(ShipEcsWorld* ship_world, Entity e, float throttle) {
    if (!ship_world || e == INVALID_ENTITY || e >= MAX_ENTITIES) return;
    ship_world->ships.target_throttle[e] = throttle;
}

void ship_ecs_set_rudder(ShipEcsWorld* ship_world, Entity e, float rudder) {
    if (!ship_world || e == INVALID_ENTITY || e >= MAX_ENTITIES) return;
    ship_world->ships.target_rudder[e] = rudder;
}

float ship_ecs_get_throttle(const ShipEcsWorld* ship_world, Entity e) {
    if (!ship_world || e == INVALID_ENTITY || e >= MAX_ENTITIES) return 0.0f;
    return ship_world->ships.throttle[e];
}

float ship_ecs_get_rudder(const ShipEcsWorld* ship_world, Entity e) {
    if (!ship_world || e == INVALID_ENTITY || e >= MAX_ENTITIES) return 0.0f;
    return ship_world->ships.rudder[e];
}

// =============================================================================
// Ship Physics System
// =============================================================================

void ship_ecs_system_physics(ECSWorld* ecs_world, ShipEcsWorld* ship_world, float delta_time) {
    if (!ecs_world || !ship_world) return;
    
    ComponentMask required = COMPONENT_TRANSFORM | COMPONENT_VELOCITY | COMPONENT_SHIP;
    
    for (Entity e = 1; e < MAX_ENTITIES; e++) {
        if ((ecs_world->entity_masks[e] & required) != required) continue;
        
        // Get per-entity config values from ship world
        float throttle_response = ship_world->ships.throttle_response[e];
        float steering_response = ship_world->ships.steering_response[e];
        float max_speed = ship_world->ships.max_speed[e];
        float accel = ship_world->ships.acceleration[e];
        float turn_rate = ship_world->ships.turn_rate[e];
        float speed_turn_factor = ship_world->ships.speed_turn_factor[e];
        float reverse_speed_mult = ship_world->ships.reverse_speed_mult[e];
        float reverse_accel_mult = ship_world->ships.reverse_accel_mult[e];
        float coast_friction = ship_world->ships.coast_friction[e];
        float drift_factor = ship_world->ships.drift_factor[e];
        
        // Smooth throttle/rudder inputs using config response times
        float throttle_rate = (throttle_response > 0.0f) 
            ? (delta_time / throttle_response) 
            : 1.0f;
        float rudder_rate = (steering_response > 0.0f) 
            ? (delta_time / steering_response) 
            : 1.0f;
        
        throttle_rate = math_clamp(throttle_rate, 0.0f, 1.0f);
        rudder_rate = math_clamp(rudder_rate, 0.0f, 1.0f);
        
        ship_world->ships.throttle[e] = math_lerp(ship_world->ships.throttle[e], 
                                                   ship_world->ships.target_throttle[e], 
                                                   throttle_rate);
        ship_world->ships.rudder[e] = math_lerp(ship_world->ships.rudder[e],
                                                 ship_world->ships.target_rudder[e],
                                                 rudder_rate);
        
        float throttle = ship_world->ships.throttle[e];
        float current_speed = ecs_world->velocities.speed[e];
        
        // Calculate target speed (handle reverse)
        float target_speed = throttle * max_speed;
        float effective_accel = accel;
        
        if (throttle < 0.0f) {
            // Reverse: slower max speed and acceleration
            target_speed *= reverse_speed_mult;
            effective_accel *= reverse_accel_mult;
        }
        
        // Apply acceleration or coasting
        if (math_abs(throttle) < 0.01f) {
            // Coasting - apply friction
            current_speed *= powf(1.0f - coast_friction, delta_time * 60.0f);
            if (math_abs(current_speed) < 0.5f) {
                current_speed = 0.0f;
            }
        } else {
            // Accelerating toward target speed
            float speed_diff = target_speed - current_speed;
            if (math_abs(speed_diff) > 0.1f) {
                float accel_amount = effective_accel * delta_time;
                if (speed_diff > 0) {
                    current_speed = math_min(current_speed + accel_amount, target_speed);
                } else {
                    current_speed = math_max(current_speed - accel_amount, target_speed);
                }
            }
        }
        ecs_world->velocities.speed[e] = current_speed;
        
        // Calculate turn effectiveness based on speed
        float speed_ratio = math_abs(current_speed) / max_speed;
        float turn_effectiveness = speed_turn_factor + 
                                   (1.0f - speed_turn_factor) * speed_ratio;
        
        // Calculate target angular velocity
        float rudder = ship_world->ships.rudder[e];
        float target_angular = rudder * turn_rate * turn_effectiveness;
        
        // Smooth angular velocity
        float angular_diff = target_angular - ecs_world->velocities.angular_vel[e];
        float angular_accel = turn_rate * 2.0f * delta_time;
        
        if (math_abs(angular_diff) > 0.1f) {
            if (angular_diff > 0) {
                ecs_world->velocities.angular_vel[e] = math_min(
                    ecs_world->velocities.angular_vel[e] + angular_accel, target_angular);
            } else {
                ecs_world->velocities.angular_vel[e] = math_max(
                    ecs_world->velocities.angular_vel[e] - angular_accel, target_angular);
            }
        }
        
        // Decay angular velocity when not turning
        if (math_abs(rudder) < 0.01f) {
            ecs_world->velocities.angular_vel[e] *= powf(0.9f, delta_time * 60.0f);
            if (math_abs(ecs_world->velocities.angular_vel[e]) < 0.1f) {
                ecs_world->velocities.angular_vel[e] = 0.0f;
            }
        }
        
        // Calculate velocity from speed and heading
        float heading_rad = math_deg_to_rad(ecs_world->transforms.rotation[e] - 90.0f);
        ecs_world->velocities.vel_x[e] = cosf(heading_rad) * ecs_world->velocities.speed[e];
        ecs_world->velocities.vel_y[e] = sinf(heading_rad) * ecs_world->velocities.speed[e];
        
        // Apply drift during turns
        float angular_vel = ecs_world->velocities.angular_vel[e];
        if (math_abs(angular_vel) > 0.1f && math_abs(current_speed) > 0.1f) {
            float drift_angle = ecs_world->transforms.rotation[e] + (angular_vel > 0 ? 90.0f : -90.0f);
            float drift_rad = math_deg_to_rad(drift_angle);
            float drift_magnitude = drift_factor * math_abs(angular_vel) * 
                                    math_abs(current_speed) * 0.01f * delta_time;
            
            ecs_world->velocities.vel_x[e] += cosf(drift_rad) * drift_magnitude;
            ecs_world->velocities.vel_y[e] += sinf(drift_rad) * drift_magnitude;
        }
    }
}
