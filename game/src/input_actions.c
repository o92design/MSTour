#include "input_actions.h"
#include <raylib.h>

// Default key bindings
static int action_bindings[SHIP_ACTION_COUNT];
static int action_bindings_alt[SHIP_ACTION_COUNT]; // Alternative keys (e.g., arrow keys)

void input_actions_init(void) {
    // Primary bindings (WASD)
    action_bindings[SHIP_ACTION_THROTTLE_UP] = KEY_W;
    action_bindings[SHIP_ACTION_THROTTLE_DOWN] = KEY_S;
    action_bindings[SHIP_ACTION_TURN_LEFT] = KEY_A;
    action_bindings[SHIP_ACTION_TURN_RIGHT] = KEY_D;
    action_bindings[SHIP_ACTION_BRAKE] = KEY_SPACE;
    
    // Alternative bindings (Arrow keys)
    action_bindings_alt[SHIP_ACTION_THROTTLE_UP] = KEY_UP;
    action_bindings_alt[SHIP_ACTION_THROTTLE_DOWN] = KEY_DOWN;
    action_bindings_alt[SHIP_ACTION_TURN_LEFT] = KEY_LEFT;
    action_bindings_alt[SHIP_ACTION_TURN_RIGHT] = KEY_RIGHT;
    action_bindings_alt[SHIP_ACTION_BRAKE] = KEY_LEFT_SHIFT;
}

void input_actions_update(void) {
    // Currently just polling, but could track state changes here
    // For future: handle button press/release events, input buffering, etc.
}

bool input_action_is_active(ShipAction action) {
    if (action < 0 || action >= SHIP_ACTION_COUNT) {
        return false;
    }
    
    // Check both primary and alternative bindings
    return IsKeyDown(action_bindings[action]) || 
           IsKeyDown(action_bindings_alt[action]);
}

bool input_action_pressed(ShipAction action) {
    if (action < 0 || action >= SHIP_ACTION_COUNT) {
        return false;
    }
    
    // Check if either binding was just pressed this frame
    return IsKeyPressed(action_bindings[action]) || 
           IsKeyPressed(action_bindings_alt[action]);
}

bool input_action_released(ShipAction action) {
    if (action < 0 || action >= SHIP_ACTION_COUNT) {
        return false;
    }
    
    // Check if either binding was just released this frame
    return IsKeyReleased(action_bindings[action]) || 
           IsKeyReleased(action_bindings_alt[action]);
}

float input_action_get_axis(ShipAction action) {
    return input_action_is_active(action) ? 1.0f : 0.0f;
}

float input_action_get_throttle_axis(void) {
    float throttle = 0.0f;
    
    if (input_action_is_active(SHIP_ACTION_THROTTLE_UP)) {
        throttle += 1.0f;
    }
    if (input_action_is_active(SHIP_ACTION_THROTTLE_DOWN)) {
        throttle -= 1.0f;
    }
    
    return throttle;
}

float input_action_get_steering_axis(void) {
    float steering = 0.0f;
    
    if (input_action_is_active(SHIP_ACTION_TURN_LEFT)) {
        steering -= 1.0f;
    }
    if (input_action_is_active(SHIP_ACTION_TURN_RIGHT)) {
        steering += 1.0f;
    }
    
    return steering;
}

void input_action_set_binding(ShipAction action, int key) {
    if (action >= 0 && action < SHIP_ACTION_COUNT) {
        action_bindings[action] = key;
    }
}

int input_action_get_binding(ShipAction action) {
    if (action >= 0 && action < SHIP_ACTION_COUNT) {
        return action_bindings[action];
    }
    return KEY_NULL;
}
