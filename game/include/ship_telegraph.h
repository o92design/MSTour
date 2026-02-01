#ifndef SHIP_TELEGRAPH_H
#define SHIP_TELEGRAPH_H

#include <stdbool.h>

// Engine telegraph orders (historical ship throttle system)
typedef enum TelegraphOrder {
    TELEGRAPH_FULL_ASTERN = -3,    // Full reverse
    TELEGRAPH_HALF_ASTERN = -2,    // Half reverse
    TELEGRAPH_SLOW_ASTERN = -1,    // Slow reverse
    TELEGRAPH_STOP = 0,             // Dead stop / idle
    TELEGRAPH_SLOW_AHEAD = 1,       // Slow forward (dock speed)
    TELEGRAPH_HALF_AHEAD = 2,       // Half forward (cruising)
    TELEGRAPH_FULL_AHEAD = 3        // Full forward (max speed)
} TelegraphOrder;

// Telegraph state
typedef struct ShipTelegraph {
    TelegraphOrder current_order;   // Current engine order
    TelegraphOrder previous_order;  // For transition detection
    float order_time;               // Time since last order change (for UI animation)
} ShipTelegraph;

// Initialize telegraph at STOP
void ship_telegraph_init(ShipTelegraph* telegraph);

// Ring up one order (increase throttle)
void ship_telegraph_ring_up(ShipTelegraph* telegraph);

// Ring down one order (decrease throttle)
void ship_telegraph_ring_down(ShipTelegraph* telegraph);

// Set specific order directly (for AI/autopilot)
void ship_telegraph_set_order(ShipTelegraph* telegraph, TelegraphOrder order);

// Get current order
TelegraphOrder ship_telegraph_get_order(const ShipTelegraph* telegraph);

// Convert telegraph order to throttle value (-1.0 to 1.0)
float ship_telegraph_get_throttle(const ShipTelegraph* telegraph);

// Check if order recently changed (for audio/visual feedback)
bool ship_telegraph_order_changed(const ShipTelegraph* telegraph, float delta_time);

// Update telegraph state (call each frame to track time)
void ship_telegraph_update(ShipTelegraph* telegraph, float delta_time);

// Get order name as string (for UI display)
const char* ship_telegraph_get_order_name(TelegraphOrder order);

#endif // SHIP_TELEGRAPH_H
