#include <gtest/gtest.h>

extern "C" {
    #include "engine_core.h"
    #include "engine_renderer.h"
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
