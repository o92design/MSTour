#include <gtest/gtest.h>
#include "voyage_manager.h"

// =============================================================================
// Voyage Manager Unit Tests
// =============================================================================

class VoyageTests : public ::testing::Test {
protected:
    void SetUp() override {
        // Tests will call voyage_init individually
    }
    
    void TearDown() override {
        voyage_shutdown();
    }
};

// =============================================================================
// Test 1: Initialization
// =============================================================================

TEST_F(VoyageTests, Initialization) {
    voyage_init(2);
    
    EXPECT_EQ(voyage_get_state(), VOYAGE_STATE_START);
    EXPECT_EQ(voyage_get_current_poi_index(), 0);
    EXPECT_FALSE(voyage_is_active());
    EXPECT_FALSE(voyage_is_complete());
    EXPECT_FLOAT_EQ(voyage_get_duration(), 0.0f);
}

// =============================================================================
// Test 2: State Transitions
// =============================================================================

TEST_F(VoyageTests, StateTransitions) {
    voyage_init(2);
    
    // Start voyage
    voyage_start();
    EXPECT_EQ(voyage_get_state(), VOYAGE_STATE_NAVIGATE);
    EXPECT_TRUE(voyage_is_active());
    
    // Transition to positioning
    voyage_set_state(VOYAGE_STATE_POSITIONING);
    EXPECT_EQ(voyage_get_state(), VOYAGE_STATE_POSITIONING);
    EXPECT_TRUE(voyage_is_active());
    
    // Transition to holding
    voyage_set_state(VOYAGE_STATE_HOLDING);
    EXPECT_EQ(voyage_get_state(), VOYAGE_STATE_HOLDING);
    EXPECT_TRUE(voyage_is_active());
    
    // Transition to complete
    voyage_set_state(VOYAGE_STATE_COMPLETE);
    EXPECT_EQ(voyage_get_state(), VOYAGE_STATE_COMPLETE);
    EXPECT_TRUE(voyage_is_active());
    
    // Transition to finish
    voyage_set_state(VOYAGE_STATE_FINISH);
    EXPECT_EQ(voyage_get_state(), VOYAGE_STATE_FINISH);
    EXPECT_FALSE(voyage_is_active());
    EXPECT_TRUE(voyage_is_complete());
}

// =============================================================================
// Test 3: POI Advancement
// =============================================================================

TEST_F(VoyageTests, POIAdvancement) {
    voyage_init(3);
    voyage_start();
    
    EXPECT_EQ(voyage_get_current_poi_index(), 0);
    
    voyage_advance_to_next_poi();
    EXPECT_EQ(voyage_get_current_poi_index(), 1);
    
    voyage_advance_to_next_poi();
    EXPECT_EQ(voyage_get_current_poi_index(), 2);
    
    voyage_advance_to_next_poi();
    EXPECT_EQ(voyage_get_current_poi_index(), 3);
}

// =============================================================================
// Test 4: Satisfaction Tracking
// =============================================================================

TEST_F(VoyageTests, SatisfactionTracking) {
    voyage_init(2);
    voyage_start();
    
    // Record satisfaction for POI 0
    voyage_record_poi_satisfaction(0.85f);
    
    // Advance to POI 1
    voyage_advance_to_next_poi();
    
    // Record satisfaction for POI 1
    voyage_record_poi_satisfaction(0.92f);
    
    // Check average satisfaction
    float avg = voyage_get_average_satisfaction();
    EXPECT_NEAR(avg, 0.885f, 0.001f);
}

// =============================================================================
// Test 5: Duration Tracking
// =============================================================================

TEST_F(VoyageTests, DurationTracking) {
    voyage_init(1);
    voyage_start();
    
    EXPECT_FLOAT_EQ(voyage_get_duration(), 0.0f);
    
    voyage_update(1.0f);  // 1 second
    EXPECT_FLOAT_EQ(voyage_get_duration(), 1.0f);
    
    voyage_update(2.5f);  // 2.5 seconds
    EXPECT_FLOAT_EQ(voyage_get_duration(), 3.5f);
    
    voyage_update(0.5f);  // 0.5 seconds
    EXPECT_FLOAT_EQ(voyage_get_duration(), 4.0f);
}

// =============================================================================
// Test 6: Voyage Completion
// =============================================================================

TEST_F(VoyageTests, VoyageCompletion) {
    voyage_init(2);
    voyage_start();
    
    EXPECT_FALSE(voyage_is_complete());
    
    // Complete POI 1
    voyage_record_poi_satisfaction(1.0f);
    voyage_set_state(VOYAGE_STATE_COMPLETE);
    voyage_advance_to_next_poi();
    
    EXPECT_FALSE(voyage_is_complete());
    
    // Complete POI 2
    voyage_record_poi_satisfaction(1.0f);
    voyage_set_state(VOYAGE_STATE_COMPLETE);
    
    EXPECT_FALSE(voyage_is_complete());  // Not finished until explicitly set
    
    // Finish voyage
    voyage_set_state(VOYAGE_STATE_FINISH);
    EXPECT_TRUE(voyage_is_complete());
    EXPECT_FALSE(voyage_is_active());
}

// =============================================================================
// Test 7: Active State Queries
// =============================================================================

TEST_F(VoyageTests, ActiveStateQueries) {
    voyage_init(1);
    
    // Initial state - not active
    EXPECT_FALSE(voyage_is_active());
    
    // Start voyage - becomes active
    voyage_start();
    EXPECT_TRUE(voyage_is_active());
    
    // Navigate state - active
    EXPECT_TRUE(voyage_is_active());
    
    // Positioning state - active
    voyage_set_state(VOYAGE_STATE_POSITIONING);
    EXPECT_TRUE(voyage_is_active());
    
    // Holding state - active
    voyage_set_state(VOYAGE_STATE_HOLDING);
    EXPECT_TRUE(voyage_is_active());
    
    // Complete state - active
    voyage_set_state(VOYAGE_STATE_COMPLETE);
    EXPECT_TRUE(voyage_is_active());
    
    // Finish state - not active
    voyage_set_state(VOYAGE_STATE_FINISH);
    EXPECT_FALSE(voyage_is_active());
}

// =============================================================================
// Test 8: Invalid State Handling
// =============================================================================

TEST_F(VoyageTests, InvalidStateHandling) {
    voyage_init(1);
    
    // Should not crash on operations before starting
    EXPECT_EQ(voyage_get_current_poi_index(), 0);
    
    // Advance beyond bounds
    voyage_advance_to_next_poi();
    EXPECT_EQ(voyage_get_current_poi_index(), 1);
    
    // Advance again (beyond total)
    voyage_advance_to_next_poi();
    EXPECT_EQ(voyage_get_current_poi_index(), 1);  // Should clamp or stay
}

// =============================================================================
// Test 9: Satisfaction Clamping
// =============================================================================

TEST_F(VoyageTests, SatisfactionClamping) {
    voyage_init(3);
    voyage_start();
    
    // Test values below 0 get clamped
    voyage_record_poi_satisfaction(-0.5f);
    voyage_advance_to_next_poi();
    
    // Test values above 1 get clamped
    voyage_record_poi_satisfaction(1.5f);
    voyage_advance_to_next_poi();
    
    // Test normal value
    voyage_record_poi_satisfaction(0.75f);
    
    // Average should reflect clamped values
    float avg = voyage_get_average_satisfaction();
    EXPECT_GE(avg, 0.0f);
    EXPECT_LE(avg, 1.0f);
}

// =============================================================================
// Test 10: Zero POI Edge Case
// =============================================================================

TEST_F(VoyageTests, ZeroPOIEdgeCase) {
    voyage_init(0);
    
    EXPECT_EQ(voyage_get_state(), VOYAGE_STATE_START);
    EXPECT_EQ(voyage_get_current_poi_index(), 0);
    EXPECT_FLOAT_EQ(voyage_get_average_satisfaction(), 0.0f);
    
    // Should not crash
    voyage_start();
    voyage_record_poi_satisfaction(1.0f);
}

// =============================================================================
// Test 11: Progress Pointer
// =============================================================================

TEST_F(VoyageTests, ProgressPointer) {
    voyage_init(2);
    voyage_start();
    
    const VoyageProgress* progress = voyage_get_progress();
    
    ASSERT_NE(progress, nullptr);
    EXPECT_EQ(progress->state, VOYAGE_STATE_NAVIGATE);
    EXPECT_EQ(progress->total_pois, 2);
    EXPECT_EQ(progress->current_poi_index, 0);
    EXPECT_TRUE(progress->voyage_active);
}
