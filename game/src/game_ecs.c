#include "game_ecs.h"
#include "game_poi_loader.h"
#include <string.h>
#include <stdio.h>

// =============================================================================
// POI Visit Callback
// =============================================================================

static void on_poi_visit(int poi_index, Entity visitor, void* user_data) {
    GameEcsState* state = (GameEcsState*)user_data;
    if (!state) return;
    
    const char* name = poi_ecs_get_name(&state->poi_world, poi_index);
    printf("POI Visit: Ship %u visited '%s'\n", visitor, name);
    
    // Record for tour satisfaction
    if (satisfaction_tour_is_active(&state->tour)) {
        int bonus = satisfaction_record_poi_visit(&state->tour, &state->poi_world, poi_index);
        if (bonus > 0) {
            printf("  Satisfaction bonus: +%d\n", bonus);
        }
    }
}

// =============================================================================
// Game ECS Lifecycle
// =============================================================================

void game_ecs_init(GameEcsState* state, ECSWorld* ecs_world) {
    if (!state || !ecs_world) return;
    
    state->ecs_world = ecs_world;
    ecs_world_init(ecs_world);
    ship_ecs_init(&state->ship_world);
    ai_ecs_init(&state->ai_world);
    poi_ecs_init(&state->poi_world);
    fog_init(&state->fog);
    
    // Initialize tour (not active until explicitly started)
    memset(&state->tour, 0, sizeof(TourSatisfaction));
    
    printf("Game ECS: Initialized with ship, AI, and POI sub-worlds\n");
}

void game_ecs_shutdown(GameEcsState* state) {
    if (!state) return;
    
    fog_shutdown(&state->fog);
    poi_ecs_shutdown(&state->poi_world);
    ai_ecs_shutdown(&state->ai_world);
    ship_ecs_shutdown(&state->ship_world);
    if (state->ecs_world) {
        ecs_world_clear(state->ecs_world);
    }
    state->ecs_world = NULL;
    
    printf("Game ECS: Shutdown\n");
}

// =============================================================================
// Ship Entity Factory
// =============================================================================

Entity game_create_player_ship(GameEcsState* state, float x, float y, float heading) {
    if (!state || !state->ecs_world) return INVALID_ENTITY;
    
    Entity ship = ecs_create_entity(state->ecs_world);
    if (ship == INVALID_ENTITY) {
        fprintf(stderr, "Game ECS: Failed to create player ship entity\n");
        return INVALID_ENTITY;
    }
    
    // Add engine core components
    ecs_add_component(state->ecs_world, ship, COMPONENT_TRANSFORM);
    ecs_add_component(state->ecs_world, ship, COMPONENT_VELOCITY);
    ecs_add_component(state->ecs_world, ship, COMPONENT_SHIP);  // COMPONENT_GAME_0
    ecs_add_component(state->ecs_world, ship, COMPONENT_RENDERABLE);
    
    // Initialize transform
    ecs_set_position(state->ecs_world, ship, x, y);
    ecs_set_rotation(state->ecs_world, ship, heading);
    state->ecs_world->transforms.scale_x[ship] = 1.0f;
    state->ecs_world->transforms.scale_y[ship] = 1.0f;
    
    // Initialize velocity to zero
    ecs_set_velocity(state->ecs_world, ship, 0.0f, 0.0f);
    state->ecs_world->velocities.angular_vel[ship] = 0.0f;
    state->ecs_world->velocities.speed[ship] = 0.0f;
    
    // Initialize ship with default config (in game-layer ship world)
    ShipEcsConfig config = ship_ecs_get_default_config();
    ship_ecs_set_config(&state->ship_world, ship, &config);
    
    // Initialize ship control state
    state->ship_world.ships.throttle[ship] = 0.0f;
    state->ship_world.ships.target_throttle[ship] = 0.0f;
    state->ship_world.ships.rudder[ship] = 0.0f;
    state->ship_world.ships.target_rudder[ship] = 0.0f;
    state->ship_world.ships.telegraph_order[ship] = 0;
    
    // Initialize renderable
    state->ecs_world->renderables.visible[ship] = true;
    state->ecs_world->renderables.layer[ship] = 1;  // Ships on layer 1
    state->ecs_world->renderables.color_r[ship] = 200;
    state->ecs_world->renderables.color_g[ship] = 200;
    state->ecs_world->renderables.color_b[ship] = 200;
    state->ecs_world->renderables.color_a[ship] = 255;
    
    printf("Game ECS: Created player ship entity %u at (%.1f, %.1f)\n", ship, x, y);
    return ship;
}

Entity game_create_ai_ship(GameEcsState* state, float x, float y, float heading, uint32_t route_id) {
    if (!state) return INVALID_ENTITY;
    
    // Create base ship
    Entity ship = game_create_player_ship(state, x, y, heading);
    if (ship == INVALID_ENTITY) return INVALID_ENTITY;
    
    // Add AI component flag
    ecs_add_component(state->ecs_world, ship, COMPONENT_AI);  // COMPONENT_GAME_1
    
    // Initialize AI in game-layer AI world
    ai_ecs_setup(&state->ai_world, ship, route_id);
    
    printf("Game ECS: Created AI ship entity %u with route %u\n", ship, route_id);
    return ship;
}

void game_ecs_apply_physics_config(GameEcsState* state, Entity ship, const ShipPhysicsConfig* config) {
    if (!state || !config || ship == INVALID_ENTITY) return;
    if (!ecs_has_component(state->ecs_world, ship, COMPONENT_SHIP)) return;
    
    // Map ShipPhysicsConfig to ShipEcsConfig
    ShipEcsConfig ecs_config;
    ecs_config.max_speed = config->max_speed;
    ecs_config.acceleration = config->acceleration;
    ecs_config.turn_rate = config->max_turn_rate;
    ecs_config.throttle_response = config->throttle_response_time;
    ecs_config.steering_response = config->steering_response_time;
    ecs_config.coast_friction = config->coast_friction;
    ecs_config.drift_factor = config->drift_factor;
    ecs_config.reverse_speed_mult = config->reverse_speed_multiplier;
    ecs_config.reverse_accel_mult = config->reverse_accel_multiplier;
    ecs_config.speed_turn_factor = config->speed_turn_factor;
    
    ship_ecs_set_config(&state->ship_world, ship, &ecs_config);
}

// =============================================================================
// Ship Control
// =============================================================================

void game_ecs_set_throttle(GameEcsState* state, Entity ship, float throttle) {
    if (!state || ship == INVALID_ENTITY) return;
    if (!ecs_has_component(state->ecs_world, ship, COMPONENT_SHIP)) return;
    
    ship_ecs_set_throttle(&state->ship_world, ship, throttle);
}

void game_ecs_set_steering(GameEcsState* state, Entity ship, float steering) {
    if (!state || ship == INVALID_ENTITY) return;
    if (!ecs_has_component(state->ecs_world, ship, COMPONENT_SHIP)) return;
    
    ship_ecs_set_rudder(&state->ship_world, ship, steering);
}

// =============================================================================
// Ship Queries
// =============================================================================

void game_ecs_get_ship_position(const GameEcsState* state, Entity ship, float* x, float* y) {
    if (!state || !state->ecs_world || ship == INVALID_ENTITY || !x || !y) return;
    
    *x = state->ecs_world->transforms.pos_x[ship];
    *y = state->ecs_world->transforms.pos_y[ship];
}

float game_ecs_get_ship_heading(const GameEcsState* state, Entity ship) {
    if (!state || !state->ecs_world || ship == INVALID_ENTITY) return 0.0f;
    return state->ecs_world->transforms.rotation[ship];
}

float game_ecs_get_ship_speed(const GameEcsState* state, Entity ship) {
    if (!state || !state->ecs_world || ship == INVALID_ENTITY) return 0.0f;
    return state->ecs_world->velocities.speed[ship];
}

void game_ecs_get_ship_velocity(const GameEcsState* state, Entity ship, float* vx, float* vy) {
    if (!state || !state->ecs_world || ship == INVALID_ENTITY || !vx || !vy) return;
    
    *vx = state->ecs_world->velocities.vel_x[ship];
    *vy = state->ecs_world->velocities.vel_y[ship];
}

float game_ecs_get_ship_throttle(const GameEcsState* state, Entity ship) {
    if (!state || ship == INVALID_ENTITY) return 0.0f;
    return ship_ecs_get_throttle(&state->ship_world, ship);
}

float game_ecs_get_ship_rudder(const GameEcsState* state, Entity ship) {
    if (!state || ship == INVALID_ENTITY) return 0.0f;
    return ship_ecs_get_rudder(&state->ship_world, ship);
}

float game_ecs_get_ship_angular_velocity(const GameEcsState* state, Entity ship) {
    if (!state || !state->ecs_world || ship == INVALID_ENTITY) return 0.0f;
    return state->ecs_world->velocities.angular_vel[ship];
}

// =============================================================================
// Systems
// =============================================================================

void game_ecs_update(GameEcsState* state, float delta_time) {
    if (!state || !state->ecs_world) return;
    
    // 1. Update AI (sets throttle/rudder targets for AI ships)
    ai_ecs_system_update(state->ecs_world, &state->ai_world, delta_time);
    
    // 2. Update ship physics (converts throttle/rudder to velocity)
    ship_ecs_system_physics(state->ecs_world, &state->ship_world, delta_time);
    
    // 3. Update movement (applies velocity to transform)
    ecs_system_movement(state->ecs_world, delta_time);
    
    // 4. Update POI visits
    POISystemContext poi_ctx = {
        .on_visit = on_poi_visit,
        .user_data = state
    };
    poi_ecs_system_update(&state->poi_world, state->ecs_world, COMPONENT_SHIP, &poi_ctx);
    
    // 5. Update fog of war
    fog_system_update(&state->fog, &state->poi_world, state->ecs_world, 
                      COMPONENT_SHIP, delta_time);
}

// =============================================================================
// Compatibility Bridge
// =============================================================================

void game_ecs_to_ship_state(const GameEcsState* state, Entity ship, ShipState* out_state) {
    if (!state || !state->ecs_world || !out_state || ship == INVALID_ENTITY) return;
    
    // Transform
    out_state->pos_x = state->ecs_world->transforms.pos_x[ship];
    out_state->pos_y = state->ecs_world->transforms.pos_y[ship];
    out_state->heading = state->ecs_world->transforms.rotation[ship];
    
    // Velocity
    out_state->velocity_x = state->ecs_world->velocities.vel_x[ship];
    out_state->velocity_y = state->ecs_world->velocities.vel_y[ship];
    out_state->speed = state->ecs_world->velocities.speed[ship];
    out_state->angular_velocity = state->ecs_world->velocities.angular_vel[ship];
    
    // Ship controls (from game-layer ship world)
    out_state->throttle = state->ship_world.ships.throttle[ship];
    out_state->target_throttle = state->ship_world.ships.target_throttle[ship];
    out_state->rudder = state->ship_world.ships.rudder[ship];
    out_state->target_rudder = state->ship_world.ships.target_rudder[ship];
}

void game_ecs_from_ship_state(GameEcsState* state, Entity ship, const ShipState* ship_state) {
    if (!state || !state->ecs_world || !ship_state || ship == INVALID_ENTITY) return;
    
    // Transform
    state->ecs_world->transforms.pos_x[ship] = ship_state->pos_x;
    state->ecs_world->transforms.pos_y[ship] = ship_state->pos_y;
    state->ecs_world->transforms.rotation[ship] = ship_state->heading;
    
    // Velocity
    state->ecs_world->velocities.vel_x[ship] = ship_state->velocity_x;
    state->ecs_world->velocities.vel_y[ship] = ship_state->velocity_y;
    state->ecs_world->velocities.speed[ship] = ship_state->speed;
    state->ecs_world->velocities.angular_vel[ship] = ship_state->angular_velocity;
    
    // Ship controls (to game-layer ship world)
    state->ship_world.ships.throttle[ship] = ship_state->throttle;
    state->ship_world.ships.target_throttle[ship] = ship_state->target_throttle;
    state->ship_world.ships.rudder[ship] = ship_state->rudder;
    state->ship_world.ships.target_rudder[ship] = ship_state->target_rudder;
}

// =============================================================================
// POI Functions
// =============================================================================

bool game_ecs_load_pois(GameEcsState* state, const char* filepath) {
    if (!state) return false;
    
    POILoadResult result = poi_load_from_file(&state->poi_world, filepath);
    
    if (result != POI_LOAD_SUCCESS) {
        printf("Game ECS: Failed to load POIs from '%s': %s\n", 
               filepath ? filepath : "(default)", 
               poi_load_result_to_string(result));
        
        // Fall back to defaults
        poi_load_defaults(&state->poi_world);
        return false;
    }
    
    printf("Game ECS: Loaded %u POIs\n", poi_ecs_get_count(&state->poi_world));
    return true;
}

POIEcsWorld* game_ecs_get_poi_world(GameEcsState* state) {
    if (!state) return NULL;
    return &state->poi_world;
}

const POIEcsWorld* game_ecs_get_poi_world_const(const GameEcsState* state) {
    if (!state) return NULL;
    return &state->poi_world;
}

FogOfWarState* game_ecs_get_fog(GameEcsState* state) {
    if (!state) return NULL;
    return &state->fog;
}

const FogOfWarState* game_ecs_get_fog_const(const GameEcsState* state) {
    if (!state) return NULL;
    return &state->fog;
}

TourSatisfaction* game_ecs_get_tour(GameEcsState* state) {
    if (!state) return NULL;
    return &state->tour;
}

const TourSatisfaction* game_ecs_get_tour_const(const GameEcsState* state) {
    if (!state) return NULL;
    return &state->tour;
}
