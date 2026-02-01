#include "ship_telegraph.h"
#include <string.h>

// Throttle values for each telegraph order
static const float TELEGRAPH_THROTTLE_VALUES[] = {
    -1.00f,  // FULL_ASTERN (-3)
    -0.65f,  // HALF_ASTERN (-2)
    -0.35f,  // SLOW_ASTERN (-1)
     0.00f,  // STOP (0)
     0.25f,  // SLOW_AHEAD (1) - good for docking/precision
     0.65f,  // HALF_AHEAD (2) - cruising speed
     1.00f   // FULL_AHEAD (3) - maximum speed
};

void ship_telegraph_init(ShipTelegraph* telegraph) {
    if (!telegraph) return;
    
    telegraph->current_order = TELEGRAPH_STOP;
    telegraph->previous_order = TELEGRAPH_STOP;
    telegraph->order_time = 0.0f;
}

void ship_telegraph_ring_up(ShipTelegraph* telegraph) {
    if (!telegraph) return;
    
    // Can't go higher than FULL_AHEAD
    if (telegraph->current_order >= TELEGRAPH_FULL_AHEAD) {
        return;
    }
    
    telegraph->previous_order = telegraph->current_order;
    telegraph->current_order = (TelegraphOrder)(telegraph->current_order + 1);
    telegraph->order_time = 0.0f; // Reset timer for animation
}

void ship_telegraph_ring_down(ShipTelegraph* telegraph) {
    if (!telegraph) return;
    
    // Can't go lower than FULL_ASTERN
    if (telegraph->current_order <= TELEGRAPH_FULL_ASTERN) {
        return;
    }
    
    telegraph->previous_order = telegraph->current_order;
    telegraph->current_order = (TelegraphOrder)(telegraph->current_order - 1);
    telegraph->order_time = 0.0f; // Reset timer for animation
}

void ship_telegraph_set_order(ShipTelegraph* telegraph, TelegraphOrder order) {
    if (!telegraph) return;
    
    // Clamp to valid range
    if (order < TELEGRAPH_FULL_ASTERN) order = TELEGRAPH_FULL_ASTERN;
    if (order > TELEGRAPH_FULL_AHEAD) order = TELEGRAPH_FULL_AHEAD;
    
    if (telegraph->current_order != order) {
        telegraph->previous_order = telegraph->current_order;
        telegraph->current_order = order;
        telegraph->order_time = 0.0f;
    }
}

TelegraphOrder ship_telegraph_get_order(const ShipTelegraph* telegraph) {
    if (!telegraph) return TELEGRAPH_STOP;
    return telegraph->current_order;
}

float ship_telegraph_get_throttle(const ShipTelegraph* telegraph) {
    if (!telegraph) return 0.0f;
    
    // Map order (-3 to +3) to array index (0 to 6)
    int index = telegraph->current_order + 3;
    if (index < 0 || index > 6) return 0.0f;
    
    return TELEGRAPH_THROTTLE_VALUES[index];
}

bool ship_telegraph_order_changed(const ShipTelegraph* telegraph, float threshold) {
    if (!telegraph) return false;
    return telegraph->order_time < threshold;
}

void ship_telegraph_update(ShipTelegraph* telegraph, float delta_time) {
    if (!telegraph) return;
    telegraph->order_time += delta_time;
}

const char* ship_telegraph_get_order_name(TelegraphOrder order) {
    switch (order) {
        case TELEGRAPH_FULL_ASTERN: return "Full Astern";
        case TELEGRAPH_HALF_ASTERN: return "Half Astern";
        case TELEGRAPH_SLOW_ASTERN: return "Slow Astern";
        case TELEGRAPH_STOP:        return "Stop";
        case TELEGRAPH_SLOW_AHEAD:  return "Slow Ahead";
        case TELEGRAPH_HALF_AHEAD:  return "Half Ahead";
        case TELEGRAPH_FULL_AHEAD:  return "Full Ahead";
        default:                    return "Unknown";
    }
}
