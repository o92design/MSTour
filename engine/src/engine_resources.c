#include "engine_resources.h"
#include <raylib.h>
#include <string.h>
#include <stdio.h>

void resources_init(ResourceManager* rm) {
    if (!rm) return;
    
    memset(rm, 0, sizeof(ResourceManager));
    rm->initialized = true;
    
    printf("Resources: Manager initialized\n");
}

void resources_shutdown(ResourceManager* rm) {
    if (!rm || !rm->initialized) return;
    
    // Unload all textures
    for (int i = 0; i < rm->texture_count; i++) {
        if (rm->textures[i].loaded) {
            UnloadTexture(rm->textures[i].texture);
            rm->textures[i].loaded = false;
        }
    }
    
    // Unload all fonts (except default)
    for (int i = 0; i < rm->font_count; i++) {
        if (rm->fonts[i].loaded) {
            UnloadFont(rm->fonts[i].font);
            rm->fonts[i].loaded = false;
        }
    }
    
    // Unload all models
    for (int i = 0; i < rm->model_count; i++) {
        if (rm->models[i].loaded) {
            UnloadModel(rm->models[i].model);
            rm->models[i].loaded = false;
        }
    }
    
    rm->initialized = false;
    printf("Resources: Manager shutdown, all resources unloaded\n");
}

// === TEXTURES ===

TextureHandle resources_load_texture(ResourceManager* rm, const char* filepath, const char* name) {
    if (!rm || !rm->initialized || !filepath) return INVALID_TEXTURE_HANDLE;
    if (rm->texture_count >= MAX_TEXTURES) {
        printf("Resources: Maximum textures reached (%d)\n", MAX_TEXTURES);
        return INVALID_TEXTURE_HANDLE;
    }
    
    // Check if already loaded
    TextureHandle existing = resources_find_texture(rm, name);
    if (existing != INVALID_TEXTURE_HANDLE) {
        return existing;
    }
    
    // Check if file exists
    if (!FileExists(filepath)) {
        printf("Resources: Texture file not found: %s\n", filepath);
        return INVALID_TEXTURE_HANDLE;
    }
    
    // Load texture
    Texture2D tex = LoadTexture(filepath);
    if (tex.id == 0) {
        printf("Resources: Failed to load texture: %s\n", filepath);
        return INVALID_TEXTURE_HANDLE;
    }
    
    // Store
    int handle = rm->texture_count;
    rm->textures[handle].texture = tex;
    rm->textures[handle].name = name;
    rm->textures[handle].filepath = filepath;
    rm->textures[handle].loaded = true;
    rm->texture_count++;
    
    printf("Resources: Loaded texture '%s' (%dx%d) from %s\n", 
           name, tex.width, tex.height, filepath);
    return handle;
}

Texture2D resources_get_texture(const ResourceManager* rm, TextureHandle handle) {
    if (!rm || handle < 0 || handle >= rm->texture_count) {
        return (Texture2D){0};
    }
    if (!rm->textures[handle].loaded) {
        return (Texture2D){0};
    }
    return rm->textures[handle].texture;
}

TextureHandle resources_find_texture(const ResourceManager* rm, const char* name) {
    if (!rm || !name) return INVALID_TEXTURE_HANDLE;
    
    for (int i = 0; i < rm->texture_count; i++) {
        if (rm->textures[i].name && strcmp(rm->textures[i].name, name) == 0) {
            return i;
        }
    }
    return INVALID_TEXTURE_HANDLE;
}

void resources_unload_texture(ResourceManager* rm, TextureHandle handle) {
    if (!rm || handle < 0 || handle >= rm->texture_count) return;
    if (!rm->textures[handle].loaded) return;
    
    UnloadTexture(rm->textures[handle].texture);
    rm->textures[handle].loaded = false;
    printf("Resources: Unloaded texture '%s'\n", rm->textures[handle].name);
}

// === FONTS ===

FontHandle resources_load_font(ResourceManager* rm, const char* filepath, const char* name, int size) {
    if (!rm || !rm->initialized || !filepath) return INVALID_FONT_HANDLE;
    if (rm->font_count >= MAX_FONTS) {
        printf("Resources: Maximum fonts reached (%d)\n", MAX_FONTS);
        return INVALID_FONT_HANDLE;
    }
    
    // Check if file exists
    if (!FileExists(filepath)) {
        printf("Resources: Font file not found: %s\n", filepath);
        return INVALID_FONT_HANDLE;
    }
    
    // Load font
    Font font = LoadFontEx(filepath, size, NULL, 0);
    if (font.texture.id == 0) {
        printf("Resources: Failed to load font: %s\n", filepath);
        return INVALID_FONT_HANDLE;
    }
    
    // Store
    int handle = rm->font_count;
    rm->fonts[handle].font = font;
    rm->fonts[handle].name = name;
    rm->fonts[handle].size = size;
    rm->fonts[handle].loaded = true;
    rm->font_count++;
    
    printf("Resources: Loaded font '%s' (size %d) from %s\n", name, size, filepath);
    return handle;
}

Font resources_get_font(const ResourceManager* rm, FontHandle handle) {
    if (!rm || handle < 0 || handle >= rm->font_count) {
        return GetFontDefault();
    }
    if (!rm->fonts[handle].loaded) {
        return GetFontDefault();
    }
    return rm->fonts[handle].font;
}

FontHandle resources_find_font(const ResourceManager* rm, const char* name) {
    if (!rm || !name) return INVALID_FONT_HANDLE;
    
    for (int i = 0; i < rm->font_count; i++) {
        if (rm->fonts[i].name && strcmp(rm->fonts[i].name, name) == 0) {
            return i;
        }
    }
    return INVALID_FONT_HANDLE;
}

Font resources_get_default_font(void) {
    return GetFontDefault();
}

// === MODELS ===

ModelHandle resources_load_model(ResourceManager* rm, const char* filepath, const char* name) {
    if (!rm || !rm->initialized || !filepath) return INVALID_MODEL_HANDLE;
    if (rm->model_count >= MAX_MODELS) {
        printf("Resources: Maximum models reached (%d)\n", MAX_MODELS);
        return INVALID_MODEL_HANDLE;
    }
    
    // Check if already loaded
    ModelHandle existing = resources_find_model(rm, name);
    if (existing != INVALID_MODEL_HANDLE) {
        return existing;
    }
    
    // Check if file exists
    if (!FileExists(filepath)) {
        printf("Resources: Model file not found: %s\n", filepath);
        return INVALID_MODEL_HANDLE;
    }
    
    // Load model
    Model model = LoadModel(filepath);
    if (model.meshCount == 0) {
        printf("Resources: Failed to load model: %s\n", filepath);
        return INVALID_MODEL_HANDLE;
    }
    
    // Store
    int handle = rm->model_count;
    rm->models[handle].model = model;
    rm->models[handle].name = name;
    rm->models[handle].filepath = filepath;
    rm->models[handle].loaded = true;
    rm->model_count++;
    
    printf("Resources: Loaded model '%s' (%d meshes) from %s\n", 
           name, model.meshCount, filepath);
    return handle;
}

Model resources_get_model(const ResourceManager* rm, ModelHandle handle) {
    if (!rm || handle < 0 || handle >= rm->model_count) {
        return (Model){0};
    }
    if (!rm->models[handle].loaded) {
        return (Model){0};
    }
    return rm->models[handle].model;
}

ModelHandle resources_find_model(const ResourceManager* rm, const char* name) {
    if (!rm || !name) return INVALID_MODEL_HANDLE;
    
    for (int i = 0; i < rm->model_count; i++) {
        if (rm->models[i].name && strcmp(rm->models[i].name, name) == 0) {
            return i;
        }
    }
    return INVALID_MODEL_HANDLE;
}

void resources_unload_model(ResourceManager* rm, ModelHandle handle) {
    if (!rm || handle < 0 || handle >= rm->model_count) return;
    if (!rm->models[handle].loaded) return;
    
    UnloadModel(rm->models[handle].model);
    rm->models[handle].loaded = false;
    printf("Resources: Unloaded model '%s'\n", rm->models[handle].name);
}

// === UTILITIES ===

bool resources_texture_valid(const ResourceManager* rm, TextureHandle handle) {
    if (!rm || handle < 0 || handle >= rm->texture_count) return false;
    return rm->textures[handle].loaded;
}

bool resources_model_valid(const ResourceManager* rm, ModelHandle handle) {
    if (!rm || handle < 0 || handle >= rm->model_count) return false;
    return rm->models[handle].loaded;
}

void resources_debug_print(const ResourceManager* rm) {
    if (!rm) return;
    
    printf("=== Resource Manager Debug ===\n");
    printf("Textures: %d/%d\n", rm->texture_count, MAX_TEXTURES);
    printf("Fonts: %d/%d\n", rm->font_count, MAX_FONTS);
    printf("Models: %d/%d\n", rm->model_count, MAX_MODELS);
    
    printf("\nTextures:\n");
    for (int i = 0; i < rm->texture_count; i++) {
        printf("  [%d] %s: %s (%dx%d)\n", i,
               rm->textures[i].name,
               rm->textures[i].loaded ? "loaded" : "unloaded",
               rm->textures[i].texture.width,
               rm->textures[i].texture.height);
    }
    
    printf("\nModels:\n");
    for (int i = 0; i < rm->model_count; i++) {
        printf("  [%d] %s: %s (%d meshes)\n", i,
               rm->models[i].name,
               rm->models[i].loaded ? "loaded" : "unloaded",
               rm->models[i].model.meshCount);
    }
}
