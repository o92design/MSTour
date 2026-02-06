#ifndef GAME_TEXTURES_H
#define GAME_TEXTURES_H

#include <raylib.h>
#include <stdbool.h>

// =============================================================================
// Game Texture Manager
// Handles loading and caching of game textures/sprites with pre-computed metadata
// =============================================================================

// Texture identifiers
typedef enum {
    TEXTURE_SHIP_FORWARD,
    TEXTURE_SHIP_BACKWARD,
    TEXTURE_LIGHTHOUSE,
    TEXTURE_COUNT,
    TEXTURE_NONE = -1
} TextureID;

// Scale factors for different texture types
#define SHIP_SPRITE_SCALE 0.15f
#define POI_LIGHTHOUSE_SCALE 0.12f

// Pre-computed texture metadata for efficient rendering
typedef struct TextureMetadata {
    Texture2D texture;        // Raw Raylib texture
    float scaled_width;       // Pre-computed: texture.width * scale
    float scaled_height;      // Pre-computed: texture.height * scale
    Rectangle source;         // Pre-computed: full texture source rect
    Vector2 origin;           // Pre-computed: center origin for rotation
    float scale;              // Scale factor used
} TextureMetadata;

// Initialize texture system (loads all textures)
bool game_textures_init(void);

// Cleanup texture system (unloads all textures)
void game_textures_cleanup(void);

// Get a loaded texture by ID (returns copy - use get_metadata for hot paths)
Texture2D game_textures_get(TextureID id);

// Get texture metadata pointer for efficient rendering (no copy, pre-computed values)
const TextureMetadata* game_textures_get_metadata(TextureID id);

// Check if textures are loaded
bool game_textures_is_loaded(void);

#endif // GAME_TEXTURES_H
