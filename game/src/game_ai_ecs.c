#include "game_ai_ecs.h"
#include <string.h>
#include <stdio.h>

// =============================================================================
// AI ECS Lifecycle
// =============================================================================

void ai_ecs_init(AIEcsWorld* ai_world) {
    if (!ai_world) return;
    
    memset(ai_world, 0, sizeof(AIEcsWorld));
    ai_world->initialized = true;
    
    printf("AI ECS: Initialized\n");
}

void ai_ecs_shutdown(AIEcsWorld* ai_world) {
    if (!ai_world) return;
    
    ai_world->initialized = false;
    printf("AI ECS: Shutdown\n");
}

// =============================================================================
// AI Component Access
// =============================================================================

void ai_ecs_setup(AIEcsWorld* ai_world, Entity e, uint32_t route_id) {
    if (!ai_world || e == INVALID_ENTITY || e >= MAX_ENTITIES) return;
    
    ai_world->ai.route_id[e] = route_id;
    ai_world->ai.waypoint_index[e] = 0;
    ai_world->ai.wait_timer[e] = 0.0f;
    ai_world->ai.ai_state[e] = AI_STATE_IDLE;
}

AIState ai_ecs_get_state(const AIEcsWorld* ai_world, Entity e) {
    if (!ai_world || e == INVALID_ENTITY || e >= MAX_ENTITIES) return AI_STATE_IDLE;
    return (AIState)ai_world->ai.ai_state[e];
}

void ai_ecs_set_state(AIEcsWorld* ai_world, Entity e, AIState state) {
    if (!ai_world || e == INVALID_ENTITY || e >= MAX_ENTITIES) return;
    ai_world->ai.ai_state[e] = (uint8_t)state;
}

uint32_t ai_ecs_get_route(const AIEcsWorld* ai_world, Entity e) {
    if (!ai_world || e == INVALID_ENTITY || e >= MAX_ENTITIES) return 0;
    return ai_world->ai.route_id[e];
}

uint32_t ai_ecs_get_waypoint(const AIEcsWorld* ai_world, Entity e) {
    if (!ai_world || e == INVALID_ENTITY || e >= MAX_ENTITIES) return 0;
    return ai_world->ai.waypoint_index[e];
}

void ai_ecs_set_waypoint(AIEcsWorld* ai_world, Entity e, uint32_t waypoint) {
    if (!ai_world || e == INVALID_ENTITY || e >= MAX_ENTITIES) return;
    ai_world->ai.waypoint_index[e] = waypoint;
}

// =============================================================================
// AI System
// =============================================================================

void ai_ecs_system_update(ECSWorld* ecs_world, AIEcsWorld* ai_world, float delta_time) {
    if (!ecs_world || !ai_world) return;
    
    ComponentMask required = COMPONENT_TRANSFORM | COMPONENT_AI;
    
    for (Entity e = 1; e < MAX_ENTITIES; e++) {
        if ((ecs_world->entity_masks[e] & required) != required) continue;
        
        // Update wait timer
        if (ai_world->ai.wait_timer[e] > 0) {
            ai_world->ai.wait_timer[e] -= delta_time;
            continue;
        }
        
        // AI state machine
        AIState state = (AIState)ai_world->ai.ai_state[e];
        
        switch (state) {
            case AI_STATE_IDLE:
                // Start moving if we have a route
                if (ai_world->ai.route_id[e] != 0) {
                    ai_world->ai.ai_state[e] = AI_STATE_MOVING;
                }
                break;
                
            case AI_STATE_MOVING:
                // TODO: Navigation toward waypoint
                // This will be implemented when route system is added
                break;
                
            case AI_STATE_WAITING:
                // Wait complete, resume moving
                ai_world->ai.ai_state[e] = AI_STATE_MOVING;
                break;
                
            case AI_STATE_DOCKING:
                // TODO: Docking behavior
                break;
                
            case AI_STATE_UNDOCKING:
                // TODO: Undocking behavior
                break;
        }
    }
}
