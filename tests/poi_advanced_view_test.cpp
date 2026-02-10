#include <gtest/gtest.h>
#include <cmath>

extern "C" {
#include "game_poi_ecs.h"
#include "engine_ecs.h"
}

class POIViewTest : public ::testing::Test {
protected:
    POIEcsWorld poi_world;
    
    void SetUp() override {
        poi_ecs_init(&poi_world);
    }
    
    void TearDown() override {
        poi_ecs_shutdown(&poi_world);
    }
    
    // Helper to create a simple POI
    int create_poi(float x, float y, float radius = 50.0f) {
        POICreateParams params = {0};
        params.name = "Test POI";
        params.x = x;
        params.y = y;
        params.radius = radius;
        return poi_ecs_create(&poi_world, &params);
    }
};

TEST_F(POIViewTest, OutOfRange) {
    int idx = create_poi(100.0f, 100.0f, 50.0f);
    
    // Ship at 500, 500 (far away)
    POIViewStatus status = poi_ecs_get_view_status(&poi_world, idx, 500.0f, 500.0f, 0.0f, 0.0f);
    
    EXPECT_FALSE(status.in_range);
    EXPECT_FALSE(status.is_viewable);
    
    // Angle is now calculated always.
    // At 500,500 facing 0 (East), ship is at SE of POI (100,100).
    // Vector to POI is NW (-400, -400).
    // Forward is East (1,0).
    // Dot product is negative (pointing awayish).
    // abs(dot) should be around 0.707 (45 deg).
    // Just ensure is_viewable is still false.
}

TEST_F(POIViewTest, InRangeButTooFast) {
    int idx = create_poi(100.0f, 100.0f, 50.0f);
    
    // Ship at 100, 200 (distance 100, within 4x radius view range)
    // Speed 100 (too fast, max is 60)
    POIViewStatus status = poi_ecs_get_view_status(&poi_world, idx, 100.0f, 200.0f, 0.0f, 100.0f);
    
    EXPECT_TRUE(status.in_range);
    EXPECT_FALSE(status.speed_ok);
    EXPECT_FALSE(status.is_viewable);
}

TEST_F(POIViewTest, InRangeGoodSpeedBadAngle) {
    int idx = create_poi(100.0f, 100.0f, 50.0f);
    
    // Ship at 100, 200 (South of POI)
    // Facing North (Rotation 270 degrees)
    // Vector to POI is (0, -1) which is North.
    // Ship forward is North.
    // Dot product should be 1.0 (Head On). Bad.
    
    POIViewStatus status = poi_ecs_get_view_status(&poi_world, idx, 100.0f, 200.0f, 270.0f, 20.0f);
    
    EXPECT_TRUE(status.in_range);
    EXPECT_TRUE(status.speed_ok);
    // Alignment should be high (bad)
    EXPECT_GT(status.alignment, 0.8f);
    EXPECT_FALSE(status.angle_ok);
    EXPECT_FALSE(status.is_viewable);
}

TEST_F(POIViewTest, InRangeGoodSpeedGoodAngle) {
    int idx = create_poi(100.0f, 100.0f, 50.0f);
    
    // Ship at 100, 200 (South of POI)
    // Facing East (0 degrees). Direction (1, 0).
    // Vector to POI (0, -1).
    // Dot product = 0. Perfect Broadside.
    
    POIViewStatus status = poi_ecs_get_view_status(&poi_world, idx, 100.0f, 200.0f, 0.0f, 20.0f);
    
    EXPECT_TRUE(status.in_range);
    EXPECT_TRUE(status.speed_ok);
    EXPECT_LT(status.alignment, 0.1f);
    EXPECT_TRUE(status.angle_ok);
    EXPECT_TRUE(status.is_viewable);
    EXPECT_GT(status.quality_score, 0.5f);
}

TEST_F(POIViewTest, CollisionVisitStillWorks) {
    // If we crash into the POI (very close), it should still trigger standard behavior
    // This is tested in existing POI tests, but let's confirm view status logic
    int idx = create_poi(100.0f, 100.0f, 50.0f);
    
    // Ship at 110, 110 (Distance 14)
    // Speed High, Bad Angle
    POIViewStatus status = poi_ecs_get_view_status(&poi_world, idx, 110.0f, 110.0f, 270.0f, 100.0f);
    
    // It's not "viewable" per se (too fast/close)
    EXPECT_FALSE(status.is_viewable);
    
    // BUT the system update logic (not tested here directly) handles the "Collision" case separately.
    // Here we just test the view status.
    EXPECT_LT(status.distance, 50.0f);
}
