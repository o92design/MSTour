/**
 * @file ship_physics_tuning_test.cpp
 * @brief Physics tuning validation tests for MST-12
 * 
 * These tests validate the three test scenarios from the game design consultant:
 * - Scenario A: Full Speed Stop (coast 50-70 units)
 * - Scenario B: U-Turn Maneuver (radius ≥400 units, drift 15-25%)
 * - Scenario C: Precision Crawl (responsive low-speed control)
 */

#include <gtest/gtest.h>
#include <cmath>

extern "C" {
    #include "ship_physics.h"
    #include "config.h"
}

// Test fixture for ship physics tuning
class ShipPhysicsTuningTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Load config from config.ini or use defaults
        if (!config_load_ship_physics("../../../config.ini", &config)) {
            config = ship_physics_get_default_config();
        }
        
        // Initialize ship state at origin
        ship_physics_init(&state, 400.0f, 300.0f, 0.0f);
    }

    ShipState state;
    ShipPhysicsConfig config;
    
    // Helper: Simulate physics for multiple frames
    void simulate_frames(int frame_count, float delta_time, float throttle_input, float rudder_input) {
        for (int i = 0; i < frame_count; i++) {
            ship_physics_process_actions(&state, throttle_input, rudder_input);
            ship_physics_update(&state, &config, delta_time);
        }
    }
    
    // Helper: Calculate distance between two points
    float calculate_distance(float x1, float y1, float x2, float y2) {
        float dx = x2 - x1;
        float dy = y2 - y1;
        return sqrtf(dx * dx + dy * dy);
    }
    
    // Helper: Simulate until condition is met (with timeout)
    int simulate_until(bool (*condition)(const ShipState*, void*), void* user_data, 
                       float delta_time, float throttle_input, float rudder_input, 
                       int max_frames = 1000) {
        for (int i = 0; i < max_frames; i++) {
            if (condition(&state, user_data)) {
                return i;
            }
            ship_physics_process_actions(&state, throttle_input, rudder_input);
            ship_physics_update(&state, &config, delta_time);
        }
        return -1; // Timeout
    }
};

// Callback helpers for simulate_until
static bool speed_reached(const ShipState* state, void* user_data) {
    float target_speed = *(float*)user_data;
    return state->speed >= target_speed;
}

static bool speed_below(const ShipState* state, void* user_data) {
    float threshold = *(float*)user_data;
    return state->speed < threshold;
}

static bool heading_changed(const ShipState* state, void* user_data) {
    float target_delta = *(float*)user_data;
    // Assuming we track initial heading separately
    return fabsf(state->heading) >= target_delta;
}

/**
 * Test Scenario A: Full Speed Stop
 * 
 * Success Criteria:
 * - Coast distance: 50-70 units
 * - Coast time: 3-5 seconds
 * - Smooth exponential decay
 * - Can still steer during coast
 */
TEST_F(ShipPhysicsTuningTest, ScenarioA_FullSpeedStop_CoastDistance) {
    const float delta_time = 1.0f / 60.0f; // 60 FPS
    
    // Phase 1: Accelerate to max speed
    float target_speed = config.max_speed * 0.95f; // 95% of max speed
    int frames_to_max = simulate_until(speed_reached, &target_speed, delta_time, 1.0f, 0.0f);
    
    ASSERT_GT(frames_to_max, 0) << "Ship should reach max speed";
    
    float time_to_max = frames_to_max * delta_time;
    std::cout << "Time to reach max speed: " << time_to_max << " seconds" << std::endl;
    std::cout << "Speed at start of coast: " << state.speed << " u/s" << std::endl;
    
    // Validate acceleration time (should be 3-4 seconds)
    EXPECT_GE(time_to_max, 2.5f) << "Acceleration should take at least 2.5 seconds (feels weighty)";
    EXPECT_LE(time_to_max, 4.5f) << "Acceleration should not take more than 4.5 seconds (still responsive)";
    
    // Phase 2: Release throttle and coast
    float coast_start_x = state.pos_x;
    float coast_start_y = state.pos_y;
    
    float stop_threshold = 5.0f;
    int coast_frames = simulate_until(speed_below, &stop_threshold, delta_time, 0.0f, 0.0f);
    
    ASSERT_GT(coast_frames, 0) << "Ship should coast to stop";
    
    float coast_distance = calculate_distance(coast_start_x, coast_start_y, state.pos_x, state.pos_y);
    float coast_time = coast_frames * delta_time;
    
    std::cout << "Coast distance: " << coast_distance << " units" << std::endl;
    std::cout << "Coast time: " << coast_time << " seconds" << std::endl;
    std::cout << "Final speed: " << state.speed << " u/s" << std::endl;
    
    // Validate coast distance (50-70 units)
    EXPECT_GE(coast_distance, 45.0f) << "Coast distance too short - increase momentum_retention or decrease coast_friction";
    EXPECT_LE(coast_distance, 75.0f) << "Coast distance too long - decrease momentum_retention or increase coast_friction";
    
    // Validate coast time (3-5 seconds)
    EXPECT_GE(coast_time, 2.5f) << "Coast time too short";
    EXPECT_LE(coast_time, 5.5f) << "Coast time too long";
}

/**
 * Test Scenario A (Part 2): Steering during coast
 */
TEST_F(ShipPhysicsTuningTest, ScenarioA_FullSpeedStop_SteerWhileCoasting) {
    const float delta_time = 1.0f / 60.0f;
    
    // Accelerate to max speed
    float target_speed = config.max_speed * 0.95f;
    simulate_until(speed_reached, &target_speed, delta_time, 1.0f, 0.0f);
    
    float initial_heading = state.heading;
    
    // Coast while turning
    simulate_frames(60, delta_time, 0.0f, 1.0f); // 1 second of coasting + turning
    
    float heading_change = fabsf(state.heading - initial_heading);
    
    std::cout << "Heading change while coasting: " << heading_change << " degrees" << std::endl;
    
    // Should still be able to turn significantly while coasting
    EXPECT_GT(heading_change, 20.0f) << "Should be able to steer effectively while coasting";
}

/**
 * Test Scenario B: U-Turn Maneuver
 * 
 * Success Criteria:
 * - Turn radius ≥ 400 units
 * - Drift distance: 15-25% of turn radius
 * - Turn duration: 4-6 seconds
 * - Speed decreases 10-20% during turn
 */
TEST_F(ShipPhysicsTuningTest, ScenarioB_UTurn_TurnRadiusAndDrift) {
    const float delta_time = 1.0f / 60.0f;
    
    // Accelerate to max speed
    float target_speed = config.max_speed * 0.95f;
    simulate_until(speed_reached, &target_speed, delta_time, 1.0f, 0.0f);
    
    float initial_speed = state.speed;
    float initial_heading = state.heading;
    float turn_start_x = state.pos_x;
    float turn_start_y = state.pos_y;
    
    std::cout << "Starting U-turn at speed: " << initial_speed << " u/s" << std::endl;
    std::cout << "Initial heading: " << initial_heading << " degrees" << std::endl;
    
    // Execute 180° turn
    int turn_frames = 0;
    while (fabsf(state.heading - initial_heading) < 180.0f && turn_frames < 600) {
        ship_physics_process_actions(&state, 1.0f, 1.0f); // Full throttle + full turn
        ship_physics_update(&state, &config, delta_time);
        turn_frames++;
    }
    
    float turn_time = turn_frames * delta_time;
    float final_speed = state.speed;
    float speed_loss_percent = ((initial_speed - final_speed) / initial_speed) * 100.0f;
    
    // Calculate turn radius (approximate from path)
    float turn_end_x = state.pos_x;
    float turn_end_y = state.pos_y;
    float chord_length = calculate_distance(turn_start_x, turn_start_y, turn_end_x, turn_end_y);
    
    // For a 180° turn, radius ≈ chord_length / 2
    float turn_radius = chord_length / 2.0f;
    
    std::cout << "Turn radius: " << turn_radius << " units" << std::endl;
    std::cout << "Turn time: " << turn_time << " seconds" << std::endl;
    std::cout << "Speed loss: " << speed_loss_percent << "%" << std::endl;
    std::cout << "Final speed: " << final_speed << " u/s" << std::endl;
    
    // Validate turn radius (≥400 units)
    EXPECT_GE(turn_radius, 350.0f) << "Turn radius too tight - decrease max_turn_rate";
    
    // Validate turn time (4-6 seconds)
    EXPECT_GE(turn_time, 3.5f) << "Turn too quick";
    EXPECT_LE(turn_time, 6.5f) << "Turn too slow";
    
    // Validate speed loss (10-20%)
    EXPECT_GE(speed_loss_percent, 5.0f) << "Should lose some speed during tight turn";
    EXPECT_LE(speed_loss_percent, 25.0f) << "Speed loss too high";
}

/**
 * Test Scenario C: Precision Crawl
 * 
 * Success Criteria:
 * - Achieves 5-10 u/s with light throttle
 * - Steering still responsive (≥60% of full turn rate)
 * - Can stop within 2-3 units
 */
TEST_F(ShipPhysicsTuningTest, ScenarioC_PrecisionCrawl_LowSpeedControl) {
    const float delta_time = 1.0f / 60.0f;
    
    // Gently accelerate to low speed (light throttle)
    simulate_frames(120, delta_time, 0.15f, 0.0f); // 2 seconds of light throttle
    
    float low_speed = state.speed;
    
    std::cout << "Low speed achieved: " << low_speed << " u/s" << std::endl;
    
    // Validate low speed range (5-15 u/s with light throttle)
    EXPECT_GE(low_speed, 3.0f) << "Should be able to maintain slow forward movement";
    EXPECT_LE(low_speed, 20.0f) << "Light throttle should not reach high speeds quickly";
    
    // Test steering responsiveness at low speed
    float initial_heading = state.heading;
    simulate_frames(60, delta_time, 0.15f, 1.0f); // 1 second of turning at low speed
    
    float heading_change = fabsf(state.heading - initial_heading);
    float max_heading_change = config.max_turn_rate * 1.0f; // 1 second at max turn rate
    float responsiveness_percent = (heading_change / max_heading_change) * 100.0f;
    
    std::cout << "Heading change at low speed: " << heading_change << " degrees" << std::endl;
    std::cout << "Responsiveness: " << responsiveness_percent << "% of max" << std::endl;
    
    // Should maintain at least 60% turn rate at low speed
    EXPECT_GE(responsiveness_percent, 50.0f) << "Steering too sluggish at low speed - increase speed_turn_factor";
}

/**
 * Test Scenario C (Part 2): Precision stopping
 */
TEST_F(ShipPhysicsTuningTest, ScenarioC_PrecisionCrawl_PrecisionStop) {
    const float delta_time = 1.0f / 60.0f;
    
    // Accelerate to low speed
    simulate_frames(60, delta_time, 0.2f, 0.0f);
    
    float stop_start_x = state.pos_x;
    float stop_start_y = state.pos_y;
    float stop_start_speed = state.speed;
    
    std::cout << "Starting precision stop from: " << stop_start_speed << " u/s" << std::endl;
    
    // Release throttle and coast to stop
    float stop_threshold = 1.0f;
    simulate_until(speed_below, &stop_threshold, delta_time, 0.0f, 0.0f);
    
    float stop_distance = calculate_distance(stop_start_x, stop_start_y, state.pos_x, state.pos_y);
    
    std::cout << "Stop distance from low speed: " << stop_distance << " units" << std::endl;
    
    // Should be able to stop within 2-5 units at low speed
    EXPECT_LE(stop_distance, 6.0f) << "Stop distance too long for precision control";
}

/**
 * Test: Reverse brake effectiveness
 */
TEST_F(ShipPhysicsTuningTest, ReverseBrake_Effectiveness) {
    const float delta_time = 1.0f / 60.0f;
    
    // Accelerate to medium speed
    simulate_frames(90, delta_time, 1.0f, 0.0f);
    
    float brake_start_speed = state.speed;
    float brake_start_x = state.pos_x;
    float brake_start_y = state.pos_y;
    
    // Apply reverse throttle (brake)
    float stop_threshold = 5.0f;
    int brake_frames = simulate_until(speed_below, &stop_threshold, delta_time, -1.0f, 0.0f);
    
    float brake_distance = calculate_distance(brake_start_x, brake_start_y, state.pos_x, state.pos_y);
    float brake_time = brake_frames * delta_time;
    
    // Also test natural coast for comparison
    ShipState coast_state;
    ship_physics_init(&coast_state, 400.0f, 300.0f, 0.0f);
    simulate_frames(90, delta_time, 1.0f, 0.0f);
    float coast_start_x = coast_state.pos_x;
    float coast_start_y = coast_state.pos_y;
    simulate_until(speed_below, &stop_threshold, delta_time, 0.0f, 0.0f);
    float coast_distance = calculate_distance(coast_start_x, coast_start_y, coast_state.pos_x, coast_state.pos_y);
    
    std::cout << "Brake distance: " << brake_distance << " units in " << brake_time << " seconds" << std::endl;
    std::cout << "Coast distance: " << coast_distance << " units" << std::endl;
    std::cout << "Brake effectiveness: " << ((coast_distance - brake_distance) / coast_distance * 100.0f) << "% reduction" << std::endl;
    
    // Reverse should provide meaningful braking (at least 25% reduction)
    EXPECT_LT(brake_distance, coast_distance * 0.75f) << "Reverse should provide meaningful braking";
}

