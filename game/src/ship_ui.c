#include "ship_ui.h"
#include <math.h>
#include <stdio.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void ship_ui_init(void) {
    // Initialize any UI resources if needed
}

void ship_ui_cleanup(void) {
    // Cleanup UI resources if needed
}

void ship_ui_render_speed_gauge(float speed, float max_speed, Vector2 position, float radius) {
    // Draw outer circle
    DrawCircleLines((int)position.x, (int)position.y, radius, DARKGRAY);
    DrawCircleLines((int)position.x, (int)position.y, radius - 1, DARKGRAY);
    
    // Draw tick marks for speed increments (0%, 25%, 50%, 75%, 100%)
    for (int i = 0; i <= 4; i++) {
        float angle = -135.0f + (270.0f * i / 4.0f); // Start at bottom-left, sweep clockwise
        float rad = angle * M_PI / 180.0f;
        float cos_angle = cosf(rad);
        float sin_angle = sinf(rad);
        
        Vector2 start = {
            position.x + cos_angle * (radius - 8),
            position.y + sin_angle * (radius - 8)
        };
        Vector2 end = {
            position.x + cos_angle * radius,
            position.y + sin_angle * radius
        };
        
        DrawLineEx(start, end, 2.0f, DARKGRAY);
    }
    
    // Calculate speed percentage and angle
    float speed_percent = fminf(speed / max_speed, 1.0f);
    float needle_angle = -135.0f + (270.0f * speed_percent);
    float needle_rad = needle_angle * M_PI / 180.0f;
    
    // Draw speed needle
    Vector2 needle_end = {
        position.x + cosf(needle_rad) * (radius - 5),
        position.y + sinf(needle_rad) * (radius - 5)
    };
    
    DrawLineEx(position, needle_end, 3.0f, SKYBLUE);
    DrawCircle((int)position.x, (int)position.y, 5, SKYBLUE);
    
    // Draw speed text
    char speed_text[32];
    snprintf(speed_text, sizeof(speed_text), "%.0f", speed);
    int text_width = MeasureText(speed_text, 20);
    DrawText(speed_text, (int)(position.x - text_width / 2), (int)(position.y + radius + 10), 20, WHITE);
    
    // Draw label
    const char* label = "SPEED";
    int label_width = MeasureText(label, 10);
    DrawText(label, (int)(position.x - label_width / 2), (int)(position.y + radius + 35), 10, LIGHTGRAY);
}

void ship_ui_render_compass(float heading, Vector2 position, float radius) {
    // Draw outer circle
    DrawCircleLines((int)position.x, (int)position.y, radius, DARKGRAY);
    DrawCircleLines((int)position.x, (int)position.y, radius - 1, DARKGRAY);
    
    // Draw cardinal directions (N, E, S, W)
    const char* directions[] = {"N", "E", "S", "W"};
    float angles[] = {-90.0f, 0.0f, 90.0f, 180.0f};
    
    for (int i = 0; i < 4; i++) {
        float angle_rad = angles[i] * M_PI / 180.0f;
        Vector2 text_pos = {
            position.x + cosf(angle_rad) * (radius - 15),
            position.y + sinf(angle_rad) * (radius - 15)
        };
        
        int text_width = MeasureText(directions[i], 12);
        DrawText(directions[i], (int)(text_pos.x - text_width / 2), (int)(text_pos.y - 6), 12, LIGHTGRAY);
    }
    
    // Draw heading needle (points in direction ship is facing)
    float heading_rad = (heading - 90.0f) * M_PI / 180.0f; // Adjust for Raylib's coordinate system
    Vector2 needle_end = {
        position.x + cosf(heading_rad) * (radius - 5),
        position.y + sinf(heading_rad) * (radius - 5)
    };
    
    DrawLineEx(position, needle_end, 3.0f, RED);
    DrawCircle((int)position.x, (int)position.y, 5, RED);
    
    // Draw heading text
    char heading_text[32];
    snprintf(heading_text, sizeof(heading_text), "%.0f°", heading);
    int text_width = MeasureText(heading_text, 20);
    DrawText(heading_text, (int)(position.x - text_width / 2), (int)(position.y + radius + 10), 20, WHITE);
    
    // Draw label
    const char* label = "HEADING";
    int label_width = MeasureText(label, 10);
    DrawText(label, (int)(position.x - label_width / 2), (int)(position.y + radius + 35), 10, LIGHTGRAY);
}

void ship_ui_render_telegraph(const ShipTelegraph* telegraph, Vector2 position) {
    // Draw telegraph box background
    Rectangle box = {position.x, position.y, 250, 80};
    DrawRectangleRec(box, (Color){20, 20, 30, 200});
    DrawRectangleLinesEx(box, 2, GOLD);
    
    // Draw title
    const char* title = "ENGINE TELEGRAPH";
    int title_width = MeasureText(title, 12);
    DrawText(title, (int)(position.x + 125 - title_width / 2), (int)(position.y + 10), 12, LIGHTGRAY);
    
    // Get current order name and throttle
    TelegraphOrder order = ship_telegraph_get_order(telegraph);
    const char* order_name = ship_telegraph_get_order_name(order);
    float throttle = ship_telegraph_get_throttle(telegraph);
    
    // Determine order color based on throttle
    Color order_color = GOLD;
    if (throttle > 0.0f) {
        order_color = GREEN;
    } else if (throttle < 0.0f) {
        order_color = RED;
    }
    
    // Draw order name (large and prominent)
    int order_width = MeasureText(order_name, 24);
    DrawText(order_name, (int)(position.x + 125 - order_width / 2), (int)(position.y + 35), 24, order_color);
    
    // Draw throttle percentage
    char throttle_text[32];
    snprintf(throttle_text, sizeof(throttle_text), "%.0f%%", throttle * 100.0f);
    int throttle_width = MeasureText(throttle_text, 16);
    DrawText(throttle_text, (int)(position.x + 125 - throttle_width / 2), (int)(position.y + 62), 16, LIGHTGRAY);
}

void ship_ui_render(const ShipState* ship, const ShipTelegraph* telegraph) {
    int screen_width = GetScreenWidth();
    int screen_height = GetScreenHeight();
    
    // Position gauges at bottom of screen
    float gauge_y = screen_height - 120.0f;
    float compass_x = 100.0f;
    float speed_x = screen_width - 100.0f;
    
    // Render compass (left)
    ship_ui_render_compass(ship->heading, (Vector2){compass_x, gauge_y}, 50.0f);
    
    // Render speed gauge (right)
    ship_ui_render_speed_gauge(ship->speed, 150.0f, (Vector2){speed_x, gauge_y}, 50.0f);
    
    // Render telegraph indicator (top center)
    float telegraph_x = (screen_width - 250) / 2.0f;
    ship_ui_render_telegraph(telegraph, (Vector2){telegraph_x, 20.0f});
}
