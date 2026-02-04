#include "game_render.h"
#include "ship_render.h"
#include "ship_ui.h"
#include "game_constants.h"
#include "game_poi_ecs.h"
#include "game_fog_of_war.h"
#include "game_satisfaction.h"
#include "voyage_manager.h"
#include "results_screen.h"
#include "engine_core.h"
#include "engine_renderer.h"
#include "engine_camera.h"
#include "engine_ui.h"
#include <raylib.h>
#include <stdio.h>
#include <math.h>

// =============================================================================
// POI Rendering Helpers
// =============================================================================

static Color get_poi_color(POIType type, POITier tier, bool visited) {
    Color base;
    
    switch (type) {
        case POI_TYPE_NATURE:
            base = (Color){34, 139, 34, 255};  // Forest green
            break;
        case POI_TYPE_HISTORICAL:
            base = (Color){139, 69, 19, 255};  // Saddle brown
            break;
        case POI_TYPE_MILITARY:
            base = (Color){105, 105, 105, 255}; // Dim gray
            break;
        default:
            base = WHITE;
            break;
    }
    
    // Special tier is brighter
    if (tier == POI_TIER_SPECIAL) {
        base.r = (unsigned char)fminf(base.r * 1.3f, 255);
        base.g = (unsigned char)fminf(base.g * 1.3f, 255);
        base.b = (unsigned char)fminf(base.b * 1.3f, 255);
    }
    
    // Visited POIs are slightly faded
    if (visited) {
        base.a = 200;
    } else {
        // Make unvisited POIs more visible
        base.a = 220;
    }
    
    return base;
}

static void draw_poi_icon(float x, float y, POIType type, POITier tier, 
                          bool visited, float fog_alpha) {
    float size = (tier == POI_TIER_SPECIAL) ? 16.0f : 12.0f;
    Color color = get_poi_color(type, tier, visited);
    
    // Apply fog - ensure minimum visibility for navigation
    // At full fog (1.0), show faint icon at FOG_POI_MIN_ALPHA
    // At no fog (0.0), show full color
    float alpha_mult = FOG_POI_MIN_ALPHA + (1.0f - FOG_POI_MIN_ALPHA) * (1.0f - fog_alpha);
    color.a = (unsigned char)(color.a * alpha_mult);
    
    // Draw based on type
    switch (type) {
        case POI_TYPE_NATURE:
            // Tree-like triangle
            DrawTriangle(
                (Vector2){x, y - size},
                (Vector2){x - size * 0.7f, y + size * 0.5f},
                (Vector2){x + size * 0.7f, y + size * 0.5f},
                color
            );
            break;
            
        case POI_TYPE_HISTORICAL:
            // Building-like rectangle with roof
            DrawRectangle((int)(x - size * 0.5f), (int)(y - size * 0.3f), 
                          (int)size, (int)(size * 0.8f), color);
            DrawTriangle(
                (Vector2){x, y - size},
                (Vector2){x - size * 0.6f, y - size * 0.3f},
                (Vector2){x + size * 0.6f, y - size * 0.3f},
                color
            );
            break;
            
        case POI_TYPE_MILITARY:
            // Star/fort shape
            DrawPoly((Vector2){x, y}, 5, size, 0.0f, color);
            break;
            
        default:
            DrawCircle((int)x, (int)y, size, color);
            break;
    }
    
    // Draw ring for special tier (more visible when revealed)
    if (tier == POI_TIER_SPECIAL) {
        Color ring_color = GOLD;
        ring_color.a = (unsigned char)(200 * alpha_mult);
        DrawCircleLines((int)x, (int)y, size + 4, ring_color);
    }
    
    // Draw check mark if visited
    if (visited) {
        Color check_color = GREEN;
        check_color.a = (unsigned char)(255 * alpha_mult);
        DrawCircle((int)(x + size), (int)(y - size), 5, check_color);
    }
}

void game_render_pois(const GameState* state) {
    if (!state) return;
    
    const POIEcsWorld* poi_world = game_ecs_get_poi_world_const(&state->game_ecs);
    const FogOfWarState* fog = game_ecs_get_fog_const(&state->game_ecs);
    
    if (!poi_world) return;
    
    // Calculate visible bounds for culling
    float cam_x = state->camera.target.x;
    float cam_y = state->camera.target.y;
    float zoom = state->camera.zoom;
    int win_w = engine_get_window_width();
    int win_h = engine_get_window_height();
    float half_w = (win_w / 2.0f) / zoom;
    float half_h = (win_h / 2.0f) / zoom;
    float cull_margin = 100.0f;  // Extra margin for POI icons/labels
    
    uint32_t count = poi_ecs_get_count(poi_world);
    
    for (uint32_t i = 0; i < count; i++) {
        float x, y;
        poi_ecs_get_position(poi_world, (int)i, &x, &y);
        
        // Cull POIs outside camera bounds
        if (x < cam_x - half_w - cull_margin || x > cam_x + half_w + cull_margin ||
            y < cam_y - half_h - cull_margin || y > cam_y + half_h + cull_margin) {
            continue;
        }
        
        POIType type = poi_ecs_get_type(poi_world, (int)i);
        POITier tier = poi_ecs_get_tier(poi_world, (int)i);
        bool visited = poi_ecs_is_visited(poi_world, (int)i);
        
        // Get fog alpha for this POI
        float fog_alpha = fog ? fog_get_poi_alpha(fog, (int)i) : 0.0f;
        
        // In prototype mode, always render POIs (but fogged)
        // In non-prototype mode, skip completely hidden POIs
        if (fog && !fog->prototype_mode && fog_alpha > 0.95f) continue;
        
        // Draw radius indicator (more visible for unvisited POIs)
        if (fog_alpha < 0.8f) {
            float radius = poi_ecs_get_radius(poi_world, (int)i);
            // Make detection zones much more visible
            unsigned char zone_alpha = visited ? 20 : 50;
            Color radius_color = {100, 200, 255, (unsigned char)(zone_alpha * (1.0f - fog_alpha))};
            DrawCircle((int)x, (int)y, radius, radius_color);
            
            // Add bright border for unvisited POIs
            if (!visited) {
                Color border = {150, 220, 255, (unsigned char)(120 * (1.0f - fog_alpha))};
                DrawCircleLines((int)x, (int)y, radius, border);
            }
        }
        
        // Draw POI icon
        draw_poi_icon(x, y, type, tier, visited, fog_alpha);
        
        // Draw name label (only when fog is mostly cleared)
        if (fog_alpha < 0.5f) {
            const char* name = poi_ecs_get_name(poi_world, (int)i);
            int text_width = MeasureText(name, 10);
            Color text_color = WHITE;
            text_color.a = (unsigned char)(200 * (1.0f - fog_alpha));
            DrawText(name, (int)(x - text_width / 2), (int)(y + 20), 10, text_color);
        }
    }
}

void game_render_world(const GameState* state) {
    if (!state) return;
    
    // Clear to water color (entire world is ocean)
    Color water_color = {WATER_COLOR_R, WATER_COLOR_G, WATER_COLOR_B, WATER_COLOR_A};
    renderer_clear(water_color);
}

// Draw fog overlay using chunk-based system with batched rendering
static void game_render_fog_overlay(const GameState* state) {
    if (!state) return;
    
    const FogOfWarState* fog = game_ecs_get_fog_const(&state->game_ecs);
    if (!fog || !fog->enabled) return;
    
    // Fog color from constants
    Color fog_color = {FOG_COLOR_R, FOG_COLOR_G, FOG_COLOR_B, FOG_COLOR_A};
    
    // Initialize rectangle batch for efficient rendering
    RectBatch batch;
    renderer_rect_batch_init(&batch, fog_color);
    
    // Get camera info to determine visible area
    float cam_x = state->camera.target.x;
    float cam_y = state->camera.target.y;
    float zoom = state->camera.zoom;
    int win_w = engine_get_window_width();
    int win_h = engine_get_window_height();
    
    // Calculate visible world bounds with padding
    float half_w = (win_w / 2.0f) / zoom;
    float half_h = (win_h / 2.0f) / zoom;
    float world_left = cam_x - half_w - FOG_CELL_SIZE;
    float world_right = cam_x + half_w + FOG_CELL_SIZE;
    float world_top = cam_y - half_h - FOG_CELL_SIZE;
    float world_bottom = cam_y + half_h + FOG_CELL_SIZE;
    
    // Find chunk range for visible area
    int min_chunk_x = (int)floorf(world_left / FOG_CHUNK_WORLD_SIZE);
    int max_chunk_x = (int)floorf(world_right / FOG_CHUNK_WORLD_SIZE);
    int min_chunk_y = (int)floorf(world_top / FOG_CHUNK_WORLD_SIZE);
    int max_chunk_y = (int)floorf(world_bottom / FOG_CHUNK_WORLD_SIZE);
    
    int cell_size = (int)FOG_CELL_SIZE;
    
    // Iterate over visible chunks
    for (int cy = min_chunk_y; cy <= max_chunk_y; cy++) {
        for (int cx = min_chunk_x; cx <= max_chunk_x; cx++) {
            float chunk_origin_x = cx * FOG_CHUNK_WORLD_SIZE;
            float chunk_origin_y = cy * FOG_CHUNK_WORLD_SIZE;
            
            // Find chunk using spatial hash - O(1) lookup
            uint16_t chunk_idx = spatial_hash_find(&fog->chunk_map, cx, cy);
            const FogChunk* chunk = (chunk_idx != SPATIAL_HASH_NOT_FOUND) 
                                    ? &fog->chunks[chunk_idx] : NULL;
            
            // Calculate cell range visible in this chunk
            int cell_min_x = (int)((world_left - chunk_origin_x) / FOG_CELL_SIZE);
            int cell_max_x = (int)((world_right - chunk_origin_x) / FOG_CELL_SIZE);
            int cell_min_y = (int)((world_top - chunk_origin_y) / FOG_CELL_SIZE);
            int cell_max_y = (int)((world_bottom - chunk_origin_y) / FOG_CELL_SIZE);
            
            if (cell_min_x < 0) cell_min_x = 0;
            if (cell_min_y < 0) cell_min_y = 0;
            if (cell_max_x >= FOG_CHUNK_SIZE) cell_max_x = FOG_CHUNK_SIZE - 1;
            if (cell_max_y >= FOG_CHUNK_SIZE) cell_max_y = FOG_CHUNK_SIZE - 1;
            
            // Batch fog cells for this chunk
            for (int cell_y = cell_min_y; cell_y <= cell_max_y; cell_y++) {
                for (int cell_x = cell_min_x; cell_x <= cell_max_x; cell_x++) {
                    // Cell is fogged if chunk doesn't exist OR cell not revealed
                    bool is_revealed = chunk && chunk->cells[cell_y][cell_x];
                    
                    if (!is_revealed) {
                        int wx = (int)(chunk_origin_x + cell_x * FOG_CELL_SIZE);
                        int wy = (int)(chunk_origin_y + cell_y * FOG_CELL_SIZE);
                        
                        // Add to batch (auto-flushes when full)
                        if (!renderer_rect_batch_add(&batch, wx, wy, cell_size + 1, cell_size + 1)) {
                            renderer_rect_batch_flush(&batch);
                            renderer_rect_batch_add(&batch, wx, wy, cell_size + 1, cell_size + 1);
                        }
                    }
                }
            }
        }
    }
    
    // Flush any remaining rectangles
    renderer_rect_batch_flush(&batch);
}

void game_render_ships(const GameState* state) {
    if (!state) return;
    
    // Get player ship style
    ShipVisualStyle player_style = ship_render_get_player_style();
    
    // Draw wake first (behind ship)
    ship_render_draw_wake(&state->player_ship, 1.0f);
    
    // Draw player ship
    ship_render_draw(&state->player_ship, &player_style);
}

void game_render_ui(const GameState* state) {
    if (!state) return;

    // Only render UI if help and debug panel are not shown
    if (!state->debug.show_help && !state->debug.show_debug_panel) {
        // Title and controls hint (top-left with margin)
        int margin = 20;
        renderer_draw_text("MS Tour - Ship Control Prototype", margin, margin, 30, WHITE);
        renderer_draw_text("Controls: W/S=Telegraph Orders | A/D=Turn | F3=Help | F8=Complete Voyage", margin, margin + 40, 20, LIGHTGRAY);

        // Voyage progress HUD (top-right) - only show during active voyage
        if (state->voyage_active) {
            const VoyageProgress* progress = voyage_get_progress();
            int screen_width = engine_get_window_width();
            int hud_x = screen_width - 280;
            int hud_y = margin;
            
            // Background panel
            Color panel_bg = {20, 20, 40, 200};
            DrawRectangle(hud_x - 10, hud_y - 10, 270, 120, panel_bg);
            DrawRectangleLinesEx((Rectangle){hud_x - 10, hud_y - 10, 270, 120}, 2, (Color){100, 150, 200, 255});
            
            // Voyage title
            renderer_draw_text("VOYAGE PROGRESS", hud_x, hud_y, 20, (Color){150, 200, 255, 255});
            
            // POI counter
            char poi_text[64];
            snprintf(poi_text, sizeof(poi_text), "POIs: %d / %d", 
                     progress->current_poi_index, progress->total_pois);
            renderer_draw_text(poi_text, hud_x, hud_y + 30, 18, WHITE);
            
            // Overall satisfaction (average of all completed POIs)
            float avg_satisfaction = voyage_get_average_satisfaction();
            float avg_percent = avg_satisfaction * 100.0f;
            char sat_text[64];
            snprintf(sat_text, sizeof(sat_text), "Satisfaction: %.0f%%", avg_percent);
            Color sat_color = avg_percent >= 80 ? GREEN : 
                             avg_percent >= 60 ? YELLOW : 
                             avg_percent > 0 ? ORANGE : GRAY;
            renderer_draw_text(sat_text, hud_x, hud_y + 55, 18, sat_color);
            
            // Duration
            int minutes = (int)(progress->voyage_duration / 60.0f);
            int seconds = (int)(progress->voyage_duration) % 60;
            char time_text[64];
            snprintf(time_text, sizeof(time_text), "Time: %dm %02ds", minutes, seconds);
            renderer_draw_text(time_text, hud_x, hud_y + 80, 18, LIGHTGRAY);
        }

        // Ship UI (gauges and indicators) - uses engine_ui internally
        ship_ui_render(&state->player_ship, &state->telegraph);

        // Frame info (bottom-left)
        char debug_text[256];
        snprintf(debug_text, sizeof(debug_text), "FPS: %.1f | Frame: %llu", 
                 engine_get_fps(),
                 (unsigned long long)engine_get_frame_count());
        renderer_draw_text(debug_text, margin, (int)ui_from_bottom(40), 18, YELLOW);
    }
}

void game_render_debug(const GameState* state) {
    if (!state) return;
    
    // Debug visualization (velocity vectors, turn radius, etc.)
    debug_tools_draw_visualization(&state->debug, &state->player_ship, &state->physics_config);
    
    // Debug panel (ship state values)
    debug_tools_draw_panel(&state->debug, &state->player_ship, &state->physics_config);
    
    // Help overlay (drawn last, on top of everything)
    debug_tools_draw_help(&state->debug);
}

void game_render(const GameState* state) {
    if (!state || !state->initialized) return;
    
    // Begin camera mode for world rendering
    camera_begin(&state->camera);
    
    // Render in layers (world space)
    game_render_world(state);
    game_render_pois(state);
    game_render_fog_overlay(state);  // Fog on top of POIs, reveals around ship
    game_render_ships(state);        // Ship on top of fog
    
    // End camera mode
    camera_end();
    
    // Render UI (screen space, not affected by camera)
    game_render_ui(state);
    game_render_debug(state);
    
    // Render results screen on top of everything if showing
    if (state->results_showing) {
        results_screen_render();
    }
}
