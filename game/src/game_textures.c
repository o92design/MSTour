#include "game_textures.h"
#include <stdio.h>
#include <string.h>

// Texture metadata storage with pre-computed values
static TextureMetadata texture_data[TEXTURE_COUNT] = {0};
static bool textures_loaded = false;

// Texture file paths and scale factors
static const struct {
    const char* path;
    float scale;
} texture_config[TEXTURE_COUNT] = {
    {"assets/textures/top_down_orto_fwd.png", SHIP_SPRITE_SCALE},
    {"assets/textures/top_down_orto_bck.png", SHIP_SPRITE_SCALE},
    {"assets/textures/soderskar_65.png", POI_LIGHTHOUSE_SCALE}
};

bool game_textures_init(void) {
    if (textures_loaded) {
        printf("[TEXTURES] Already loaded\n");
        return true;
    }
    
    printf("[TEXTURES] Loading game textures...\n");
    
    for (int i = 0; i < TEXTURE_COUNT; i++) {
        Texture2D tex = LoadTexture(texture_config[i].path);
        
        if (tex.id == 0) {
            printf("[TEXTURES] ERROR: Failed to load texture: %s\n", texture_config[i].path);
            // Cleanup previously loaded textures
            for (int j = 0; j < i; j++) {
                UnloadTexture(texture_data[j].texture);
                texture_data[j] = (TextureMetadata){0};
            }
            return false;
        }
        
        // Pre-compute metadata for efficient rendering
        float scale = texture_config[i].scale;
        float scaled_w = (float)tex.width * scale;
        float scaled_h = (float)tex.height * scale;
        
        texture_data[i] = (TextureMetadata){
            .texture = tex,
            .scaled_width = scaled_w,
            .scaled_height = scaled_h,
            .source = (Rectangle){0, 0, (float)tex.width, (float)tex.height},
            .origin = (Vector2){scaled_w * 0.5f, scaled_h * 0.5f},
            .scale = scale
        };
        
        printf("[TEXTURES] Loaded: %s (%dx%d, scale=%.2f)\n", 
               texture_config[i].path, tex.width, tex.height, scale);
    }
    
    textures_loaded = true;
    printf("[TEXTURES] All textures loaded with pre-computed metadata\n");
    return true;
}

void game_textures_cleanup(void) {
    if (!textures_loaded) {
        return;
    }
    
    printf("[TEXTURES] Unloading textures...\n");
    
    for (int i = 0; i < TEXTURE_COUNT; i++) {
        if (texture_data[i].texture.id != 0) {
            UnloadTexture(texture_data[i].texture);
            texture_data[i] = (TextureMetadata){0};
        }
    }
    
    textures_loaded = false;
}

Texture2D game_textures_get(TextureID id) {
    if (!textures_loaded || id < 0 || id >= TEXTURE_COUNT) {
        return (Texture2D){0};
    }
    return texture_data[id].texture;
}

const TextureMetadata* game_textures_get_metadata(TextureID id) {
    if (!textures_loaded || id < 0 || id >= TEXTURE_COUNT) {
        return NULL;
    }
    return &texture_data[id];
}

bool game_textures_is_loaded(void) {
    return textures_loaded;
}
