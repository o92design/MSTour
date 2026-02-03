#ifndef GAME_SHIP_ECS_H
#define GAME_SHIP_ECS_H

#include "engine_ecs.h"
#include <stdbool.h>

// =============================================================================
// Game Ship ECS
// 
// Ship-specific ECS components and systems. This is game-layer code that
// extends the engine's generic ECS with ship behavior.
// =============================================================================

// Ship component flag (uses game-reserved bit from engine)
#define COMPONENT_SHIP COMPONENT_GAME_0

// =============================================================================
// Ship Configuration (defaults and initialization)
// =============================================================================

typedef struct ShipEcsConfig {
    float max_speed;
    float acceleration;
    float turn_rate;
    float throttle_response;
    float steering_response;
    float coast_friction;
    float drift_factor;
    float reverse_speed_mult;
    float reverse_accel_mult;
    float speed_turn_factor;
} ShipEcsConfig;

// Get default ship configuration
ShipEcsConfig ship_ecs_get_default_config(void);

// =============================================================================
// Ship Components (SoA Layout)
// =============================================================================

typedef struct ShipComponents {
    // Control inputs (smoothed)
    float throttle[MAX_ENTITIES];
    float target_throttle[MAX_ENTITIES];
    float rudder[MAX_ENTITIES];
    float target_rudder[MAX_ENTITIES];
    
    // Ship properties (per-entity configuration)
    float max_speed[MAX_ENTITIES];
    float acceleration[MAX_ENTITIES];
    float turn_rate[MAX_ENTITIES];
    float throttle_response[MAX_ENTITIES];
    float steering_response[MAX_ENTITIES];
    float coast_friction[MAX_ENTITIES];
    float drift_factor[MAX_ENTITIES];
    float reverse_speed_mult[MAX_ENTITIES];
    float reverse_accel_mult[MAX_ENTITIES];
    float speed_turn_factor[MAX_ENTITIES];
    
    // Ship state
    uint8_t telegraph_order[MAX_ENTITIES];  // -3 to +3
} ShipComponents;

// Global ship component storage
// (Parallel to ECSWorld's entity arrays)
typedef struct ShipEcsWorld {
    ShipComponents ships;
    bool initialized;
} ShipEcsWorld;

// =============================================================================
// Ship ECS Lifecycle
// =============================================================================

// Initialize ship ECS world
void ship_ecs_init(ShipEcsWorld* ship_world);

// Shutdown ship ECS world
void ship_ecs_shutdown(ShipEcsWorld* ship_world);

// =============================================================================
// Ship Component Access
// =============================================================================

// Apply config to a ship entity
void ship_ecs_set_config(ShipEcsWorld* ship_world, Entity e, const ShipEcsConfig* config);

// Set ship throttle target (-1.0 to 1.0)
void ship_ecs_set_throttle(ShipEcsWorld* ship_world, Entity e, float throttle);

// Set ship rudder target (-1.0 left to 1.0 right)
void ship_ecs_set_rudder(ShipEcsWorld* ship_world, Entity e, float rudder);

// Get ship throttle (current, not target)
float ship_ecs_get_throttle(const ShipEcsWorld* ship_world, Entity e);

// Get ship rudder (current, not target)
float ship_ecs_get_rudder(const ShipEcsWorld* ship_world, Entity e);

// =============================================================================
// Ship Physics System
// =============================================================================

// Update ship physics for all entities with COMPONENT_SHIP
// This handles throttle/rudder smoothing, acceleration, turning, drift
void ship_ecs_system_physics(ECSWorld* ecs_world, ShipEcsWorld* ship_world, float delta_time);

#endif // GAME_SHIP_ECS_H
