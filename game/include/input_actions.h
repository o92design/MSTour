#ifndef INPUT_ACTIONS_H
#define INPUT_ACTIONS_H

#include <stdbool.h>

// Ship control actions
typedef enum ShipAction {
    SHIP_ACTION_THROTTLE_UP,      // Default: W or Up Arrow
    SHIP_ACTION_THROTTLE_DOWN,    // Default: S or Down Arrow
    SHIP_ACTION_TURN_LEFT,        // Default: A or Left Arrow
    SHIP_ACTION_TURN_RIGHT,       // Default: D or Right Arrow
    SHIP_ACTION_BRAKE,            // Default: Space (optional, for future)
    SHIP_ACTION_COUNT
} ShipAction;

// Initialize action system
void input_actions_init(void);

// Update action states (call once per frame)
void input_actions_update(void);

// Check if action is currently active
bool input_action_is_active(ShipAction action);

// Check if action was just pressed this frame
bool input_action_pressed(ShipAction action);

// Check if action was just released this frame
bool input_action_released(ShipAction action);

// Get action axis value (-1.0 to 1.0 for analog inputs, -1/0/1 for digital)
float input_action_get_axis(ShipAction action);

// Get throttle axis (combining THROTTLE_UP and THROTTLE_DOWN)
// Returns: -1.0 (full reverse) to 1.0 (full forward)
float input_action_get_throttle_axis(void);

// Get steering axis (combining TURN_LEFT and TURN_RIGHT)
// Returns: -1.0 (full left) to 1.0 (full right)
float input_action_get_steering_axis(void);

// Configuration (for future remapping support)
void input_action_set_binding(ShipAction action, int key);
int input_action_get_binding(ShipAction action);

#endif // INPUT_ACTIONS_H
