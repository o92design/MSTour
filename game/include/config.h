#ifndef CONFIG_H
#define CONFIG_H

#include "ship_physics.h"
#include <stdbool.h>

// Load ship physics configuration from config.ini
// Returns true on success, false on failure (uses defaults)
bool config_load_ship_physics(const char* filepath, ShipPhysicsConfig* config);

// Save current ship physics configuration to config.ini
bool config_save_ship_physics(const char* filepath, const ShipPhysicsConfig* config);

// Reload configuration (hot-reload support)
bool config_reload_ship_physics(const char* filepath, ShipPhysicsConfig* config);

#endif // CONFIG_H
