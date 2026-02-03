#ifndef ENGINE_INPUT_H
#define ENGINE_INPUT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <raylib.h>

// =============================================================================
// Input System
// Provides input abstraction with action mapping and state tracking
// =============================================================================

// Maximum number of actions that can be registered
#define MAX_INPUT_ACTIONS 32
#define MAX_BINDINGS_PER_ACTION 4

// Action state
typedef struct InputActionState {
    bool is_active;      // Currently pressed
    bool just_pressed;   // Pressed this frame
    bool just_released;  // Released this frame
    float axis_value;    // Axis value (-1.0 to 1.0)
} InputActionState;

// Action binding (key or button)
typedef struct InputBinding {
    int key;             // Keyboard key (KEY_*)
    int gamepad_button;  // Gamepad button (-1 if not bound)
    int gamepad_axis;    // Gamepad axis (-1 if not bound)
    float axis_positive; // If true, positive axis = positive value
} InputBinding;

// Action definition
typedef struct InputAction {
    const char* name;
    InputBinding bindings[MAX_BINDINGS_PER_ACTION];
    int binding_count;
    InputActionState state;
} InputAction;

// Input system state
typedef struct InputSystem {
    InputAction actions[MAX_INPUT_ACTIONS];
    int action_count;
    
    // Mouse state
    Vector2 mouse_position;
    Vector2 mouse_delta;
    float mouse_wheel;
    bool mouse_buttons[3]; // Left, Right, Middle
    
    // Gamepad
    int active_gamepad;
    bool gamepad_connected;
} InputSystem;

// Initialize input system
void input_init(InputSystem* input);

// Update input system (call once per frame)
void input_update(InputSystem* input);

// Register a new action
int input_register_action(InputSystem* input, const char* name);

// Bind a keyboard key to an action
void input_bind_key(InputSystem* input, int action_id, int key);

// Bind an alternative key to an action
void input_bind_key_alt(InputSystem* input, int action_id, int key);

// Unbind all bindings for an action
void input_unbind_action(InputSystem* input, int action_id);

// Check if action is currently active (pressed)
bool input_is_active(const InputSystem* input, int action_id);

// Check if action was just pressed this frame
bool input_just_pressed(const InputSystem* input, int action_id);

// Check if action was just released this frame
bool input_just_released(const InputSystem* input, int action_id);

// Get axis value for an action (-1.0 to 1.0 for digital, smooth for analog)
float input_get_axis(const InputSystem* input, int action_id);

// Get combined axis from positive and negative actions
float input_get_combined_axis(const InputSystem* input, int positive_action, int negative_action);

// Mouse functions
Vector2 input_get_mouse_position(const InputSystem* input);
Vector2 input_get_mouse_delta(const InputSystem* input);
float input_get_mouse_wheel(const InputSystem* input);
bool input_mouse_button_down(const InputSystem* input, int button);
bool input_mouse_button_pressed(const InputSystem* input, int button);

// Find action ID by name (-1 if not found)
int input_find_action(const InputSystem* input, const char* name);

// Debug: Print all registered actions
void input_debug_print(const InputSystem* input);

#ifdef __cplusplus
}
#endif

#endif // ENGINE_INPUT_H
