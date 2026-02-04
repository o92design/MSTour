#include <gtest/gtest.h>
#include "results_screen.h"
#include <cstring>

// =============================================================================
// Results Screen Unit Tests
// =============================================================================

class ResultsTests : public ::testing::Test {
protected:
    void SetUp() override {
        results_screen_init();
    }
    
    void TearDown() override {
        results_screen_shutdown();
    }
};

// =============================================================================
// Test 1: Grade Calculation - Perfect
// =============================================================================

TEST_F(ResultsTests, GradeCalculationPerfect) {
    VoyageGrade grade = calculate_grade(0.98f);
    EXPECT_EQ(grade, GRADE_PERFECT);
    
    grade = calculate_grade(0.95f);
    EXPECT_EQ(grade, GRADE_PERFECT);
}

// =============================================================================
// Test 2: Grade Calculation - Excellent
// =============================================================================

TEST_F(ResultsTests, GradeCalculationExcellent) {
    VoyageGrade grade = calculate_grade(0.88f);
    EXPECT_EQ(grade, GRADE_EXCELLENT);
    
    grade = calculate_grade(0.85f);
    EXPECT_EQ(grade, GRADE_EXCELLENT);
    
    grade = calculate_grade(0.94f);
    EXPECT_EQ(grade, GRADE_EXCELLENT);
}

// =============================================================================
// Test 3: Grade Calculation - Good
// =============================================================================

TEST_F(ResultsTests, GradeCalculationGood) {
    VoyageGrade grade = calculate_grade(0.75f);
    EXPECT_EQ(grade, GRADE_GOOD);
    
    grade = calculate_grade(0.70f);
    EXPECT_EQ(grade, GRADE_GOOD);
    
    grade = calculate_grade(0.84f);
    EXPECT_EQ(grade, GRADE_GOOD);
}

// =============================================================================
// Test 4: Grade Calculation - Fair
// =============================================================================

TEST_F(ResultsTests, GradeCalculationFair) {
    VoyageGrade grade = calculate_grade(0.55f);
    EXPECT_EQ(grade, GRADE_FAIR);
    
    grade = calculate_grade(0.50f);
    EXPECT_EQ(grade, GRADE_FAIR);
    
    grade = calculate_grade(0.69f);
    EXPECT_EQ(grade, GRADE_FAIR);
}

// =============================================================================
// Test 5: Grade Calculation - Poor
// =============================================================================

TEST_F(ResultsTests, GradeCalculationPoor) {
    VoyageGrade grade = calculate_grade(0.30f);
    EXPECT_EQ(grade, GRADE_POOR);
    
    grade = calculate_grade(0.0f);
    EXPECT_EQ(grade, GRADE_POOR);
    
    grade = calculate_grade(0.49f);
    EXPECT_EQ(grade, GRADE_POOR);
}

// =============================================================================
// Test 6: Star Rating Mapping
// =============================================================================

TEST_F(ResultsTests, StarRatingMapping) {
    EXPECT_EQ(get_star_rating(0.98f), 5);
    EXPECT_EQ(get_star_rating(0.95f), 5);
    
    EXPECT_EQ(get_star_rating(0.87f), 4);
    EXPECT_EQ(get_star_rating(0.85f), 4);
    
    EXPECT_EQ(get_star_rating(0.75f), 3);
    EXPECT_EQ(get_star_rating(0.70f), 3);
    
    EXPECT_EQ(get_star_rating(0.55f), 2);
    EXPECT_EQ(get_star_rating(0.50f), 2);
    
    EXPECT_EQ(get_star_rating(0.30f), 1);
    EXPECT_EQ(get_star_rating(0.25f), 1);
    
    EXPECT_EQ(get_star_rating(0.10f), 0);
    EXPECT_EQ(get_star_rating(0.0f), 0);
}

// =============================================================================
// Test 7: Duration Formatting
// =============================================================================

TEST_F(ResultsTests, DurationFormatting) {
    char buffer[32];
    
    format_duration(92.5f, buffer);
    EXPECT_STREQ(buffer, "1m 32s");
    
    format_duration(605.0f, buffer);
    EXPECT_STREQ(buffer, "10m 05s");
    
    format_duration(45.0f, buffer);
    EXPECT_STREQ(buffer, "0m 45s");
    
    format_duration(3661.0f, buffer);
    EXPECT_STREQ(buffer, "61m 01s");
    
    format_duration(0.0f, buffer);
    EXPECT_STREQ(buffer, "0m 00s");
}

// =============================================================================
// Test 8: Show and Hide
// =============================================================================

TEST_F(ResultsTests, ShowAndHide) {
    EXPECT_FALSE(results_screen_is_visible());
    
    float scores[] = {0.95f, 0.82f};
    results_screen_show("Test Route", scores, 2, 452.5f);
    
    EXPECT_TRUE(results_screen_is_visible());
    
    results_screen_hide();
    
    EXPECT_FALSE(results_screen_is_visible());
}

// =============================================================================
// Test 9: Average Satisfaction Calculation
// =============================================================================

TEST_F(ResultsTests, AverageSatisfactionCalculation) {
    float scores[] = {0.85f, 0.92f, 0.78f};
    results_screen_show("Test", scores, 3, 300.0f);
    
    const ResultsScreenState* state = results_screen_get_state();
    
    // Average should be (0.85 + 0.92 + 0.78) / 3 = 0.85
    EXPECT_NEAR(state->average_satisfaction, 0.85f, 0.01f);
}

// =============================================================================
// Test 10: State Populated Correctly
// =============================================================================

TEST_F(ResultsTests, StatePopulatedCorrectly) {
    float scores[] = {0.88f, 0.91f};
    results_screen_show("Vinga Lighthouse Tour", scores, 2, 425.0f);
    
    const ResultsScreenState* state = results_screen_get_state();
    
    ASSERT_NE(state, nullptr);
    EXPECT_TRUE(state->visible);
    EXPECT_STREQ(state->route_name, "Vinga Lighthouse Tour");
    EXPECT_EQ(state->num_pois, 2);
    EXPECT_NE(state->poi_scores, nullptr);
    EXPECT_FLOAT_EQ(state->poi_scores[0], 0.88f);
    EXPECT_FLOAT_EQ(state->poi_scores[1], 0.91f);
    EXPECT_NEAR(state->average_satisfaction, 0.895f, 0.01f);
    EXPECT_EQ(state->grade, GRADE_EXCELLENT);
    EXPECT_EQ(state->star_rating, 4);
    EXPECT_FLOAT_EQ(state->duration, 425.0f);
}

// =============================================================================
// Test 11: Edge Case - Zero POIs
// =============================================================================

TEST_F(ResultsTests, ZeroPOIsEdgeCase) {
    results_screen_show("Empty Route", nullptr, 0, 100.0f);
    
    const ResultsScreenState* state = results_screen_get_state();
    
    EXPECT_TRUE(state->visible);
    EXPECT_EQ(state->num_pois, 0);
    EXPECT_EQ(state->poi_scores, nullptr);
    EXPECT_FLOAT_EQ(state->average_satisfaction, 0.0f);
    EXPECT_EQ(state->grade, GRADE_POOR);
    EXPECT_EQ(state->star_rating, 0);
}

// =============================================================================
// Test 12: Memory Management
// =============================================================================

TEST_F(ResultsTests, MemoryManagement) {
    // Show multiple times to test reallocation
    float scores1[] = {0.8f};
    results_screen_show("Route 1", scores1, 1, 100.0f);
    
    float scores2[] = {0.9f, 0.85f, 0.92f};
    results_screen_show("Route 2", scores2, 3, 200.0f);
    
    const ResultsScreenState* state = results_screen_get_state();
    EXPECT_EQ(state->num_pois, 3);
    EXPECT_NE(state->poi_scores, nullptr);
    
    // Shutdown should not crash
    results_screen_shutdown();
    
    // Re-init for proper teardown
    results_screen_init();
}

// =============================================================================
// Test 13: Duration String Formatting
// =============================================================================

TEST_F(ResultsTests, DurationStringInState) {
    float scores[] = {0.5f};
    results_screen_show("Test", scores, 1, 125.0f);
    
    const ResultsScreenState* state = results_screen_get_state();
    
    // Should format as "2m 05s"
    EXPECT_STREQ(state->duration_str, "2m 05s");
}
