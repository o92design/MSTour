#include "engine_input.h"
#include <raylib.h>
#include <string.h>
#include <stdio.h>

void input_init(InputSystem* input) {
    if (!input) return;
    
    memset(input, 0, sizeof(InputSystem));
    input->active_gamepad = 0;
    input->gamepad_connected = false;
    
    // Initialize all bindings to -1 (unbound)
    for (int i = 0; i < MAX_INPUT_ACTIONS; i++) {
        for (int j = 0; j < MAX_BINDINGS_PER_ACTION; j++) {
            input->actions[i].bindings[j].key = -1;
            input->actions[i].bindings[j].gamepad_button = -1;
            input->actions[i].bindings[j].gamepad_axis = -1;
        }
    }
}

void input_update(InputSystem* input) {
    if (!input) return;
    
    // Update mouse state
    input->mouse_position = GetMousePosition();
    input->mouse_delta = GetMouseDelta();
    input->mouse_wheel = GetMouseWheelMove();
    input->mouse_buttons[0] = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
    input->mouse_buttons[1] = IsMouseButtonDown(MOUSE_RIGHT_BUTTON);
    input->mouse_buttons[2] = IsMouseButtonDown(MOUSE_MIDDLE_BUTTON);
    
    // Check gamepad
    input->gamepad_connected = IsGamepadAvailable(input->active_gamepad);
    
    // Update all actions
    for (int i = 0; i < input->action_count; i++) {
        InputAction* action = &input->actions[i];
        bool was_active = action->state.is_active;
        bool is_active = false;
        float axis_value = 0.0f;
        
        // Check all bindings for this action
        for (int j = 0; j < action->binding_count; j++) {
            InputBinding* binding = &action->bindings[j];
            
            // Check keyboard
            if (binding->key >= 0) {
                if (IsKeyDown(binding->key)) {
                    is_active = true;
                    axis_value = 1.0f;
                }
            }
            
            // Check gamepad button
            if (input->gamepad_connected && binding->gamepad_button >= 0) {
                if (IsGamepadButtonDown(input->active_gamepad, binding->gamepad_button)) {
                    is_active = true;
                    axis_value = 1.0f;
                }
            }
            
            // Check gamepad axis
            if (input->gamepad_connected && binding->gamepad_axis >= 0) {
                float gp_axis = GetGamepadAxisMovement(input->active_gamepad, binding->gamepad_axis);
                if (binding->axis_positive) {
                    if (gp_axis > 0.15f) { // Deadzone
                        is_active = true;
                        axis_value = gp_axis;
                    }
                } else {
                    if (gp_axis < -0.15f) {
                        is_active = true;
                        axis_value = -gp_axis;
                    }
                }
            }
        }
        
        // Update state
        action->state.just_pressed = is_active && !was_active;
        action->state.just_released = !is_active && was_active;
        action->state.is_active = is_active;
        action->state.axis_value = axis_value;
    }
}

int input_register_action(InputSystem* input, const char* name) {
    if (!input || !name) return -1;
    if (input->action_count >= MAX_INPUT_ACTIONS) return -1;
    
    int id = input->action_count;
    input->actions[id].name = name;
    input->actions[id].binding_count = 0;
    memset(&input->actions[id].state, 0, sizeof(InputActionState));
    input->action_count++;
    
    return id;
}

void input_bind_key(InputSystem* input, int action_id, int key) {
    if (!input || action_id < 0 || action_id >= input->action_count) return;
    
    InputAction* action = &input->actions[action_id];
    if (action->binding_count >= MAX_BINDINGS_PER_ACTION) return;
    
    int idx = action->binding_count;
    action->bindings[idx].key = key;
    action->bindings[idx].gamepad_button = -1;
    action->bindings[idx].gamepad_axis = -1;
    action->binding_count++;
}

void input_bind_key_alt(InputSystem* input, int action_id, int key) {
    // Same as bind_key, adds another binding
    input_bind_key(input, action_id, key);
}

void input_unbind_action(InputSystem* input, int action_id) {
    if (!input || action_id < 0 || action_id >= input->action_count) return;
    
    InputAction* action = &input->actions[action_id];
    for (int j = 0; j < MAX_BINDINGS_PER_ACTION; j++) {
        action->bindings[j].key = -1;
        action->bindings[j].gamepad_button = -1;
        action->bindings[j].gamepad_axis = -1;
    }
    action->binding_count = 0;
}

bool input_is_active(const InputSystem* input, int action_id) {
    if (!input || action_id < 0 || action_id >= input->action_count) return false;
    return input->actions[action_id].state.is_active;
}

bool input_just_pressed(const InputSystem* input, int action_id) {
    if (!input || action_id < 0 || action_id >= input->action_count) return false;
    return input->actions[action_id].state.just_pressed;
}

bool input_just_released(const InputSystem* input, int action_id) {
    if (!input || action_id < 0 || action_id >= input->action_count) return false;
    return input->actions[action_id].state.just_released;
}

float input_get_axis(const InputSystem* input, int action_id) {
    if (!input || action_id < 0 || action_id >= input->action_count) return 0.0f;
    return input->actions[action_id].state.axis_value;
}

float input_get_combined_axis(const InputSystem* input, int positive_action, int negative_action) {
    float positive = input_get_axis(input, positive_action);
    float negative = input_get_axis(input, negative_action);
    return positive - negative;
}

Vector2 input_get_mouse_position(const InputSystem* input) {
    if (!input) return (Vector2){0, 0};
    return input->mouse_position;
}

Vector2 input_get_mouse_delta(const InputSystem* input) {
    if (!input) return (Vector2){0, 0};
    return input->mouse_delta;
}

float input_get_mouse_wheel(const InputSystem* input) {
    if (!input) return 0.0f;
    return input->mouse_wheel;
}

bool input_mouse_button_down(const InputSystem* input, int button) {
    if (!input || button < 0 || button > 2) return false;
    return input->mouse_buttons[button];
}

bool input_mouse_button_pressed(const InputSystem* input, int button) {
    if (!input || button < 0 || button > 2) return false;
    return IsMouseButtonPressed(button);
}

int input_find_action(const InputSystem* input, const char* name) {
    if (!input || !name) return -1;
    
    for (int i = 0; i < input->action_count; i++) {
        if (input->actions[i].name && strcmp(input->actions[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

void input_debug_print(const InputSystem* input) {
    if (!input) return;
    
    printf("=== Input System Debug ===\n");
    printf("Actions registered: %d\n", input->action_count);
    
    for (int i = 0; i < input->action_count; i++) {
        InputAction* action = &input->actions[i];
        printf("  [%d] %s: active=%d, pressed=%d, released=%d, axis=%.2f\n",
               i, action->name,
               action->state.is_active,
               action->state.just_pressed,
               action->state.just_released,
               action->state.axis_value);
    }
    
    printf("Mouse: (%.0f, %.0f) wheel=%.1f\n",
           input->mouse_position.x, input->mouse_position.y, input->mouse_wheel);
    printf("Gamepad: %s\n", input->gamepad_connected ? "connected" : "disconnected");
}
