#include <gtest/gtest.h>

extern "C" {
    #include "engine_core.h"
    #include "engine_renderer.h"
    #include "engine_math.h"
    #include "engine_ecs.h"
    #include "game_ship_ecs.h"
    #include "game_ai_ecs.h"
}

#include <raylib.h>

// Test engine state structure
TEST(EngineTests, StateStructSize) {
    // Ensure engine state structure is reasonable size
    EXPECT_LT(sizeof(EngineState), 1024);
}

// Test Raylib color usage
TEST(RendererTests, RaylibColors) {
    // Test that Raylib colors are accessible
    EXPECT_EQ(BLACK.r, 0);
    EXPECT_EQ(BLACK.g, 0);
    EXPECT_EQ(BLACK.b, 0);
    
    EXPECT_EQ(WHITE.r, 255);
    EXPECT_EQ(WHITE.g, 255);
    EXPECT_EQ(WHITE.b, 255);
    
    EXPECT_EQ(RED.r, 230);
    EXPECT_EQ(RED.g, 41);
    EXPECT_EQ(RED.b, 55);
}

// Test engine config structure
TEST(EngineTests, ConfigStruct) {
    EngineConfig config;
    config.window_title = "Test";
    config.window_width = 800;
    config.window_height = 600;
    config.target_fps = 60;
    
    EXPECT_STREQ(config.window_title, "Test");
    EXPECT_EQ(config.window_width, 800);
    EXPECT_EQ(config.window_height, 600);
    EXPECT_EQ(config.target_fps, 60);
}

// =============================================================================
// Math Utility Tests
// =============================================================================

TEST(MathTests, Lerp) {
    EXPECT_FLOAT_EQ(math_lerp(0.0f, 10.0f, 0.5f), 5.0f);
    EXPECT_FLOAT_EQ(math_lerp(0.0f, 10.0f, 0.0f), 0.0f);
    EXPECT_FLOAT_EQ(math_lerp(0.0f, 10.0f, 1.0f), 10.0f);
    EXPECT_FLOAT_EQ(math_lerp(-5.0f, 5.0f, 0.5f), 0.0f);
}

TEST(MathTests, Clamp) {
    EXPECT_FLOAT_EQ(math_clamp(5.0f, 0.0f, 10.0f), 5.0f);
    EXPECT_FLOAT_EQ(math_clamp(-5.0f, 0.0f, 10.0f), 0.0f);
    EXPECT_FLOAT_EQ(math_clamp(15.0f, 0.0f, 10.0f), 10.0f);
}

TEST(MathTests, DegToRad) {
    EXPECT_NEAR(math_deg_to_rad(0.0f), 0.0f, 0.0001f);
    EXPECT_NEAR(math_deg_to_rad(90.0f), PI / 2.0f, 0.0001f);
    EXPECT_NEAR(math_deg_to_rad(180.0f), PI, 0.0001f);
    EXPECT_NEAR(math_deg_to_rad(360.0f), 2.0f * PI, 0.0001f);
}

TEST(MathTests, WrapAngle360) {
    EXPECT_FLOAT_EQ(math_wrap_angle_360(0.0f), 0.0f);
    EXPECT_FLOAT_EQ(math_wrap_angle_360(360.0f), 0.0f);
    EXPECT_FLOAT_EQ(math_wrap_angle_360(450.0f), 90.0f);
    EXPECT_FLOAT_EQ(math_wrap_angle_360(-90.0f), 270.0f);
}

TEST(MathTests, MoveToward) {
    // Moving toward target when far away
    EXPECT_FLOAT_EQ(math_move_toward(0.0f, 10.0f, 3.0f), 3.0f);
    
    // Moving toward target when close
    EXPECT_FLOAT_EQ(math_move_toward(9.0f, 10.0f, 3.0f), 10.0f);
    
    // Moving toward negative target
    EXPECT_FLOAT_EQ(math_move_toward(0.0f, -10.0f, 3.0f), -3.0f);
}

// =============================================================================
// ECS Tests
// =============================================================================

TEST(ECSTests, WorldInit) {
    ECSWorld world;
    ecs_world_init(&world);
    
    EXPECT_EQ(ecs_get_entity_count(&world), 0u);
}

TEST(ECSTests, CreateEntity) {
    ECSWorld world;
    ecs_world_init(&world);
    
    Entity e1 = ecs_create_entity(&world);
    EXPECT_NE(e1, INVALID_ENTITY);
    EXPECT_EQ(ecs_get_entity_count(&world), 1u);
    
    Entity e2 = ecs_create_entity(&world);
    EXPECT_NE(e2, INVALID_ENTITY);
    EXPECT_NE(e1, e2);
    EXPECT_EQ(ecs_get_entity_count(&world), 2u);
}

TEST(ECSTests, DestroyEntity) {
    ECSWorld world;
    ecs_world_init(&world);
    
    Entity e1 = ecs_create_entity(&world);
    ecs_add_component(&world, e1, COMPONENT_TRANSFORM);
    EXPECT_EQ(ecs_get_entity_count(&world), 1u);
    
    ecs_destroy_entity(&world, e1);
    EXPECT_EQ(ecs_get_entity_count(&world), 0u);
}

TEST(ECSTests, AddRemoveComponents) {
    ECSWorld world;
    ecs_world_init(&world);
    
    Entity e = ecs_create_entity(&world);
    
    // Add transform
    ecs_add_component(&world, e, COMPONENT_TRANSFORM);
    EXPECT_TRUE(ecs_has_component(&world, e, COMPONENT_TRANSFORM));
    EXPECT_FALSE(ecs_has_component(&world, e, COMPONENT_VELOCITY));
    
    // Add velocity
    ecs_add_component(&world, e, COMPONENT_VELOCITY);
    EXPECT_TRUE(ecs_has_component(&world, e, COMPONENT_TRANSFORM));
    EXPECT_TRUE(ecs_has_component(&world, e, COMPONENT_VELOCITY));
    
    // Remove transform
    ecs_remove_component(&world, e, COMPONENT_TRANSFORM);
    EXPECT_FALSE(ecs_has_component(&world, e, COMPONENT_TRANSFORM));
    EXPECT_TRUE(ecs_has_component(&world, e, COMPONENT_VELOCITY));
}

TEST(ECSTests, HasComponents) {
    ECSWorld world;
    ecs_world_init(&world);
    
    Entity e = ecs_create_entity(&world);
    ecs_add_component(&world, e, COMPONENT_TRANSFORM);
    ecs_add_component(&world, e, COMPONENT_VELOCITY);
    ecs_add_component(&world, e, COMPONENT_GAME_0);  // COMPONENT_SHIP in game layer
    
    // Has all three
    ComponentMask ship_mask = COMPONENT_TRANSFORM | COMPONENT_VELOCITY | COMPONENT_GAME_0;
    EXPECT_TRUE(ecs_has_components(&world, e, ship_mask));
    
    // Has subset
    ComponentMask move_mask = COMPONENT_TRANSFORM | COMPONENT_VELOCITY;
    EXPECT_TRUE(ecs_has_components(&world, e, move_mask));
    
    // Doesn't have GAME_1 (AI in game layer)
    ComponentMask ai_mask = COMPONENT_TRANSFORM | COMPONENT_GAME_1;
    EXPECT_FALSE(ecs_has_components(&world, e, ai_mask));
}

TEST(ECSTests, SetPosition) {
    ECSWorld world;
    ecs_world_init(&world);
    
    Entity e = ecs_create_entity(&world);
    ecs_add_component(&world, e, COMPONENT_TRANSFORM);
    
    ecs_set_position(&world, e, 100.0f, 200.0f);
    
    EXPECT_FLOAT_EQ(world.transforms.pos_x[e], 100.0f);
    EXPECT_FLOAT_EQ(world.transforms.pos_y[e], 200.0f);
}

TEST(ECSTests, CountWith) {
    ECSWorld world;
    ecs_world_init(&world);
    
    // Create 5 entities with transform
    for (int i = 0; i < 5; i++) {
        Entity e = ecs_create_entity(&world);
        ecs_add_component(&world, e, COMPONENT_TRANSFORM);
    }
    
    // Create 3 entities with transform and velocity
    for (int i = 0; i < 3; i++) {
        Entity e = ecs_create_entity(&world);
        ecs_add_component(&world, e, COMPONENT_TRANSFORM);
        ecs_add_component(&world, e, COMPONENT_VELOCITY);
    }
    
    EXPECT_EQ(ecs_count_with(&world, COMPONENT_TRANSFORM), 8u);
    EXPECT_EQ(ecs_count_with(&world, COMPONENT_VELOCITY), 3u);
    EXPECT_EQ(ecs_count_with(&world, COMPONENT_TRANSFORM | COMPONENT_VELOCITY), 3u);
}

TEST(ECSTests, MovementSystem) {
    ECSWorld world;
    ecs_world_init(&world);
    
    Entity e = ecs_create_entity(&world);
    ecs_add_component(&world, e, COMPONENT_TRANSFORM);
    ecs_add_component(&world, e, COMPONENT_VELOCITY);
    
    // Set initial position and velocity
    ecs_set_position(&world, e, 0.0f, 0.0f);
    ecs_set_velocity(&world, e, 10.0f, 5.0f);
    
    // Run movement system for 1 second
    ecs_system_movement(&world, 1.0f);
    
    EXPECT_FLOAT_EQ(world.transforms.pos_x[e], 10.0f);
    EXPECT_FLOAT_EQ(world.transforms.pos_y[e], 5.0f);
}

TEST(ECSTests, ShipConfigSetup) {
    ECSWorld world;
    ShipEcsWorld ship_world;
    ecs_world_init(&world);
    ship_ecs_init(&ship_world);
    
    Entity e = ecs_create_entity(&world);
    ecs_add_component(&world, e, COMPONENT_TRANSFORM);
    ecs_add_component(&world, e, COMPONENT_VELOCITY);
    ecs_add_component(&world, e, COMPONENT_SHIP);  // COMPONENT_GAME_0
    
    // Get default config and apply
    ShipEcsConfig config = ship_ecs_get_default_config();
    ship_ecs_set_config(&ship_world, e, &config);
    
    // Verify config values were set in ship world
    EXPECT_FLOAT_EQ(ship_world.ships.max_speed[e], 150.0f);
    EXPECT_FLOAT_EQ(ship_world.ships.acceleration[e], 45.0f);
    EXPECT_FLOAT_EQ(ship_world.ships.turn_rate[e], 40.0f);
    EXPECT_FLOAT_EQ(ship_world.ships.throttle_response[e], 0.4f);
    EXPECT_FLOAT_EQ(ship_world.ships.steering_response[e], 0.25f);
    EXPECT_FLOAT_EQ(ship_world.ships.coast_friction[e], 0.015f);
    EXPECT_FLOAT_EQ(ship_world.ships.drift_factor[e], 0.25f);
    EXPECT_FLOAT_EQ(ship_world.ships.reverse_speed_mult[e], 0.35f);
    EXPECT_FLOAT_EQ(ship_world.ships.reverse_accel_mult[e], 0.5f);
    EXPECT_FLOAT_EQ(ship_world.ships.speed_turn_factor[e], 0.65f);
}

TEST(ECSTests, ShipPhysicsUsesConfig) {
    ECSWorld world;
    ShipEcsWorld ship_world;
    ecs_world_init(&world);
    ship_ecs_init(&ship_world);
    
    Entity e = ecs_create_entity(&world);
    ecs_add_component(&world, e, COMPONENT_TRANSFORM);
    ecs_add_component(&world, e, COMPONENT_VELOCITY);
    ecs_add_component(&world, e, COMPONENT_SHIP);  // COMPONENT_GAME_0
    
    // Set custom config with different values
    ShipEcsConfig config;
    config.max_speed = 100.0f;      // Different from default 150
    config.acceleration = 20.0f;    // Different from default 45
    config.turn_rate = 30.0f;
    config.throttle_response = 0.2f;  // Faster response
    config.steering_response = 0.1f;
    config.coast_friction = 0.02f;
    config.drift_factor = 0.1f;
    config.reverse_speed_mult = 0.5f;
    config.reverse_accel_mult = 0.6f;
    config.speed_turn_factor = 0.5f;
    ship_ecs_set_config(&ship_world, e, &config);
    
    // Set throttle and run physics
    ship_ecs_set_throttle(&ship_world, e, 1.0f);
    
    // Run for several frames to accelerate
    for (int i = 0; i < 300; i++) {  // 5 seconds at 60fps
        ship_ecs_system_physics(&world, &ship_world, 1.0f / 60.0f);
        ecs_system_movement(&world, 1.0f / 60.0f);
    }
    
    // Speed should approach custom max_speed (100), not default (150)
    EXPECT_GT(world.velocities.speed[e], 90.0f);  // Should be near 100
    EXPECT_LT(world.velocities.speed[e], 110.0f); // But not near 150
}

TEST(ECSTests, WorldClear) {
    ECSWorld world;
    ecs_world_init(&world);
    
    // Create some entities
    for (int i = 0; i < 10; i++) {
        Entity e = ecs_create_entity(&world);
        ecs_add_component(&world, e, COMPONENT_TRANSFORM);
    }
    
    EXPECT_EQ(ecs_get_entity_count(&world), 10u);
    
    ecs_world_clear(&world);
    
    EXPECT_EQ(ecs_get_entity_count(&world), 0u);
}
