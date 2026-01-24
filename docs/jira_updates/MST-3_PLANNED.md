# MST-3: Engine Foundation - PLANNED 🟡

**Ticket URL**: https://o92design.atlassian.net/browse/MST-3  
**Status**: 🟡 PLANNED  
**Priority**: High  
**Depends On**: MST-2 (✅ Complete)  
**Estimated Effort**: 8-12 hours  

---

## Purpose

Build the core engine systems required for a data-oriented game engine. These systems form the foundation for all game features: input handling for player interaction, camera system for world viewing, Entity Component System (ECS) for game objects, resource management for assets, and enhanced rendering for visual output. Without these systems, game features like ships, routes, and UI cannot be implemented.

**Why This Matters**:
- Input system enables player control and UI interaction
- Camera system allows viewing and navigating the game world
- ECS provides scalable, data-oriented entity management (50+ ships, 20+ islands)
- Resource management enables efficient asset loading and caching
- Enhanced rendering supports 2D backgrounds + 3D ships

---

## Description

Implement five core engine systems that work together to support game development:

### 1. Input System (`engine_input.h/c`)
**Purpose**: Handle keyboard and mouse input for game controls and UI interaction.

**Features**:
- Key state tracking (pressed, held, released)
- Mouse position and button states
- Input event queue for frame-independent input
- Configurable key bindings

**API Functions** (estimated):
```c
// Keyboard
bool input_is_key_pressed(int key);      // This frame press
bool input_is_key_down(int key);         // Held down
bool input_is_key_released(int key);     // This frame release

// Mouse  
Vector2 input_get_mouse_position(void);
bool input_is_mouse_button_pressed(int button);
bool input_is_mouse_button_down(int button);
float input_get_mouse_wheel_delta(void);

// System
void input_init(void);
void input_update(void);                  // Call each frame
void input_shutdown(void);
```

**Data Structures**:
```c
typedef struct {
    bool keys[512];           // Current key states
    bool keys_previous[512];  // Previous frame states
    Vector2 mouse_position;
    bool mouse_buttons[8];
    bool mouse_buttons_previous[8];
    float mouse_wheel_delta;
} InputState;
```

### 2. Camera System (`engine_camera.h/c`)
**Purpose**: Provide orthographic camera for viewing the 2D/3D game world.

**Features**:
- Orthographic projection for isometric view
- Camera position, zoom, and rotation
- Screen-to-world coordinate conversion
- Camera boundaries and clamping
- Smooth camera movement

**API Functions** (estimated):
```c
// Camera management
void camera_init(float width, float height);
void camera_update(float delta_time);
Camera2D camera_get_raylib_camera(void);  // For Raylib integration

// Positioning
void camera_set_position(Vector2 position);
Vector2 camera_get_position(void);
void camera_set_target(Vector2 target);
void camera_move(Vector2 delta);

// Zoom and rotation
void camera_set_zoom(float zoom);
float camera_get_zoom(void);
void camera_set_rotation(float rotation);

// Coordinate conversion
Vector2 camera_screen_to_world(Vector2 screen_pos);
Vector2 camera_world_to_screen(Vector2 world_pos);

// Boundaries
void camera_set_bounds(Rectangle bounds);
void camera_apply_bounds(void);

// System
void camera_shutdown(void);
```

**Data Structures**:
```c
typedef struct {
    Vector2 position;      // Camera center position
    Vector2 target;        // Target position for smooth movement
    float zoom;            // Zoom level (1.0 = normal)
    float rotation;        // Rotation in degrees
    float smooth_speed;    // Smoothing factor (0-1)
    Rectangle bounds;      // World boundaries
    bool use_bounds;       // Enable boundary clamping
} CameraState;
```

### 3. Entity Component System (`engine_ecs.h/c`)
**Purpose**: Data-oriented entity management for scalable game objects (ships, islands, cargo).

**Features**:
- Component-based architecture (position, velocity, sprite, etc.)
- Entity creation and destruction
- Component storage (Struct-of-Arrays for cache efficiency)
- Entity queries and iteration
- Component add/remove at runtime

**API Functions** (estimated):
```c
// System
void ecs_init(uint32_t max_entities);
void ecs_shutdown(void);

// Entity management
EntityID ecs_entity_create(void);
void ecs_entity_destroy(EntityID entity);
bool ecs_entity_is_alive(EntityID entity);

// Component management
void ecs_add_component(EntityID entity, ComponentType type, void* data);
void* ecs_get_component(EntityID entity, ComponentType type);
void ecs_remove_component(EntityID entity, ComponentType type);
bool ecs_has_component(EntityID entity, ComponentType type);

// Queries
uint32_t ecs_query(ComponentType* required, uint32_t count, EntityID* out_entities);
void ecs_foreach(ComponentType* required, uint32_t count, 
                 void (*callback)(EntityID, void*), void* userdata);
```

**Data Structures**:
```c
typedef uint32_t EntityID;

typedef enum {
    COMPONENT_TRANSFORM = 0,  // Position, rotation, scale
    COMPONENT_VELOCITY,       // Movement vector
    COMPONENT_SPRITE,         // 2D sprite data
    COMPONENT_MODEL,          // 3D model data
    COMPONENT_COLLIDER,       // Collision shape
    COMPONENT_SHIP,           // Ship-specific data
    COMPONENT_CARGO,          // Cargo data
    COMPONENT_COUNT
} ComponentType;

typedef struct {
    Vector2 position;
    float rotation;
    float scale;
} TransformComponent;

typedef struct {
    Vector2 velocity;
    float max_speed;
} VelocityComponent;

typedef struct {
    Texture2D texture;
    Rectangle source_rect;
    Color tint;
} SpriteComponent;

typedef struct {
    uint32_t entity_count;
    uint32_t max_entities;
    bool* entity_alive;                    // Bitset of alive entities
    uint8_t* entity_components[COMPONENT_COUNT];  // Component storage
} ECSWorld;
```

### 4. Resource Management (`engine_resources.h/c`)
**Purpose**: Load, cache, and manage game assets (textures, models, sounds).

**Features**:
- Asset loading from disk (textures, models, sounds, fonts)
- Resource caching (avoid duplicate loads)
- Resource reference counting
- Async loading support (future)
- Resource unloading
- Hot-reloading for development (future)

**API Functions** (estimated):
```c
// System
void resources_init(const char* asset_path);
void resources_shutdown(void);

// Texture loading
Texture2D resources_load_texture(const char* path);
void resources_unload_texture(const char* path);

// Model loading
Model resources_load_model(const char* path);
void resources_unload_model(const char* path);

// Sound loading
Sound resources_load_sound(const char* path);
void resources_unload_sound(const char* path);

// Font loading
Font resources_load_font(const char* path, int font_size);
void resources_unload_font(const char* path);

// Cache management
void resources_clear_cache(void);
uint32_t resources_get_loaded_count(void);
```

**Data Structures**:
```c
typedef enum {
    RESOURCE_TEXTURE,
    RESOURCE_MODEL,
    RESOURCE_SOUND,
    RESOURCE_FONT
} ResourceType;

typedef struct {
    char path[256];
    ResourceType type;
    void* data;           // Actual resource (Texture2D, Model, etc.)
    uint32_t ref_count;   // Reference counting
    bool is_loaded;
} ResourceEntry;

typedef struct {
    ResourceEntry* entries;
    uint32_t entry_count;
    uint32_t max_entries;
    char asset_base_path[256];
} ResourceManager;
```

### 5. Enhanced Rendering (`engine_renderer.h/c` - extend existing)
**Purpose**: Add layered rendering, sprites, and 3D model support.

**New Features**:
- Layered rendering (background, game, UI)
- Sprite rendering with transforms
- 3D model rendering
- Camera integration
- Render queue for sorting

**New API Functions** (estimated):
```c
// Layers
void renderer_begin_layer(int layer);
void renderer_end_layer(void);

// Sprite rendering
void renderer_draw_sprite(Texture2D texture, Vector2 position, 
                          float rotation, float scale, Color tint);
void renderer_draw_sprite_rect(Texture2D texture, Rectangle source, 
                               Vector2 position, float rotation, 
                               float scale, Color tint);

// 3D rendering
void renderer_begin_3d(Camera2D camera);
void renderer_end_3d(void);
void renderer_draw_model(Model model, Vector3 position, 
                         float scale, Color tint);

// Camera
void renderer_set_camera(Camera2D camera);
Camera2D renderer_get_camera(void);

// Render queue
void renderer_submit_draw_call(DrawCall call);
void renderer_flush(void);  // Draw all queued calls
```

**New Data Structures**:
```c
typedef enum {
    LAYER_BACKGROUND = 0,
    LAYER_GAME = 1,
    LAYER_UI = 2,
    LAYER_DEBUG = 3,
    LAYER_COUNT
} RenderLayer;

typedef struct {
    int layer;
    Vector2 position;
    float depth;          // Z-order within layer
    void* data;           // Render-specific data
} DrawCall;

typedef struct {
    DrawCall* draw_calls;
    uint32_t call_count;
    uint32_t max_calls;
    Camera2D active_camera;
} RenderQueue;
```

---

## Definition of Done

Task will be complete when ALL of the following criteria are met:

### Input System ✅
- [ ] `engine_input.h` API defined with all functions documented
- [ ] `engine_input.c` implementation complete
- [ ] Keyboard state tracking (pressed, down, released) working
- [ ] Mouse position and button tracking working
- [ ] Input update called each frame in engine core
- [ ] Unit tests pass (5+ test cases covering key/mouse states)
- [ ] Demo in game: Arrow keys move object, mouse clicks register

### Camera System ✅
- [ ] `engine_camera.h` API defined with all functions documented
- [ ] `engine_camera.c` implementation complete
- [ ] Orthographic camera with position, zoom, rotation working
- [ ] Screen-to-world and world-to-screen conversion accurate
- [ ] Camera boundaries and clamping functional
- [ ] Smooth camera movement implemented
- [ ] Unit tests pass (5+ test cases covering transforms, bounds)
- [ ] Demo in game: WASD moves camera, mouse wheel zooms

### Entity Component System ✅
- [ ] `engine_ecs.h` API defined with all functions documented
- [ ] `engine_ecs.c` implementation complete
- [ ] Entity creation/destruction working
- [ ] Component add/remove/get working
- [ ] Struct-of-Arrays storage implemented for cache efficiency
- [ ] Entity queries functional
- [ ] At least 3 component types defined (Transform, Velocity, Sprite)
- [ ] Unit tests pass (10+ test cases covering entities, components, queries)
- [ ] Demo in game: 10 entities with transform + velocity components moving

### Resource Management ✅
- [ ] `engine_resources.h` API defined with all functions documented
- [ ] `engine_resources.c` implementation complete
- [ ] Texture loading and caching working
- [ ] Model loading and caching working
- [ ] Sound loading and caching working
- [ ] Reference counting prevents duplicate loads
- [ ] Resource unloading functional
- [ ] Unit tests pass (5+ test cases covering load, cache, unload)
- [ ] Demo in game: Load 5 textures, verify only 1 copy in memory for duplicates

### Enhanced Rendering ✅
- [ ] `engine_renderer.h` extended with new API
- [ ] Layered rendering system implemented (background, game, UI)
- [ ] Sprite rendering with transforms working
- [ ] 3D model rendering integrated
- [ ] Camera integration functional
- [ ] Render queue for sorted drawing implemented
- [ ] Unit tests pass (5+ test cases covering layers, sprites, 3D)
- [ ] Demo in game: Background layer + 3D ship model + UI overlay

### Integration ✅
- [ ] All systems integrated in `engine_core.c` (init, update, shutdown)
- [ ] No memory leaks (verified with valgrind/Dr. Memory)
- [ ] Game still runs at 60 FPS with all systems active
- [ ] Comprehensive integration test covering all systems together
- [ ] Demo game updated to showcase all new features

### Documentation ✅
- [ ] All header files have complete API documentation
- [ ] `docs/ENGINE_ARCHITECTURE.md` created explaining each system
- [ ] `docs/ECS_GUIDE.md` created with ECS usage examples
- [ ] Code examples added for each system
- [ ] SETUP_SUMMARY.md updated with Phase 2 completion
- [ ] Jira ticket MST-3 updated with final results

---

## Test Plan

### Unit Tests

Each system will have dedicated unit tests:

#### Input System Tests
```cpp
TEST(InputTests, KeyPressedSingleFrame)
TEST(InputTests, KeyHeldMultipleFrames)
TEST(InputTests, KeyReleasedDetection)
TEST(InputTests, MouseButtonStates)
TEST(InputTests, MousePositionTracking)
```
**Expected**: All input state transitions work correctly

#### Camera System Tests
```cpp
TEST(CameraTests, PositionAndZoom)
TEST(CameraTests, ScreenToWorldConversion)
TEST(CameraTests, WorldToScreenConversion)
TEST(CameraTests, BoundaryClampingEnabled)
TEST(CameraTests, BoundaryClampingDisabled)
TEST(CameraTests, SmoothMovement)
```
**Expected**: All camera operations and transforms accurate

#### ECS Tests
```cpp
TEST(ECSTests, EntityCreationAndDestruction)
TEST(ECSTests, ComponentAddGetRemove)
TEST(ECSTests, MultipleComponents)
TEST(ECSTests, EntityQueries)
TEST(ECSTests, EntityIteration)
TEST(ECSTests, ComponentStorageSeparation)
TEST(ECSTests, MaxEntitiesHandling)
TEST(ECSTests, ComponentNotFound)
TEST(ECSTests, InvalidEntityHandling)
TEST(ECSTests, ForeachCallback)
```
**Expected**: All ECS operations work, queries return correct results

#### Resource Management Tests
```cpp
TEST(ResourceTests, TextureLoadAndCache)
TEST(ResourceTests, DuplicateLoadReturnsSame)
TEST(ResourceTests, ModelLoading)
TEST(ResourceTests, SoundLoading)
TEST(ResourceTests, ResourceUnload)
TEST(ResourceTests, CacheClear)
```
**Expected**: Resources load correctly, duplicates cached, unload works

#### Rendering Tests
```cpp
TEST(RendererTests, LayerRendering)
TEST(RendererTests, SpriteDrawing)
TEST(RendererTests, 3DModelDrawing)
TEST(RendererTests, CameraIntegration)
TEST(RendererTests, RenderQueueOrdering)
```
**Expected**: All rendering operations work, layers render in order

### Integration Tests

Test all systems working together:

```cpp
TEST(IntegrationTests, AllSystemsInit)
TEST(IntegrationTests, InputControlsCamera)
TEST(IntegrationTests, CameraAffectsRendering)
TEST(IntegrationTests, ECSEntitiesRender)
TEST(IntegrationTests, ResourcesUsedByECS)
TEST(IntegrationTests, CompleteFrameSimulation)
```
**Expected**: Systems integrate cleanly, no conflicts

### Performance Tests

Verify system performance meets requirements:

- **ECS**: Create 1000 entities with 3 components, iterate in <1ms
- **Resources**: Load 50 textures in <100ms (cached load <1ms)
- **Rendering**: Render 100 sprites + 10 models at 60 FPS
- **Input**: Input polling overhead <0.1ms per frame
- **Camera**: Transform calculations <0.5ms per frame

**Expected**: All performance targets met at 60 FPS

### Manual Testing

Visual verification of systems:

1. **Input Demo**:
   - Arrow keys move a square on screen
   - Mouse clicks spawn new squares
   - Mouse wheel changes square size
   - **Expected**: Responsive controls, no input lag

2. **Camera Demo**:
   - WASD moves camera around a large grid
   - Mouse wheel zooms in/out
   - Camera stays within bounds
   - **Expected**: Smooth camera movement, correct zoom

3. **ECS Demo**:
   - 20 entities with Transform + Velocity components
   - Entities move across screen and wrap around
   - Each entity has different color (Sprite component)
   - **Expected**: Smooth movement, no stuttering

4. **Resource Demo**:
   - Load ship textures from assets/textures/
   - Load ship model from assets/models/
   - Verify only one copy in memory
   - **Expected**: Fast loading, correct images displayed

5. **Rendering Demo**:
   - Background layer: Textured background
   - Game layer: 3D ship models moving
   - UI layer: Text overlay with FPS counter
   - **Expected**: Correct layering, 60 FPS maintained

6. **Integrated Demo**:
   - Combine all systems in working game prototype
   - Input controls camera
   - Camera views ECS entities
   - Entities use resources (textures, models)
   - All rendered in layers
   - **Expected**: Cohesive experience, 60 FPS

---

## Technical Approach

### Input System Implementation
- Use Raylib's input functions (IsKeyPressed, GetMousePosition, etc.)
- Store previous frame state to detect transitions
- Update input state in engine_core's begin_frame

### Camera System Implementation
- Use Raylib's Camera2D struct internally
- Implement smooth lerp for position changes
- Matrix math for coordinate transforms
- Clamp position to boundaries if enabled

### ECS Implementation
- **Sparse Set** approach for entity IDs (fast add/remove)
- **Struct-of-Arrays** for components (cache-friendly iteration)
- Generational indices to avoid use-after-free
- Bitset for component masks (fast queries)

### Resource Management Implementation
- **Hash map** for fast resource lookup by path
- Store Raylib resource handles (Texture2D, Model, Sound)
- Reference counting for shared resources
- Base path resolution for asset location

### Rendering Enhancement Implementation
- Extend existing renderer with layer support
- Maintain separate draw queues per layer
- Sort draw calls by depth within layer
- Integrate Raylib's BeginMode2D/BeginMode3D for camera

---

## File Changes

### New Files (10 files)
1. **engine/include/engine_input.h** - Input API
2. **engine/src/engine_input.c** - Input implementation
3. **engine/include/engine_camera.h** - Camera API
4. **engine/src/engine_camera.c** - Camera implementation
5. **engine/include/engine_ecs.h** - ECS API
6. **engine/src/engine_ecs.c** - ECS implementation
7. **engine/include/engine_resources.h** - Resource API
8. **engine/src/engine_resources.c** - Resource implementation
9. **tests/input_tests.cpp** - Input unit tests
10. **tests/camera_tests.cpp** - Camera unit tests
11. **tests/ecs_tests.cpp** - ECS unit tests
12. **tests/resource_tests.cpp** - Resource unit tests
13. **tests/renderer_tests.cpp** - Renderer unit tests
14. **tests/integration_tests.cpp** - Integration tests

### Modified Files (5 files)
15. **engine/include/engine_core.h** - Add new system init calls
16. **engine/src/engine_core.c** - Integrate new systems in lifecycle
17. **engine/include/engine_renderer.h** - Add new rendering functions
18. **engine/src/engine_renderer.c** - Implement layered rendering
19. **game/src/main.c** - Update to demonstrate all systems

### New Documentation (2 files)
20. **docs/ENGINE_ARCHITECTURE.md** - System architecture guide
21. **docs/ECS_GUIDE.md** - ECS usage tutorial

---

## Dependencies

### No New External Dependencies
All systems use existing dependencies:
- **Raylib 5.0**: Input handling, camera, rendering
- **C11 Standard Library**: Memory, math, strings

### Internal Dependencies
- Input system: None (independent)
- Camera system: None (independent)
- ECS: None (independent)
- Resources: Raylib for loading
- Renderer: All systems (input for debug, camera for transforms, ECS for entities, resources for assets)

---

## Success Metrics

### Code Quality
- [ ] Zero compiler warnings
- [ ] All unit tests passing (40+ tests)
- [ ] Code coverage >80% for new systems
- [ ] Static analysis clean (cppcheck)

### Performance
- [ ] 60 FPS with 100 entities
- [ ] <5ms per frame for all engine systems
- [ ] <50MB memory usage
- [ ] Zero memory leaks

### Functionality
- [ ] All systems working independently
- [ ] All systems integrated correctly
- [ ] Demo game showcases all features
- [ ] Input responsive (<16ms latency)

### Documentation
- [ ] Every public function documented
- [ ] Architecture guide complete
- [ ] ECS guide with examples
- [ ] Jira ticket updated

---

## Risk Assessment

### High Risk
- **ECS Performance**: Struct-of-Arrays implementation complexity
  - **Mitigation**: Start simple, profile, optimize incrementally
  - **Fallback**: Array-of-Structs if SoA too complex

### Medium Risk
- **System Integration**: Five systems may have unexpected interactions
  - **Mitigation**: Implement one system at a time, test integration early
  - **Fallback**: Simplify interfaces if conflicts arise

- **Camera Math**: Coordinate transforms prone to errors
  - **Mitigation**: Extensive unit tests for all transform cases
  - **Fallback**: Use Raylib's GetScreenToWorld2D/GetWorldToScreen2D

### Low Risk
- **Input System**: Straightforward wrapper around Raylib
- **Resource Management**: Standard hash map + reference counting pattern
- **Rendering**: Extends existing working renderer

---

## Implementation Plan

### Phase A: Independent Systems (3-4 hours)
1. Input system (header → impl → tests)
2. Camera system (header → impl → tests)
3. Resource management (header → impl → tests)

### Phase B: ECS (3-4 hours)
4. ECS data structures
5. ECS core functions (entity, component management)
6. ECS queries and iteration
7. ECS unit tests

### Phase C: Rendering (2 hours)
8. Extend renderer with layers
9. Sprite and model rendering
10. Camera integration
11. Renderer tests

### Phase D: Integration (2 hours)
12. Integrate all systems in engine_core
13. Update game demo
14. Integration tests
15. Performance profiling

### Phase E: Documentation (1 hour)
16. Complete API docs
17. Write architecture guide
18. Write ECS guide
19. Update Jira ticket

---

## Questions for Planning

Before starting implementation, consider:

1. **ECS Component Limit**: How many component types? (current plan: 16)
2. **ECS Max Entities**: How many entities? (current plan: 1024)
3. **Resource Cache Size**: Max cached resources? (current plan: 256)
4. **Render Queue Size**: Max draw calls per frame? (current plan: 1000)
5. **Camera Bounds**: Hard boundaries or soft? (current plan: hard clamp)

---

## Next Ticket Preview: MST-4 - Game Mechanics

After MST-3, we'll have engine foundation ready. MST-4 will implement:
- Ship entities with movement
- Island/port placement
- Route planning
- Basic cargo system

---

## Status: Ready for Development

**Prerequisites Met**:
- ✅ MST-2 complete (build system, basic engine)
- ✅ Planning complete (detailed specification)
- ✅ API design reviewed
- ✅ Test plan defined

**Next Action**: Assign ticket and begin Phase A implementation.

---

**Ticket Created**: 2026-01-24  
**Estimated Completion**: TBD (8-12 hours development time)  
**Priority**: High (blocks game feature development)
