#ifndef ENGINE_RESOURCES_H
#define ENGINE_RESOURCES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <raylib.h>
#include <stdbool.h>

// =============================================================================
// Resource Manager
// Provides resource loading, caching, and lifecycle management
// =============================================================================

// Maximum resources per type
#define MAX_TEXTURES 128
#define MAX_FONTS 16
#define MAX_MODELS 64

// Resource handles
typedef int TextureHandle;
typedef int FontHandle;
typedef int ModelHandle;

#define INVALID_TEXTURE_HANDLE -1
#define INVALID_FONT_HANDLE -1
#define INVALID_MODEL_HANDLE -1

// Texture entry
typedef struct TextureEntry {
    Texture2D texture;
    const char* name;
    const char* filepath;
    bool loaded;
} TextureEntry;

// Font entry
typedef struct FontEntry {
    Font font;
    const char* name;
    int size;
    bool loaded;
} FontEntry;

// Model entry
typedef struct ModelEntry {
    Model model;
    const char* name;
    const char* filepath;
    bool loaded;
} ModelEntry;

// Resource manager state
typedef struct ResourceManager {
    TextureEntry textures[MAX_TEXTURES];
    int texture_count;
    
    FontEntry fonts[MAX_FONTS];
    int font_count;
    
    ModelEntry models[MAX_MODELS];
    int model_count;
    
    bool initialized;
} ResourceManager;

// Initialize resource manager
void resources_init(ResourceManager* rm);

// Shutdown resource manager (unloads all resources)
void resources_shutdown(ResourceManager* rm);

// === TEXTURES ===

// Load texture from file
TextureHandle resources_load_texture(ResourceManager* rm, const char* filepath, const char* name);

// Get texture by handle
Texture2D resources_get_texture(const ResourceManager* rm, TextureHandle handle);

// Find texture by name
TextureHandle resources_find_texture(const ResourceManager* rm, const char* name);

// Unload specific texture
void resources_unload_texture(ResourceManager* rm, TextureHandle handle);

// === FONTS ===

// Load font from file with size
FontHandle resources_load_font(ResourceManager* rm, const char* filepath, const char* name, int size);

// Get font by handle
Font resources_get_font(const ResourceManager* rm, FontHandle handle);

// Find font by name
FontHandle resources_find_font(const ResourceManager* rm, const char* name);

// Get default font (always available)
Font resources_get_default_font(void);

// === MODELS ===

// Load 3D model from file
ModelHandle resources_load_model(ResourceManager* rm, const char* filepath, const char* name);

// Get model by handle
Model resources_get_model(const ResourceManager* rm, ModelHandle handle);

// Find model by name
ModelHandle resources_find_model(const ResourceManager* rm, const char* name);

// Unload specific model
void resources_unload_model(ResourceManager* rm, ModelHandle handle);

// === UTILITIES ===

// Check if texture is valid/loaded
bool resources_texture_valid(const ResourceManager* rm, TextureHandle handle);

// Check if model is valid/loaded
bool resources_model_valid(const ResourceManager* rm, ModelHandle handle);

// Print resource statistics
void resources_debug_print(const ResourceManager* rm);

#ifdef __cplusplus
}
#endif

#endif // ENGINE_RESOURCES_H
