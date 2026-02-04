#ifndef GAME_ECS_H
#define GAME_ECS_H

#include "engine_ecs.h"
#include "game_ship_ecs.h"
#include "game_ai_ecs.h"
#include "game_poi_ecs.h"
#include "game_fog_of_war.h"
#include "game_satisfaction.h"
#include "ship_physics.h"
#include <stdbool.h>

// =============================================================================
// Game ECS Layer
// 
// Provides game-specific entity factories and utilities built on top of
// the engine's generic ECS system. Coordinates between engine ECS (Transform,
// Velocity) and game-layer ECS (Ship, AI, POI).
// =============================================================================

// =============================================================================
// Combined Game ECS State
// =============================================================================

typedef struct GameEcsState {
    ECSWorld* ecs_world;        // Pointer to engine ECS world
    ShipEcsWorld ship_world;    // Game-layer ship components
    AIEcsWorld ai_world;        // Game-layer AI components
    POIEcsWorld poi_world;      // Game-layer POI components
    FogOfWarState fog;          // Fog of war visibility
    TourSatisfaction tour;      // Current tour satisfaction tracking
} GameEcsState;

// =============================================================================
// Game ECS Lifecycle
// =============================================================================

// Initialize game ECS (also initializes ship/AI sub-worlds)
void game_ecs_init(GameEcsState* state, ECSWorld* ecs_world);

// Shutdown game ECS (clears ship/AI sub-worlds)
void game_ecs_shutdown(GameEcsState* state);

// =============================================================================
// Ship Entity Factory
// =============================================================================

// Create a player ship entity with all required components
// Returns entity handle (INVALID_ENTITY on failure)
Entity game_create_player_ship(GameEcsState* state, float x, float y, float heading);

// Create an AI ship entity
Entity game_create_ai_ship(GameEcsState* state, float x, float y, float heading, uint32_t route_id);

// Configure ship from ShipPhysicsConfig (legacy config format)
void game_ecs_apply_physics_config(GameEcsState* state, Entity ship, const ShipPhysicsConfig* config);

// =============================================================================
// Ship Control (convenience wrappers)
// =============================================================================

// Set ship throttle from telegraph order (-1.0 to 1.0)
void game_ecs_set_throttle(GameEcsState* state, Entity ship, float throttle);

// Set ship steering/rudder (-1.0 left to 1.0 right)
void game_ecs_set_steering(GameEcsState* state, Entity ship, float steering);

// =============================================================================
// Ship Queries
// =============================================================================

// Get ship position
void game_ecs_get_ship_position(const GameEcsState* state, Entity ship, float* x, float* y);

// Get ship rotation (heading in degrees)
float game_ecs_get_ship_heading(const GameEcsState* state, Entity ship);

// Get ship speed
float game_ecs_get_ship_speed(const GameEcsState* state, Entity ship);

// Get ship velocity components
void game_ecs_get_ship_velocity(const GameEcsState* state, Entity ship, float* vx, float* vy);

// Get ship throttle (current, not target)
float game_ecs_get_ship_throttle(const GameEcsState* state, Entity ship);

// Get ship rudder (current, not target)
float game_ecs_get_ship_rudder(const GameEcsState* state, Entity ship);

// Get ship angular velocity
float game_ecs_get_ship_angular_velocity(const GameEcsState* state, Entity ship);

// =============================================================================
// Systems
// =============================================================================

// Run all game ECS systems (ship physics, AI, then movement)
void game_ecs_update(GameEcsState* state, float delta_time);

// =============================================================================
// Compatibility Bridge (for gradual migration)
// =============================================================================

// Copy ECS ship state to legacy ShipState struct (for rendering/UI)
void game_ecs_to_ship_state(const GameEcsState* state, Entity ship, ShipState* out_state);

// Copy legacy ShipState to ECS entity (for initialization)
void game_ecs_from_ship_state(GameEcsState* state, Entity ship, const ShipState* ship_state);

// =============================================================================
// POI Functions
// =============================================================================

// Load POIs from file (call after init)
bool game_ecs_load_pois(GameEcsState* state, const char* filepath);

// Get POI world for queries
POIEcsWorld* game_ecs_get_poi_world(GameEcsState* state);
const POIEcsWorld* game_ecs_get_poi_world_const(const GameEcsState* state);

// Get fog of war state
FogOfWarState* game_ecs_get_fog(GameEcsState* state);
const FogOfWarState* game_ecs_get_fog_const(const GameEcsState* state);

// Get tour satisfaction
TourSatisfaction* game_ecs_get_tour(GameEcsState* state);
const TourSatisfaction* game_ecs_get_tour_const(const GameEcsState* state);

#endif // GAME_ECS_H
