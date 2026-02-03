#ifndef ENGINE_ECS_H
#define ENGINE_ECS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

// =============================================================================
// Entity Component System (Data-Oriented Design)
// 
// This ECS follows Struct-of-Arrays (SoA) pattern for cache efficiency.
// Each component type is stored in a contiguous array.
// Entities are just IDs that index into component arrays.
// 
// The engine provides generic components (Transform, Velocity, Renderable,
// Collider). Game-specific components (Ship, AI) should be defined in the
// game layer using the reserved COMPONENT_GAME_* bits.
// =============================================================================

// Entity handle
typedef uint32_t Entity;
#define INVALID_ENTITY 0
#define MAX_ENTITIES 1024

// Component flags (bitmask for which components an entity has)
typedef uint32_t ComponentMask;

// Component types (engine-provided)
// Bits 0-3: Engine core components
// Bits 4-15: Reserved for game use (COMPONENT_GAME_0 through COMPONENT_GAME_11)
typedef enum ComponentType {
    COMPONENT_NONE       = 0,
    COMPONENT_TRANSFORM  = (1 << 0),
    COMPONENT_VELOCITY   = (1 << 1),
    COMPONENT_RENDERABLE = (1 << 2),
    COMPONENT_COLLIDER   = (1 << 3),
    
    // Reserved for game layer (do not use in engine code)
    COMPONENT_GAME_0     = (1 << 4),
    COMPONENT_GAME_1     = (1 << 5),
    COMPONENT_GAME_2     = (1 << 6),
    COMPONENT_GAME_3     = (1 << 7),
    COMPONENT_GAME_4     = (1 << 8),
    COMPONENT_GAME_5     = (1 << 9),
    COMPONENT_GAME_6     = (1 << 10),
    COMPONENT_GAME_7     = (1 << 11),
    
    COMPONENT_MAX        = (1 << 12)
} ComponentType;

// =============================================================================
// Component Data (SoA Layout) - Engine Core Components
// =============================================================================

// Transform component data (position, rotation, scale)
typedef struct TransformComponents {
    float pos_x[MAX_ENTITIES];
    float pos_y[MAX_ENTITIES];
    float rotation[MAX_ENTITIES];    // degrees
    float scale_x[MAX_ENTITIES];
    float scale_y[MAX_ENTITIES];
} TransformComponents;

// Velocity component data (movement)
typedef struct VelocityComponents {
    float vel_x[MAX_ENTITIES];
    float vel_y[MAX_ENTITIES];
    float angular_vel[MAX_ENTITIES]; // degrees/sec
    float speed[MAX_ENTITIES];       // magnitude (cached)
} VelocityComponents;

// Renderable component data (visual representation)
typedef struct RenderableComponents {
    uint32_t sprite_id[MAX_ENTITIES];  // Or model ID
    uint8_t layer[MAX_ENTITIES];       // Render layer
    uint8_t color_r[MAX_ENTITIES];
    uint8_t color_g[MAX_ENTITIES];
    uint8_t color_b[MAX_ENTITIES];
    uint8_t color_a[MAX_ENTITIES];
    bool visible[MAX_ENTITIES];
} RenderableComponents;

// Collider component data (physics collision)
typedef struct ColliderComponents {
    float radius[MAX_ENTITIES];        // Collision radius
    float width[MAX_ENTITIES];         // For box colliders
    float height[MAX_ENTITIES];
    uint8_t collision_layer[MAX_ENTITIES];
    uint8_t collision_mask[MAX_ENTITIES];
} ColliderComponents;

// =============================================================================
// World - Container for all ECS data (Engine Core Only)
// =============================================================================

typedef struct ECSWorld {
    // Entity management
    ComponentMask entity_masks[MAX_ENTITIES];
    uint32_t entity_count;
    Entity free_list[MAX_ENTITIES];
    uint32_t free_count;
    
    // Engine core component arrays (SoA)
    TransformComponents transforms;
    VelocityComponents velocities;
    RenderableComponents renderables;
    ColliderComponents colliders;
    
    // Note: Game-specific components (Ship, AI) are stored in separate
    // game-layer structures (ShipEcsWorld, AIEcsWorld) that parallel this
    // world's entity indices.
    
    // Stats
    uint32_t active_entities;
} ECSWorld;

// =============================================================================
// World Management
// =============================================================================

// Initialize ECS world
void ecs_world_init(ECSWorld* world);

// Clear all entities from world
void ecs_world_clear(ECSWorld* world);

// Get world statistics
uint32_t ecs_get_entity_count(const ECSWorld* world);

// =============================================================================
// Entity Management
// =============================================================================

// Create a new entity
Entity ecs_create_entity(ECSWorld* world);

// Destroy an entity
void ecs_destroy_entity(ECSWorld* world, Entity entity);

// Check if entity is valid
bool ecs_entity_valid(const ECSWorld* world, Entity entity);

// Check if entity has component(s)
bool ecs_has_component(const ECSWorld* world, Entity entity, ComponentType type);
bool ecs_has_components(const ECSWorld* world, Entity entity, ComponentMask mask);

// Add component to entity
void ecs_add_component(ECSWorld* world, Entity entity, ComponentType type);

// Remove component from entity
void ecs_remove_component(ECSWorld* world, Entity entity, ComponentType type);

// =============================================================================
// Component Access (Inline for performance)
// =============================================================================

// Get component mask for entity
static inline ComponentMask ecs_get_mask(const ECSWorld* world, Entity entity) {
    if (entity == INVALID_ENTITY || entity >= MAX_ENTITIES) return COMPONENT_NONE;
    return world->entity_masks[entity];
}

// Transform accessors
static inline void ecs_set_position(ECSWorld* world, Entity e, float x, float y) {
    world->transforms.pos_x[e] = x;
    world->transforms.pos_y[e] = y;
}

static inline void ecs_set_rotation(ECSWorld* world, Entity e, float rotation) {
    world->transforms.rotation[e] = rotation;
}

// Velocity accessors
static inline void ecs_set_velocity(ECSWorld* world, Entity e, float vx, float vy) {
    world->velocities.vel_x[e] = vx;
    world->velocities.vel_y[e] = vy;
}

// =============================================================================
// Batch Operations (Efficient iteration)
// =============================================================================

// Callback for entity iteration
typedef void (*ECSEntityCallback)(ECSWorld* world, Entity entity, void* user_data);

// Iterate all entities with specified components
void ecs_foreach(ECSWorld* world, ComponentMask required_mask, ECSEntityCallback callback, void* user_data);

// Get count of entities with specified components
uint32_t ecs_count_with(const ECSWorld* world, ComponentMask required_mask);

// =============================================================================
// System Update Functions (Engine Core)
// =============================================================================

// Update all transform positions based on velocities
// Entities need COMPONENT_TRANSFORM | COMPONENT_VELOCITY
void ecs_system_movement(ECSWorld* world, float delta_time);

// Note: Game-specific systems (ship physics, AI) are implemented in
// game_ship_ecs.c and game_ai_ecs.c, not in the engine.

#ifdef __cplusplus
}
#endif

#endif // ENGINE_ECS_H
