#include "ship_render.h"
#include "game_constants.h"
#include "game_textures.h"
#include "engine_math.h"
#include <raylib.h>
#include <raymath.h>
#include <math.h>

ShipVisualStyle ship_render_get_default_style(void) {
    ShipVisualStyle style = {
        .hull_color = RED,
        .outline_color = DARKGRAY,
        .center_color = YELLOW,
        .length = SHIP_LENGTH,
        .width = SHIP_WIDTH
    };
    return style;
}

ShipVisualStyle ship_render_get_player_style(void) {
    ShipVisualStyle style = {
        .hull_color = RED,
        .outline_color = DARKGRAY,
        .center_color = YELLOW,
        .length = SHIP_LENGTH,
        .width = SHIP_WIDTH
    };
    return style;
}

ShipVisualStyle ship_render_get_ai_style(void) {
    ShipVisualStyle style = {
        .hull_color = BLUE,
        .outline_color = DARKBLUE,
        .center_color = WHITE,
        .length = SHIP_LENGTH * 0.8f,  // AI ships slightly smaller
        .width = SHIP_WIDTH * 0.8f
    };
    return style;
}

void ship_render_draw_at(float x, float y, float heading, const ShipVisualStyle* style) {
    if (!style) return;
    
    Vector2 ship_pos = {x, y};
    
    // Try to use sprite rendering with pre-computed metadata
    const TextureMetadata* meta = game_textures_get_metadata(TEXTURE_SHIP_FORWARD);
    if (meta && meta->texture.id != 0) {
        // Use pre-computed values - no per-frame calculations
        Rectangle dest = {x, y, meta->scaled_width, meta->scaled_height};
        DrawTexturePro(meta->texture, meta->source, dest, meta->origin, heading, WHITE);
        return;
    }
    
    // Fallback to geometric rendering if sprites not available
    // Calculate ship vertices for triangle (pointing in heading direction)
    // Ship heading: 0° = north (up), rotating clockwise
    
    // Front point (bow)
    float bow_angle = (heading - 90.0f) * DEG2RAD;
    Vector2 bow = {
        ship_pos.x + cosf(bow_angle) * style->length * SHIP_BOW_LENGTH_MULT,
        ship_pos.y + sinf(bow_angle) * style->length * SHIP_BOW_LENGTH_MULT
    };
    
    // Rear points (stern)
    float stern_angle_left = (heading - 90.0f + SHIP_STERN_ANGLE) * DEG2RAD;
    float stern_angle_right = (heading - 90.0f - SHIP_STERN_ANGLE) * DEG2RAD;
    Vector2 stern_left = {
        ship_pos.x + cosf(stern_angle_left) * style->width * SHIP_STERN_WIDTH_MULT,
        ship_pos.y + sinf(stern_angle_left) * style->width * SHIP_STERN_WIDTH_MULT
    };
    Vector2 stern_right = {
        ship_pos.x + cosf(stern_angle_right) * style->width * SHIP_STERN_WIDTH_MULT,
        ship_pos.y + sinf(stern_angle_right) * style->width * SHIP_STERN_WIDTH_MULT
    };
    
    // Draw ship as triangle
    DrawTriangle(stern_left, stern_right, bow, style->hull_color);
    DrawTriangleLines(stern_left, stern_right, bow, style->outline_color);
    
    // Draw ship center point
    DrawCircle((int)ship_pos.x, (int)ship_pos.y, SHIP_CENTER_RADIUS, style->center_color);
}

void ship_render_draw(const ShipState* ship, const ShipVisualStyle* style) {
    if (!ship || !style) return;
    
    // Choose texture based on velocity direction
    bool moving_forward = ship->speed >= 0.0f;
    TextureID tex_id = moving_forward ? TEXTURE_SHIP_FORWARD : TEXTURE_SHIP_BACKWARD;
    const TextureMetadata* meta = game_textures_get_metadata(tex_id);
    
    if (meta && meta->texture.id != 0) {
        // Use pre-computed metadata - no per-frame calculations
        Rectangle dest = {ship->pos_x, ship->pos_y, meta->scaled_width, meta->scaled_height};
        DrawTexturePro(meta->texture, meta->source, dest, meta->origin, ship->heading, WHITE);
        return;
    }
    
    // Fallback to geometric rendering
    ship_render_draw_at(ship->pos_x, ship->pos_y, ship->heading, style);
}

void ship_render_draw_default(const ShipState* ship) {
    if (!ship) return;
    ShipVisualStyle style = ship_render_get_default_style();
    ship_render_draw(ship, &style);
}

void ship_render_draw_wake(const ShipState* ship, float intensity) {
    if (!ship || intensity <= 0.0f) return;
    if (math_abs(ship->speed) < 1.0f) return;
    
    // Calculate wake position behind ship
    float wake_angle = math_deg_to_rad(ship->heading - 90.0f + 180.0f); // Behind ship
    float wake_distance = SHIP_LENGTH * 0.5f;
    
    Vector2 wake_pos = {
        ship->pos_x + cosf(wake_angle) * wake_distance,
        ship->pos_y + sinf(wake_angle) * wake_distance
    };
    
    // Wake intensity based on speed
    float speed_factor = math_clamp(math_abs(ship->speed) / 100.0f, 0.0f, 1.0f);
    float alpha = 100.0f * speed_factor * intensity;
    
    // Draw wake as fading circles
    for (int i = 0; i < 3; i++) {
        float offset = (float)i * 15.0f;
        Vector2 circle_pos = {
            wake_pos.x + cosf(wake_angle) * offset,
            wake_pos.y + sinf(wake_angle) * offset
        };
        float radius = 8.0f + (float)i * 4.0f;
        unsigned char circle_alpha = (unsigned char)(alpha * (1.0f - (float)i * 0.3f));
        
        DrawCircle((int)circle_pos.x, (int)circle_pos.y, radius, 
                   (Color){255, 255, 255, circle_alpha});
    }
}
