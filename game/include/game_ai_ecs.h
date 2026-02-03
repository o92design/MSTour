#ifndef GAME_AI_ECS_H
#define GAME_AI_ECS_H

#include "engine_ecs.h"
#include <stdbool.h>

// =============================================================================
// Game AI ECS
// 
// AI-specific ECS components and systems. This is game-layer code that
// extends the engine's generic ECS with AI ship behavior.
// =============================================================================

// AI component flag (uses game-reserved bit from engine)
#define COMPONENT_AI COMPONENT_GAME_1

// =============================================================================
// AI State Enum
// =============================================================================

typedef enum AIState {
    AI_STATE_IDLE = 0,
    AI_STATE_MOVING = 1,
    AI_STATE_WAITING = 2,
    AI_STATE_DOCKING = 3,
    AI_STATE_UNDOCKING = 4,
} AIState;

// =============================================================================
// AI Components (SoA Layout)
// =============================================================================

typedef struct AIComponents {
    uint32_t route_id[MAX_ENTITIES];
    uint32_t waypoint_index[MAX_ENTITIES];
    float wait_timer[MAX_ENTITIES];
    uint8_t ai_state[MAX_ENTITIES];
} AIComponents;

// Global AI component storage
typedef struct AIEcsWorld {
    AIComponents ai;
    bool initialized;
} AIEcsWorld;

// =============================================================================
// AI ECS Lifecycle
// =============================================================================

// Initialize AI ECS world
void ai_ecs_init(AIEcsWorld* ai_world);

// Shutdown AI ECS world
void ai_ecs_shutdown(AIEcsWorld* ai_world);

// =============================================================================
// AI Component Access
// =============================================================================

// Initialize AI for an entity
void ai_ecs_setup(AIEcsWorld* ai_world, Entity e, uint32_t route_id);

// Get current AI state
AIState ai_ecs_get_state(const AIEcsWorld* ai_world, Entity e);

// Set AI state
void ai_ecs_set_state(AIEcsWorld* ai_world, Entity e, AIState state);

// Get current route ID
uint32_t ai_ecs_get_route(const AIEcsWorld* ai_world, Entity e);

// Get current waypoint index
uint32_t ai_ecs_get_waypoint(const AIEcsWorld* ai_world, Entity e);

// Set waypoint index
void ai_ecs_set_waypoint(AIEcsWorld* ai_world, Entity e, uint32_t waypoint);

// =============================================================================
// AI System
// =============================================================================

// Update AI for all entities with COMPONENT_AI
// This handles route following, state transitions, etc.
void ai_ecs_system_update(ECSWorld* ecs_world, AIEcsWorld* ai_world, float delta_time);

#endif // GAME_AI_ECS_H
