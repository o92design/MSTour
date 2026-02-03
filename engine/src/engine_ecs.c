#include "engine_ecs.h"
#include "engine_math.h"
#include <string.h>
#include <stdio.h>

// =============================================================================
// World Management
// =============================================================================

void ecs_world_init(ECSWorld* world) {
    if (!world) return;
    
    memset(world, 0, sizeof(ECSWorld));
    
    // Initialize free list (all entities available)
    // Start from 1 (0 is INVALID_ENTITY)
    for (uint32_t i = 1; i < MAX_ENTITIES; i++) {
        world->free_list[i - 1] = i;
    }
    world->free_count = MAX_ENTITIES - 1;
    
    // Initialize default values for components
    for (uint32_t i = 0; i < MAX_ENTITIES; i++) {
        world->transforms.scale_x[i] = 1.0f;
        world->transforms.scale_y[i] = 1.0f;
        world->renderables.visible[i] = true;
        world->renderables.color_a[i] = 255;
    }
    
    printf("ECS: World initialized (max %d entities)\n", MAX_ENTITIES);
}

void ecs_world_clear(ECSWorld* world) {
    if (!world) return;
    
    // Reset all entity masks
    memset(world->entity_masks, 0, sizeof(world->entity_masks));
    
    // Rebuild free list
    for (uint32_t i = 1; i < MAX_ENTITIES; i++) {
        world->free_list[i - 1] = i;
    }
    world->free_count = MAX_ENTITIES - 1;
    world->entity_count = 0;
    world->active_entities = 0;
    
    printf("ECS: World cleared\n");
}

uint32_t ecs_get_entity_count(const ECSWorld* world) {
    if (!world) return 0;
    return world->active_entities;
}

// =============================================================================
// Entity Management
// =============================================================================

Entity ecs_create_entity(ECSWorld* world) {
    if (!world || world->free_count == 0) {
        printf("ECS: Cannot create entity - no free slots\n");
        return INVALID_ENTITY;
    }
    
    // Pop from free list
    world->free_count--;
    Entity entity = world->free_list[world->free_count];
    
    // Initialize entity
    world->entity_masks[entity] = COMPONENT_NONE;
    world->entity_count++;
    world->active_entities++;
    
    return entity;
}

void ecs_destroy_entity(ECSWorld* world, Entity entity) {
    if (!world || entity == INVALID_ENTITY || entity >= MAX_ENTITIES) return;
    if (world->entity_masks[entity] == COMPONENT_NONE) return; // Already destroyed
    
    // Clear mask
    world->entity_masks[entity] = COMPONENT_NONE;
    
    // Add to free list
    world->free_list[world->free_count] = entity;
    world->free_count++;
    world->active_entities--;
}

bool ecs_entity_valid(const ECSWorld* world, Entity entity) {
    if (!world || entity == INVALID_ENTITY || entity >= MAX_ENTITIES) return false;

    // An entity is valid if it is not currently in the free list.
    // Fast path: if it has any components, it is definitely alive.
    if (world->entity_masks[entity] != COMPONENT_NONE) {
        return true;
    }

    // Slow path: entity has no components; determine validity by checking the free list.
    // If the entity is present in the free list, it is invalid (destroyed/never allocated).
    for (uint32_t i = 0; i < world->free_count; i++) {
        if (world->free_list[i] == entity) {
            return false;
        }
    }

    // Not in the free list, so this is a live entity slot that currently has no components.
    return true;
}

bool ecs_has_component(const ECSWorld* world, Entity entity, ComponentType type) {
    if (!world || entity == INVALID_ENTITY || entity >= MAX_ENTITIES) return false;
    return (world->entity_masks[entity] & type) != 0;
}

bool ecs_has_components(const ECSWorld* world, Entity entity, ComponentMask mask) {
    if (!world || entity == INVALID_ENTITY || entity >= MAX_ENTITIES) return false;
    return (world->entity_masks[entity] & mask) == mask;
}

void ecs_add_component(ECSWorld* world, Entity entity, ComponentType type) {
    if (!world || entity == INVALID_ENTITY || entity >= MAX_ENTITIES) return;
    world->entity_masks[entity] |= type;
}

void ecs_remove_component(ECSWorld* world, Entity entity, ComponentType type) {
    if (!world || entity == INVALID_ENTITY || entity >= MAX_ENTITIES) return;
    world->entity_masks[entity] &= ~type;
}

// =============================================================================
// Batch Operations
// =============================================================================

void ecs_foreach(ECSWorld* world, ComponentMask required_mask, ECSEntityCallback callback, void* user_data) {
    if (!world || !callback) return;
    
    for (Entity e = 1; e < MAX_ENTITIES; e++) {
        if ((world->entity_masks[e] & required_mask) == required_mask) {
            callback(world, e, user_data);
        }
    }
}

uint32_t ecs_count_with(const ECSWorld* world, ComponentMask required_mask) {
    if (!world) return 0;
    
    uint32_t count = 0;
    for (Entity e = 1; e < MAX_ENTITIES; e++) {
        if ((world->entity_masks[e] & required_mask) == required_mask) {
            count++;
        }
    }
    return count;
}

// =============================================================================
// System Update Functions (Engine Core Only)
// =============================================================================

void ecs_system_movement(ECSWorld* world, float delta_time) {
    if (!world) return;
    
    ComponentMask required = COMPONENT_TRANSFORM | COMPONENT_VELOCITY;
    
    // Batch update all entities with transform and velocity
    for (Entity e = 1; e < MAX_ENTITIES; e++) {
        if ((world->entity_masks[e] & required) != required) continue;
        
        // Update position based on velocity
        world->transforms.pos_x[e] += world->velocities.vel_x[e] * delta_time;
        world->transforms.pos_y[e] += world->velocities.vel_y[e] * delta_time;
        
        // Update rotation based on angular velocity
        world->transforms.rotation[e] += world->velocities.angular_vel[e] * delta_time;
        
        // Wrap rotation to 0-360
        world->transforms.rotation[e] = math_wrap_angle_360(world->transforms.rotation[e]);
    }
}

// Note: Ship physics and AI systems are now in game layer:
// - ship_ecs_system_physics() in game/src/game_ship_ecs.c
// - ai_ecs_system_update() in game/src/game_ai_ecs.c
