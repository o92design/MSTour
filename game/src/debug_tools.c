#include "debug_tools.h"
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <math.h>

// Helper: Convert degrees to radians
static float deg_to_rad(float degrees) {
    return degrees * (PI / 180.0f);
}

void debug_tools_init(DebugState* state) {
    state->show_visualization = false;
    state->show_debug_panel = false;
    state->show_help = false;
}

void debug_tools_update(DebugState* state) {
    // Toggle visualization (F1)
    if (IsKeyPressed(KEY_F1)) {
        state->show_visualization = !state->show_visualization;
        printf("Debug: Visualization %s\n", state->show_visualization ? "ON" : "OFF");
    }
    
    // Toggle debug panel (F2)
    if (IsKeyPressed(KEY_F2)) {
        state->show_debug_panel = !state->show_debug_panel;
        printf("Debug: Panel %s\n", state->show_debug_panel ? "ON" : "OFF");
    }
    
    // Toggle help (F3)
    if (IsKeyPressed(KEY_F3)) {
        state->show_help = !state->show_help;
        printf("Debug: Help %s\n", state->show_help ? "ON" : "OFF");
    }
}

void debug_tools_draw_visualization(const DebugState* state, const ShipState* ship, const ShipPhysicsConfig* config) {
    if (!state->show_visualization) return;
    
    Vector2 ship_pos = {ship->pos_x, ship->pos_y};
    
    // === VELOCITY VECTOR ===
    if (fabsf(ship->speed) > 0.1f) {
        float heading_rad = deg_to_rad(ship->heading - 90.0f);
        Vector2 velocity_end = {
            ship_pos.x + cosf(heading_rad) * ship->speed * 2.0f,
            ship_pos.y + sinf(heading_rad) * ship->speed * 2.0f
        };
        
        // Draw velocity vector (green)
        DrawLineEx(ship_pos, velocity_end, 3.0f, GREEN);
        DrawCircleV(velocity_end, 5.0f, GREEN);
        
        // Draw speed label
        char speed_label[32];
        snprintf(speed_label, sizeof(speed_label), "%.1f u/s", ship->speed);
        DrawText(speed_label, (int)velocity_end.x + 10, (int)velocity_end.y - 10, 16, GREEN);
    }
    
    // === HEADING INDICATOR ===
    {
        float heading_rad = deg_to_rad(ship->heading - 90.0f);
        Vector2 heading_end = {
            ship_pos.x + cosf(heading_rad) * 50.0f,
            ship_pos.y + sinf(heading_rad) * 50.0f
        };
        
        // Draw heading line (yellow)
        DrawLineEx(ship_pos, heading_end, 2.0f, YELLOW);
    }
    
    // === TURN RADIUS PREVIEW ===
    if (fabsf(ship->angular_velocity) > 0.5f && fabsf(ship->speed) > 1.0f) {
        // Calculate turn radius
        // radius = speed / angular_velocity (converted to radians)
        float angular_velocity_rad = ship->angular_velocity * DEG2RAD;
        float turn_radius = fabsf(ship->speed / angular_velocity_rad);
        
        // Clamp for display
        if (turn_radius < 2000.0f) {
            // Calculate center of turn circle
            float perpendicular_angle = ship->heading + (ship->angular_velocity > 0 ? 0.0f : 180.0f);
            float perp_rad = deg_to_rad(perpendicular_angle - 90.0f);
            
            Vector2 circle_center = {
                ship_pos.x + cosf(perp_rad) * turn_radius,
                ship_pos.y + sinf(perp_rad) * turn_radius
            };
            
            // Draw turn radius circle (cyan, semi-transparent)
            DrawCircleLines((int)circle_center.x, (int)circle_center.y, turn_radius, SKYBLUE);
            
            // Draw radius line
            DrawLineEx(ship_pos, circle_center, 1.0f, SKYBLUE);
            
            // Draw radius label
            char radius_label[32];
            snprintf(radius_label, sizeof(radius_label), "R: %.0f", turn_radius);
            DrawText(radius_label, (int)circle_center.x - 20, (int)circle_center.y - 10, 16, SKYBLUE);
        }
    }
    
    // === DRIFT VISUALIZATION ===
    if (fabsf(ship->angular_velocity) > 0.5f) {
        // Show drift direction
        float drift_angle = ship->heading + (ship->angular_velocity > 0 ? 90.0f : -90.0f);
        float drift_rad = deg_to_rad(drift_angle - 90.0f);
        float drift_magnitude = config->drift_factor * fabsf(ship->angular_velocity) * fabsf(ship->speed) * 0.5f;
        
        Vector2 drift_end = {
            ship_pos.x + cosf(drift_rad) * drift_magnitude,
            ship_pos.y + sinf(drift_rad) * drift_magnitude
        };
        
        // Draw drift vector (orange)
        DrawLineEx(ship_pos, drift_end, 2.0f, ORANGE);
        DrawText("drift", (int)drift_end.x + 5, (int)drift_end.y - 10, 14, ORANGE);
    }
    
    // === COORDINATE AXES (at ship position) ===
    DrawCircleLines((int)ship_pos.x, (int)ship_pos.y, 100.0f, (Color){255, 255, 255, 100});
    DrawCircleLines((int)ship_pos.x, (int)ship_pos.y, 200.0f, (Color){255, 255, 255, 50});
}

void debug_tools_draw_panel(const DebugState* state, const ShipState* ship, const ShipPhysicsConfig* config) {
    if (!state->show_debug_panel) return;
    
    // Draw semi-transparent background panel
    int panel_x = 1000;
    int panel_y = 20;
    int panel_width = 260;
    int panel_height = 400;
    
    DrawRectangle(panel_x, panel_y, panel_width, panel_height, (Color){0, 0, 0, 200});
    DrawRectangleLines(panel_x, panel_y, panel_width, panel_height, WHITE);
    
    // Title
    DrawText("DEBUG PANEL [F2]", panel_x + 10, panel_y + 10, 18, YELLOW);
    
    int y = panel_y + 40;
    int line_height = 20;
    
    // Helper macro for drawing debug values
    #define DRAW_VALUE(label, format, value) \
        { char buf[128]; \
          snprintf(buf, sizeof(buf), label ": " format, value); \
          DrawText(buf, panel_x + 10, y, 16, WHITE); \
          y += line_height; }
    
    DRAW_VALUE("Position X", "%.1f", ship->pos_x);
    DRAW_VALUE("Position Y", "%.1f", ship->pos_y);
    DRAW_VALUE("Heading", "%.1f°", ship->heading);
    
    y += 5; // Spacer
    DRAW_VALUE("Speed", "%.1f u/s", ship->speed);
    DRAW_VALUE("Max Speed", "%.1f u/s", config->max_speed);
    DRAW_VALUE("Speed %%", "%.1f%%", (ship->speed / config->max_speed) * 100.0f);
    
    y += 5;
    DRAW_VALUE("Velocity X", "%.1f", ship->velocity_x);
    DRAW_VALUE("Velocity Y", "%.1f", ship->velocity_y);
    
    y += 5;
    DRAW_VALUE("Throttle", "%.2f", ship->throttle);
    DRAW_VALUE("Target Throttle", "%.2f", ship->target_throttle);
    
    y += 5;
    DRAW_VALUE("Rudder", "%.2f", ship->rudder);
    DRAW_VALUE("Target Rudder", "%.2f", ship->target_rudder);
    
    y += 5;
    DRAW_VALUE("Angular Vel", "%.1f°/s", ship->angular_velocity);
    DRAW_VALUE("Max Turn Rate", "%.1f°/s", config->max_turn_rate);
    
    #undef DRAW_VALUE
}

void debug_tools_draw_help(const DebugState* state) {
    if (!state->show_help) return;
    
    // Draw semi-transparent overlay
    DrawRectangle(0, 0, 1280, 720, (Color){0, 0, 0, 230});
    
    int y = 100;
    int line_height = 30;
    
    DrawText("MS TOUR - DEBUG CONTROLS [F3]", 100, y, 30, YELLOW);
    y += 50;
    
    DrawText("=== Ship Controls ===", 100, y, 24, WHITE);
    y += line_height;
    DrawText("W / Up Arrow     - Throttle Forward", 120, y, 20, LIGHTGRAY);
    y += line_height;
    DrawText("S / Down Arrow   - Throttle Reverse", 120, y, 20, LIGHTGRAY);
    y += line_height;
    DrawText("A / Left Arrow   - Turn Left", 120, y, 20, LIGHTGRAY);
    y += line_height;
    DrawText("D / Right Arrow  - Turn Right", 120, y, 20, LIGHTGRAY);
    y += line_height + 10;
    
    DrawText("=== Debug Tools ===", 100, y, 24, WHITE);
    y += line_height;
    DrawText("F1 - Toggle Visualization (velocity, turn radius)", 120, y, 20, LIGHTGRAY);
    y += line_height;
    DrawText("F2 - Toggle Debug Panel (ship state values)", 120, y, 20, LIGHTGRAY);
    y += line_height;
    DrawText("F3 - Toggle This Help Screen", 120, y, 20, LIGHTGRAY);
    y += line_height;
    DrawText("F4 - Teleport Ship to Center", 120, y, 20, LIGHTGRAY);
    y += line_height;
    DrawText("F5 - Reset Ship State", 120, y, 20, LIGHTGRAY);
    y += line_height;
    DrawText("F6 - Hot-Reload config.ini", 120, y, 20, LIGHTGRAY);
    y += line_height + 10;
    
    DrawText("=== Info ===", 100, y, 24, WHITE);
    y += line_height;
    DrawText("Edit config.ini in the game folder to tune physics", 120, y, 20, LIGHTGRAY);
    y += line_height;
    DrawText("Press F6 to reload without recompiling", 120, y, 20, LIGHTGRAY);
    y += line_height + 20;
    
    DrawText("Press F3 or ESC to close this help screen", 100, y, 20, YELLOW);
}

void debug_tools_teleport_ship(ShipState* ship, float x, float y) {
    ship->pos_x = x;
    ship->pos_y = y;
    printf("Debug: Teleported ship to (%.1f, %.1f)\n", x, y);
}

void debug_tools_reset_ship(ShipState* ship) {
    ship->speed = 0.0f;
    ship->velocity_x = 0.0f;
    ship->velocity_y = 0.0f;
    ship->angular_velocity = 0.0f;
    ship->throttle = 0.0f;
    ship->target_throttle = 0.0f;
    ship->rudder = 0.0f;
    ship->target_rudder = 0.0f;
    printf("Debug: Reset ship state\n");
}
