// =============================================================================
// POI System Tests
// Tests for Point of Interest ECS, loader, fog of war, and satisfaction systems
// =============================================================================

#include <gtest/gtest.h>
#include <cstring>

extern "C" {
#include "game_poi_ecs.h"
#include "game_poi_loader.h"
#include "game_fog_of_war.h"
#include "game_satisfaction.h"
}

// Helper to create POI params (C++17 compatible)
static POICreateParams make_poi_params(const char* name, POIType type, POITier tier,
                                        float x, float y, float radius = 0.0f,
                                        int satisfaction_bonus = 0,
                                        const char* description = nullptr) {
    POICreateParams params;
    memset(&params, 0, sizeof(params));
    params.name = name;
    params.description = description;
    params.type = type;
    params.tier = tier;
    params.x = x;
    params.y = y;
    params.radius = radius;
    params.satisfaction_bonus = satisfaction_bonus;
    return params;
}

// =============================================================================
// POI ECS Tests
// =============================================================================

class POIEcsTest : public ::testing::Test {
protected:
    POIEcsWorld poi_world;
    
    void SetUp() override {
        poi_ecs_init(&poi_world);
    }
    
    void TearDown() override {
        poi_ecs_shutdown(&poi_world);
    }
};

TEST_F(POIEcsTest, InitializesCorrectly) {
    EXPECT_TRUE(poi_world.initialized);
    EXPECT_EQ(poi_ecs_get_count(&poi_world), 0u);
}

TEST_F(POIEcsTest, CreatePOI) {
    POICreateParams params = make_poi_params(
        "Test POI", POI_TYPE_HISTORICAL, POI_TIER_SPECIAL,
        100.0f, 200.0f, 50.0f, 15, "A test point of interest"
    );
    
    int idx = poi_ecs_create(&poi_world, &params);
    
    EXPECT_GE(idx, 0);
    EXPECT_EQ(poi_ecs_get_count(&poi_world), 1u);
    EXPECT_STREQ(poi_ecs_get_name(&poi_world, idx), "Test POI");
    EXPECT_EQ(poi_ecs_get_type(&poi_world, idx), POI_TYPE_HISTORICAL);
    EXPECT_EQ(poi_ecs_get_tier(&poi_world, idx), POI_TIER_SPECIAL);
    EXPECT_FLOAT_EQ(poi_ecs_get_radius(&poi_world, idx), 50.0f);
    EXPECT_EQ(poi_ecs_get_satisfaction_bonus(&poi_world, idx), 15);
}

TEST_F(POIEcsTest, DefaultRadius) {
    POICreateParams params = make_poi_params(
        "Default Radius POI", POI_TYPE_NATURE, POI_TIER_GENERAL,
        0.0f, 0.0f, 0.0f, 0  // Should use defaults
    );
    
    int idx = poi_ecs_create(&poi_world, &params);
    
    EXPECT_FLOAT_EQ(poi_ecs_get_radius(&poi_world, idx), POI_DEFAULT_RADIUS);
    EXPECT_EQ(poi_ecs_get_satisfaction_bonus(&poi_world, idx), POI_SATISFACTION_GENERAL);
}

TEST_F(POIEcsTest, SpecialTierDefaultBonus) {
    POICreateParams params = make_poi_params(
        "Special POI", POI_TYPE_MILITARY, POI_TIER_SPECIAL,
        0.0f, 0.0f, 0.0f, 0  // Should use special tier default
    );
    
    int idx = poi_ecs_create(&poi_world, &params);
    
    EXPECT_EQ(poi_ecs_get_satisfaction_bonus(&poi_world, idx), POI_SATISFACTION_SPECIAL);
}

TEST_F(POIEcsTest, FindAtPosition) {
    POICreateParams params = make_poi_params(
        "Findable POI", POI_TYPE_NATURE, POI_TIER_GENERAL,
        100.0f, 100.0f, 50.0f, 5
    );
    
    int idx = poi_ecs_create(&poi_world, &params);
    
    // Point inside radius
    int found = poi_ecs_find_at_position(&poi_world, 120.0f, 110.0f);
    EXPECT_EQ(found, idx);
    
    // Point outside radius
    found = poi_ecs_find_at_position(&poi_world, 200.0f, 200.0f);
    EXPECT_EQ(found, -1);
}

TEST_F(POIEcsTest, VisitState) {
    POICreateParams params = make_poi_params(
        "Visitable POI", POI_TYPE_HISTORICAL, POI_TIER_GENERAL,
        50.0f, 50.0f, 30.0f, 8
    );
    
    int idx = poi_ecs_create(&poi_world, &params);
    
    EXPECT_FALSE(poi_ecs_is_visited(&poi_world, idx));
    
    poi_ecs_set_visited(&poi_world, idx, true);
    
    EXPECT_TRUE(poi_ecs_is_visited(&poi_world, idx));
}

TEST_F(POIEcsTest, Statistics) {
    // Create variety of POIs
    POICreateParams nature_general = make_poi_params("Nature General", POI_TYPE_NATURE, POI_TIER_GENERAL, 0.0f, 0.0f);
    POICreateParams historical_special = make_poi_params("Historical Special", POI_TYPE_HISTORICAL, POI_TIER_SPECIAL, 100.0f, 0.0f);
    POICreateParams military_general = make_poi_params("Military General", POI_TYPE_MILITARY, POI_TIER_GENERAL, 200.0f, 0.0f);
    
    poi_ecs_create(&poi_world, &nature_general);
    poi_ecs_create(&poi_world, &historical_special);
    int mil_idx = poi_ecs_create(&poi_world, &military_general);
    
    poi_ecs_set_visited(&poi_world, mil_idx, true);
    
    POIStats stats = poi_ecs_get_stats(&poi_world);
    
    EXPECT_EQ(stats.total_pois, 3u);
    EXPECT_EQ(stats.visited_pois, 1u);
    EXPECT_EQ(stats.nature_count, 1u);
    EXPECT_EQ(stats.historical_count, 1u);
    EXPECT_EQ(stats.military_count, 1u);
    EXPECT_EQ(stats.general_count, 2u);
    EXPECT_EQ(stats.special_count, 1u);
}

TEST_F(POIEcsTest, TypeStringConversion) {
    EXPECT_STREQ(poi_type_to_string(POI_TYPE_NATURE), "nature");
    EXPECT_STREQ(poi_type_to_string(POI_TYPE_HISTORICAL), "historical");
    EXPECT_STREQ(poi_type_to_string(POI_TYPE_MILITARY), "military");
    
    EXPECT_EQ(poi_type_from_string("nature"), POI_TYPE_NATURE);
    EXPECT_EQ(poi_type_from_string("HISTORICAL"), POI_TYPE_HISTORICAL);
    EXPECT_EQ(poi_type_from_string("Military"), POI_TYPE_MILITARY);
}

// =============================================================================
// Fog of War Tests
// =============================================================================

class FogOfWarTest : public ::testing::Test {
protected:
    FogOfWarState fog;
    
    void SetUp() override {
        fog_init(&fog);
    }
    
    void TearDown() override {
        fog_shutdown(&fog);
    }
};

TEST_F(FogOfWarTest, InitializesCorrectly) {
    EXPECT_TRUE(fog.initialized);
    EXPECT_TRUE(fog.enabled);
    EXPECT_TRUE(fog.prototype_mode);
}

TEST_F(FogOfWarTest, PrototypeModeVisibility) {
    // In prototype mode, POIs start as hidden but are still rendered
    // The actual visibility state tracks whether player has approached them
    VisibilityState state = fog_get_poi_visibility(&fog, 0);
    EXPECT_EQ(state, VISIBILITY_HIDDEN);  // Starts hidden, prototype mode just allows rendering
}

TEST_F(FogOfWarTest, RevealPOI) {
    fog_reveal_poi(&fog, 0);
    
    EXPECT_EQ(fog_get_poi_visibility(&fog, 0), VISIBILITY_VISIBLE);
    EXPECT_FLOAT_EQ(fog_get_poi_alpha(&fog, 0), 0.0f);
}

TEST_F(FogOfWarTest, FogAlphaWithPrototypeMode) {
    // In prototype mode, fog alpha is NOT artificially reduced
    // It stays at 1.0 until player approaches the POI
    float alpha = fog_get_poi_alpha(&fog, 0);
    EXPECT_FLOAT_EQ(alpha, 1.0f);  // Full fog for hidden POIs
}

TEST_F(FogOfWarTest, DisabledFog) {
    fog_set_enabled(&fog, false);
    
    float alpha = fog_get_poi_alpha(&fog, 0);
    EXPECT_FLOAT_EQ(alpha, 0.0f);  // No fog when disabled
}

// =============================================================================
// Satisfaction Tests
// =============================================================================

class SatisfactionTest : public ::testing::Test {
protected:
    POIEcsWorld poi_world;
    TourSatisfaction tour;
    
    void SetUp() override {
        poi_ecs_init(&poi_world);
        satisfaction_tour_start(&tour, 0);
    }
    
    void TearDown() override {
        poi_ecs_shutdown(&poi_world);
    }
};

TEST_F(SatisfactionTest, TourStarts) {
    EXPECT_TRUE(satisfaction_tour_is_active(&tour));
    EXPECT_EQ(tour.visited_count, 0u);
}

TEST_F(SatisfactionTest, RecordPOIVisit) {
    POICreateParams params = make_poi_params(
        "Test POI", POI_TYPE_NATURE, POI_TIER_GENERAL,
        0.0f, 0.0f, 50.0f, 10
    );
    
    int idx = poi_ecs_create(&poi_world, &params);
    
    int bonus = satisfaction_record_poi_visit(&tour, &poi_world, idx);
    
    EXPECT_GT(bonus, 0);
    EXPECT_EQ(tour.visited_count, 1u);
    EXPECT_TRUE(tour.has_nature);
}

TEST_F(SatisfactionTest, NoDuplicateVisits) {
    POICreateParams params = make_poi_params(
        "Test POI", POI_TYPE_HISTORICAL, POI_TIER_GENERAL, 0.0f, 0.0f
    );
    
    int idx = poi_ecs_create(&poi_world, &params);
    
    satisfaction_record_poi_visit(&tour, &poi_world, idx);
    int second_bonus = satisfaction_record_poi_visit(&tour, &poi_world, idx);
    
    EXPECT_EQ(second_bonus, 0);  // No bonus for duplicate
    EXPECT_EQ(tour.visited_count, 1u);  // Still only one visit
}

TEST_F(SatisfactionTest, VarietyBonus) {
    // Create POIs of different types
    POICreateParams nature = make_poi_params("Nature", POI_TYPE_NATURE, POI_TIER_GENERAL, 0.0f, 0.0f);
    POICreateParams historical = make_poi_params("Historical", POI_TYPE_HISTORICAL, POI_TIER_GENERAL, 100.0f, 0.0f);
    POICreateParams military = make_poi_params("Military", POI_TYPE_MILITARY, POI_TIER_GENERAL, 200.0f, 0.0f);
    
    int n_idx = poi_ecs_create(&poi_world, &nature);
    int h_idx = poi_ecs_create(&poi_world, &historical);
    int m_idx = poi_ecs_create(&poi_world, &military);
    
    satisfaction_record_poi_visit(&tour, &poi_world, n_idx);
    satisfaction_record_poi_visit(&tour, &poi_world, h_idx);
    
    // After 2 types, should have variety bonus
    EXPECT_GT(tour.variety_bonus, 0);
    
    satisfaction_record_poi_visit(&tour, &poi_world, m_idx);
    
    // After 3 types, should have higher variety bonus
    SatisfactionConfig config = satisfaction_get_default_config();
    EXPECT_EQ(tour.variety_bonus, config.variety_bonus_3_types);
}

TEST_F(SatisfactionTest, ScoreCalculation) {
    POICreateParams params = make_poi_params(
        "Test POI", POI_TYPE_NATURE, POI_TIER_GENERAL,
        0.0f, 0.0f, 50.0f, 10
    );
    
    poi_ecs_create(&poi_world, &params);
    satisfaction_record_poi_visit(&tour, &poi_world, 0);
    
    int score = satisfaction_calculate_score(&tour);
    
    // Base (50) + POI bonus (at least 10)
    EXPECT_GE(score, 50 + 10);
}

TEST_F(SatisfactionTest, RatingThresholds) {
    EXPECT_EQ(satisfaction_get_rating(30), RATING_POOR);
    EXPECT_EQ(satisfaction_get_rating(50), RATING_AVERAGE);  // 40-59 is AVERAGE
    EXPECT_EQ(satisfaction_get_rating(65), RATING_GOOD);     // 60-74 is GOOD
    EXPECT_EQ(satisfaction_get_rating(80), RATING_EXCELLENT); // 75-89 is EXCELLENT
    EXPECT_EQ(satisfaction_get_rating(95), RATING_OUTSTANDING);
}

TEST_F(SatisfactionTest, TourEnd) {
    POICreateParams params = make_poi_params("POI", POI_TYPE_NATURE, POI_TIER_GENERAL, 0.0f, 0.0f);
    int idx = poi_ecs_create(&poi_world, &params);
    satisfaction_record_poi_visit(&tour, &poi_world, idx);
    
    satisfaction_tour_end(&tour);
    
    EXPECT_FALSE(satisfaction_tour_is_active(&tour));
    EXPECT_GT(tour.total_satisfaction, 0);
}

TEST_F(SatisfactionTest, CompletionBonus) {
    // Restart tour with expectation of 1 POI
    satisfaction_tour_start(&tour, 1);
    
    POICreateParams params = make_poi_params(
        "Low Value POI", POI_TYPE_NATURE, POI_TIER_GENERAL,
        0.0f, 0.0f, 50.0f, 5
    );
    
    int idx = poi_ecs_create(&poi_world, &params);
    
    // Visiting this one POI should give 50 (base) + 5 (bonus) = 55 normally
    // But since it's the only one and we visited all (1/1), it should be 100
    
    satisfaction_record_poi_visit(&tour, &poi_world, idx);
    int score = satisfaction_calculate_score(&tour);
    
    EXPECT_EQ(score, 100);
}

// =============================================================================
// POI Loader Tests (using string parsing)
// =============================================================================

class POILoaderTest : public ::testing::Test {
protected:
    POIEcsWorld poi_world;
    
    void SetUp() override {
        poi_ecs_init(&poi_world);
    }
    
    void TearDown() override {
        poi_ecs_shutdown(&poi_world);
    }
};

TEST_F(POILoaderTest, LoadFromValidJSON) {
    const char* json = R"({
        "version": "1.0",
        "pois": [
            {
                "name": "Test Lighthouse",
                "type": "historical",
                "tier": "special",
                "position": { "x": 100.0, "y": 200.0 },
                "radius": 50.0,
                "satisfaction_bonus": 15,
                "description": "A test lighthouse"
            }
        ]
    })";
    
    POILoadResult result = poi_load_from_string(&poi_world, json);
    
    EXPECT_EQ(result, POI_LOAD_SUCCESS);
    EXPECT_EQ(poi_ecs_get_count(&poi_world), 1u);
    
    EXPECT_STREQ(poi_ecs_get_name(&poi_world, 0), "Test Lighthouse");
    EXPECT_EQ(poi_ecs_get_type(&poi_world, 0), POI_TYPE_HISTORICAL);
    EXPECT_EQ(poi_ecs_get_tier(&poi_world, 0), POI_TIER_SPECIAL);
}

TEST_F(POILoaderTest, LoadMultiplePOIs) {
    const char* json = R"({
        "version": "1.0",
        "pois": [
            {"name": "POI 1", "type": "nature", "tier": "general", "position": {"x": 0, "y": 0}},
            {"name": "POI 2", "type": "historical", "tier": "special", "position": {"x": 100, "y": 100}},
            {"name": "POI 3", "type": "military", "tier": "general", "position": {"x": 200, "y": 200}}
        ]
    })";
    
    POILoadResult result = poi_load_from_string(&poi_world, json);
    
    EXPECT_EQ(result, POI_LOAD_SUCCESS);
    EXPECT_EQ(poi_ecs_get_count(&poi_world), 3u);
}

TEST_F(POILoaderTest, InvalidJSONReturnsError) {
    const char* invalid_json = "{ this is not valid json }";
    
    POILoadResult result = poi_load_from_string(&poi_world, invalid_json);
    
    EXPECT_EQ(result, POI_LOAD_PARSE_ERROR);
}

TEST_F(POILoaderTest, MissingPOIsArrayReturnsError) {
    const char* json = R"({"version": "1.0"})";
    
    POILoadResult result = poi_load_from_string(&poi_world, json);
    
    EXPECT_EQ(result, POI_LOAD_INVALID_SCHEMA);
}

TEST_F(POILoaderTest, LoadDefaults) {
    poi_load_defaults(&poi_world);
    
    // Should have at least a few default POIs
    EXPECT_GE(poi_ecs_get_count(&poi_world), 3u);
}

TEST_F(POILoaderTest, LighthouseSpriteIDFromJSON) {
    // Test that lighthouse POIs loaded from JSON get correct sprite ID
    const char* json = R"({
        "version": "1.0",
        "pois": [
            {
                "name": "Vinga Lighthouse",
                "type": "historical",
                "tier": "special",
                "position": { "x": 100.0, "y": 200.0 },
                "radius": 50.0,
                "satisfaction_bonus": 15,
                "description": "A coastal lighthouse"
            },
            {
                "name": "Old Castle",
                "type": "historical",
                "tier": "special",
                "position": { "x": 200.0, "y": 300.0 },
                "radius": 60.0,
                "satisfaction_bonus": 18,
                "description": "Historic castle"
            }
        ]
    })";
    
    POILoadResult result = poi_load_from_string(&poi_world, json);
    
    EXPECT_EQ(result, POI_LOAD_SUCCESS);
    EXPECT_EQ(poi_ecs_get_count(&poi_world), 2u);
    
    // Lighthouse should have TEXTURE_LIGHTHOUSE
    EXPECT_STREQ(poi_ecs_get_name(&poi_world, 0), "Vinga Lighthouse");
    EXPECT_EQ(poi_ecs_get_sprite_id(&poi_world, 0), TEXTURE_LIGHTHOUSE);
    
    // Non-lighthouse should have TEXTURE_NONE
    EXPECT_STREQ(poi_ecs_get_name(&poi_world, 1), "Old Castle");
    EXPECT_EQ(poi_ecs_get_sprite_id(&poi_world, 1), TEXTURE_NONE);
}

TEST_F(POILoaderTest, LoadActualAssetFile) {
    // Verify that the actual asset file is valid JSON and loads correctly
    POILoadResult result = poi_load_from_file(&poi_world, "assets/data/pois.json");
    
    // Note: If running from build dir, path might need adjustment. 
    // Usually CTest runs from build dir, so "../assets/data/pois.json" might be needed
    // But since we don't know exact CWD, we'll try a few or just skip if file not found
    
    if (result == POI_LOAD_FILE_NOT_FOUND) {
        result = poi_load_from_file(&poi_world, "../assets/data/pois.json");
    }
    
    if (result != POI_LOAD_FILE_NOT_FOUND) {
        EXPECT_EQ(result, POI_LOAD_SUCCESS);
        // Expect at least the 10 POIs we know are in there
        EXPECT_GE(poi_ecs_get_count(&poi_world), 10u);
    }
}

// =============================================================================
// Sprite ID Optimization Tests
// =============================================================================

TEST_F(POIEcsTest, LighthouseSpriteIDPrecomputed) {
    // Test that lighthouses get TEXTURE_LIGHTHOUSE sprite ID at load time
    POICreateParams lighthouse_params = make_poi_params(
        "Test Lighthouse", POI_TYPE_HISTORICAL, POI_TIER_SPECIAL,
        100.0f, 200.0f, 50.0f, 15
    );
    
    int idx = poi_ecs_create(&poi_world, &lighthouse_params);
    
    EXPECT_GE(idx, 0);
    
    // Verify sprite ID is set to TEXTURE_LIGHTHOUSE
    TextureID sprite_id = poi_ecs_get_sprite_id(&poi_world, idx);
    EXPECT_EQ(sprite_id, TEXTURE_LIGHTHOUSE);
}

TEST_F(POIEcsTest, LighthouseCaseInsensitive) {
    // Test lowercase "lighthouse"
    POICreateParams lighthouse_params = make_poi_params(
        "Old lighthouse", POI_TYPE_HISTORICAL, POI_TIER_GENERAL,
        50.0f, 50.0f, 30.0f, 8
    );
    
    int idx = poi_ecs_create(&poi_world, &lighthouse_params);
    
    TextureID sprite_id = poi_ecs_get_sprite_id(&poi_world, idx);
    EXPECT_EQ(sprite_id, TEXTURE_LIGHTHOUSE);
}

TEST_F(POIEcsTest, NonLighthouseUsesGeometricFallback) {
    // Test that non-lighthouse POIs use TEXTURE_NONE (geometric fallback)
    POICreateParams nature_params = make_poi_params(
        "Mountain Peak", POI_TYPE_NATURE, POI_TIER_GENERAL,
        150.0f, 150.0f, 40.0f, 5
    );
    
    int idx = poi_ecs_create(&poi_world, &nature_params);
    
    TextureID sprite_id = poi_ecs_get_sprite_id(&poi_world, idx);
    EXPECT_EQ(sprite_id, TEXTURE_NONE);
}

TEST_F(POIEcsTest, MultiplePOIsCorrectSpriteIDs) {
    // Create variety of POIs and verify correct sprite IDs
    POICreateParams lighthouse = make_poi_params("Coastal Lighthouse", POI_TYPE_HISTORICAL, POI_TIER_SPECIAL, 0.0f, 0.0f);
    POICreateParams castle = make_poi_params("Ancient Castle", POI_TYPE_HISTORICAL, POI_TIER_SPECIAL, 100.0f, 0.0f);
    POICreateParams another_lighthouse = make_poi_params("Harbor Lighthouse", POI_TYPE_HISTORICAL, POI_TIER_GENERAL, 200.0f, 0.0f);
    
    int lighthouse_idx = poi_ecs_create(&poi_world, &lighthouse);
    int castle_idx = poi_ecs_create(&poi_world, &castle);
    int lighthouse2_idx = poi_ecs_create(&poi_world, &another_lighthouse);
    
    // Verify sprite IDs
    EXPECT_EQ(poi_ecs_get_sprite_id(&poi_world, lighthouse_idx), TEXTURE_LIGHTHOUSE);
    EXPECT_EQ(poi_ecs_get_sprite_id(&poi_world, castle_idx), TEXTURE_NONE);
    EXPECT_EQ(poi_ecs_get_sprite_id(&poi_world, lighthouse2_idx), TEXTURE_LIGHTHOUSE);
}
