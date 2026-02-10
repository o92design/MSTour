# POI (Point of Interest) System

## Overview

The **POI (Point of Interest) System** is a core gameplay feature in MS Tour that enhances the tourism experience by introducing discoverable locations throughout the Gothenburg Archipelago. Players earn passenger satisfaction bonuses by visiting these landmarks during tours, encouraging strategic route planning and exploration.

POIs integrate with multiple game systems:
- **Tourism Gameplay**: Visiting POIs increases passenger satisfaction and tour revenue
- **Fog of War**: POIs are revealed as ships explore the archipelago
- **Route Planning**: Players can create routes that maximize POI visits
- **Progression**: Discovery and visit tracking supports achievements and statistics

The system is fully data-driven using JSON configuration files, allowing modders to easily add custom POIs without code changes.

---

## POI Types and Tiers

### Two-Tier Rarity System

POIs are categorized into two tiers based on importance and satisfaction bonuses:

#### **Tier 1: General POIs**
- **Purpose**: Common landmarks providing modest satisfaction bonuses
- **Default Satisfaction Bonus**: +5 points
- **Examples**: Beaches, scenic viewpoints, small villages, fishing harbors
- **Gameplay Role**: Frequent stops for routine tours

#### **Tier 2: Special POIs**
- **Purpose**: Rare, historically significant landmarks providing substantial bonuses
- **Default Satisfaction Bonus**: +15 points
- **Examples**: Famous lighthouses, major fortresses, nature reserves, VIP-related locations
- **Gameplay Role**: Premium destinations for high-value tours

### Three Category System

POIs are classified into three thematic categories that appeal to different passenger interests:

#### **Nature** (`POI_TYPE_NATURE`)
- Natural beauty, wildlife, and scenic areas
- Examples: Nature reserves, beaches, cliffs, hidden coves
- Multiplier: 1.0× (base satisfaction)

#### **Historical** (`POI_TYPE_HISTORICAL`)
- Historic buildings, monuments, and cultural sites
- Examples: Fishing villages, traditional harbors, heritage buildings
- Multiplier: 1.2× (increased satisfaction)

#### **Military** (`POI_TYPE_MILITARY`)
- Fortifications, naval sites, and strategic defense positions
- Examples: Coastal fortresses, gun batteries, quarantine stations
- Multiplier: 1.1× (moderate increase)

### Variety Bonuses

Tours that visit multiple POI categories receive additional satisfaction bonuses:
- **2 Different Types**: +5 satisfaction bonus
- **3 Different Types** (all categories): +15 satisfaction bonus

This encourages diverse tour routes rather than repeatedly visiting the same POI type.

---

## Advanced POI Viewing System

The Advanced POI Viewing System enhances tourism gameplay by rewarding players for **proper sightseeing technique**. Instead of simply sailing through a POI's radius, players must demonstrate good viewing behavior to trigger a visit and earn satisfaction bonuses.

### Dual Trigger System

POI visits can be triggered in two ways:

#### 1. **Legacy Close Proximity** (Immediate Trigger)
- Ship enters the POI's **base radius** (e.g., 50 units)
- Triggers instantly regardless of speed or orientation
- Represents "accidental" or "close encounter" visits
- Simple fallback for gameplay accessibility

#### 2. **Advanced Viewing** (Quality Sightseeing)
- Ship must satisfy **three criteria simultaneously**:
  - **Distance**: Within **4× POI radius** (e.g., 200 units for a 50-unit radius POI)
  - **Speed**: Below **60.0 units/second** (slow cruising speed)
  - **Angle**: **Broadside orientation** to the POI (passengers can see from ship sides)
- Represents proper tourist viewing behavior
- Encourages strategic route planning and ship maneuvering

### Viewing Criteria Details

#### Distance Criterion
```c
#define POI_VIEW_MAX_DIST_MULT 4.0f  // View range = POI radius × 4
```
- **Purpose**: POI must be within visible range, but not necessarily touching
- **Example**: POI with 50-unit radius → viewable up to 200 units away
- **Gameplay**: Allows passing ships to "view" landmarks from a distance

#### Speed Criterion
```c
#define POI_VIEW_MAX_SPEED 60.0f  // Units per second
```
- **Purpose**: Ship must be moving slow enough for passengers to observe
- **Typical Ship Speeds**:
  - Full Speed: 80-100 units/sec (too fast for sightseeing)
  - Cruising Speed: 40-60 units/sec (ideal for viewing)
  - Slow Speed: 20-30 units/sec (also acceptable)
- **Gameplay**: Encourages players to slow down near landmarks

#### Angle Criterion (Broadside Viewing)
```c
#define POI_VIEW_ALIGNMENT_TOLERANCE 0.7f  // Dot product threshold
```
- **Purpose**: Ship must be oriented so passengers can see the POI
- **Calculation**: Uses dot product of ship forward vector and vector-to-POI
  - `alignment = |dot(ship_forward, to_poi)|`
  - `0.0` = Perfect broadside (ship perpendicular to POI) ✅
  - `1.0` = Head-on or stern-on (POI directly ahead/behind) ❌
- **Threshold**: `alignment ≤ 0.7` passes the check
- **Gameplay**: Players must turn the ship to present the broadside to the POI

### Quality Score Calculation

The system calculates a **quality score** (0.0 to 1.0) when all criteria are met:

```c
quality_score = (dist_score × 0.3) + (speed_score × 0.3) + (angle_score × 0.4)
```

**Component Scores**:
- **Distance Score**: `1.0 - (distance / view_radius)` (closer is better, up to base radius)
- **Speed Score**: `1.0 - (current_speed / max_speed)` (slower is better)
- **Angle Score**: `1.0 - (alignment / tolerance)` (more broadside is better)

**Weights**:
- Distance: 30% (less important - viewing range is generous)
- Speed: 30% (moderate importance)
- Angle: 40% (most important - demonstrates intentional viewing)

**Usage**: Quality score can be used for:
- Tiered satisfaction bonuses (higher quality = more satisfaction)
- UI feedback (show players how good their viewing is)
- Achievement tracking (e.g., "Perfect View" for quality ≥ 0.9)
- Future gameplay mechanics (e.g., photography minigame)

### Example Scenarios

#### ✅ **Successful Viewing**
```
Ship Position: (400, 300)
POI Position: (450, 320)
Distance: 51 units (within 4× radius of 50)
Ship Speed: 45 units/sec (below 60 threshold)
Ship Rotation: 45° (facing northeast)
POI Direction: East (90°)
Alignment: |cos(45° - 90°)| = 0.707 (close to tolerance)
Result: ✅ Visit triggered!
Quality: ~0.65 (good viewing)
```

#### ❌ **Too Fast**
```
Ship Speed: 85 units/sec (exceeds 60 threshold)
Result: ❌ No visit (even if other criteria met)
Player Feedback: "Slow down to view the landmark!"
```

#### ❌ **Wrong Angle**
```
Ship heading directly toward POI (alignment = 1.0)
Result: ❌ No visit (POI is in front, not on sides)
Player Feedback: "Turn broadside to give passengers a better view!"
```

#### ✅ **Legacy Proximity Override**
```
Ship Position: (450, 320) (within base radius)
Result: ✅ Visit triggered regardless of speed/angle
Note: Close proximity always works as fallback
```

### Implementation Notes

- **Performance**: Angle calculation only performed when ship is in range (optimization)
- **Fallback**: Legacy proximity ensures casual players aren't blocked from progression
- **Per-Frame Check**: System updates every frame to catch brief viewing windows
- **One-Time Visits**: Once a POI is marked visited, it doesn't trigger again during that tour

---

## JSON Schema

### File Format: `assets/data/pois.json`

POIs are loaded from a JSON configuration file. The schema is designed for easy modding and localization.

#### Root Structure

```json
{
  "version": "1.0",
  "description": "Points of Interest in the Gothenburg Archipelago",
  "pois": [ /* array of POI objects */ ]
}
```

#### POI Object Schema

```json
{
  "id": "unique_poi_identifier",
  "name": "Display Name",
  "type": "nature|historical|military",
  "tier": "general|special",
  "position": { "x": 0.0, "y": 0.0 },
  "radius": 50.0,
  "satisfaction_bonus": 10,
  "description": "Long description shown to players"
}
```

#### Field Specifications

| Field | Type | Required | Description |
|-------|------|----------|-------------|
| `id` | string | Yes | Unique identifier (lowercase, snake_case recommended) |
| `name` | string | Yes | Display name shown in UI (max 64 chars) |
| `type` | string | Yes | Category: `"nature"`, `"historical"`, or `"military"` |
| `tier` | string | Yes | Rarity: `"general"` or `"special"` |
| `position` | object | Yes | World coordinates: `{ "x": float, "y": float }` |
| `radius` | float | Optional | Visit detection radius (default: 50.0) |
| `satisfaction_bonus` | int | Optional | Custom satisfaction value (default: tier-based) |
| `description` | string | Yes | Long description for UI (max 256 chars) |

#### Example POI Entry

```json
{
  "id": "poi_vinga",
  "name": "Vinga Lighthouse",
  "type": "historical",
  "tier": "special",
  "position": { "x": 800.0, "y": 200.0 },
  "radius": 60.0,
  "satisfaction_bonus": 20,
  "description": "Historic lighthouse marking the entrance to Gothenburg harbor"
}
```

#### Validation Rules

- **Unique IDs**: No two POIs can share the same `id`
- **Type Values**: Must be one of: `nature`, `historical`, `military` (case-insensitive)
- **Tier Values**: Must be one of: `general`, `special` (case-insensitive)
- **Position**: World coordinates in game units (typically 0-1000 range)
- **Radius**: Positive float, typical range 30.0-100.0
- **Satisfaction Bonus**: Positive integer, typical range 5-25
- **String Lengths**: 
  - `name`: max 64 characters
  - `description`: max 256 characters

#### Loading Behavior

1. **File Path**: `assets/data/pois.json` (relative to executable)
2. **Default Fallback**: If file not found, 10 placeholder POIs load automatically
3. **Parse Errors**: Logged with line number; system continues with defaults
4. **Invalid Fields**: Use sensible defaults (e.g., missing radius → 50.0)
5. **Duplicate IDs**: Later entries overwrite earlier ones (warning logged)

---

## API Reference

### POI ECS Module (`game_poi_ecs.h`)

The core POI entity-component-system managing POI data and lifecycle.

#### POI Creation

##### `poi_ecs_init`
```c
void poi_ecs_init(POIEcsWorld* poi_world);
```
**Description**: Initialize the POI ECS world. Must be called before any POI operations.

**Parameters**:
- `poi_world`: Pointer to POIEcsWorld structure to initialize

**Example**:
```c
POIEcsWorld poi_world;
poi_ecs_init(&poi_world);
```

---

##### `poi_ecs_create`
```c
int poi_ecs_create(POIEcsWorld* poi_world, const POICreateParams* params);
```
**Description**: Create a new POI in the world.

**Parameters**:
- `poi_world`: POI ECS world
- `params`: POI creation parameters

**Returns**: POI index (0 to MAX_POIS-1) on success, -1 on failure

**POICreateParams Structure**:
```c
typedef struct POICreateParams {
    const char* name;
    const char* description;
    POIType type;
    POITier tier;
    float x;
    float y;
    float radius;                   // 0 = use default (50.0)
    int satisfaction_bonus;         // 0 = use tier default
} POICreateParams;
```

**Example**:
```c
POICreateParams params = {
    .name = "Vinga Lighthouse",
    .description = "Historic lighthouse marking harbor entrance",
    .type = POI_TYPE_HISTORICAL,
    .tier = POI_TIER_SPECIAL,
    .x = 800.0f,
    .y = 200.0f,
    .radius = 60.0f,
    .satisfaction_bonus = 20
};
int poi_index = poi_ecs_create(&poi_world, &params);
```

---

##### `poi_ecs_destroy`
```c
void poi_ecs_destroy(POIEcsWorld* poi_world, int poi_index);
```
**Description**: Remove a POI from the world (swap-and-pop).

**Parameters**:
- `poi_world`: POI ECS world
- `poi_index`: Valid POI index to destroy

---

##### `poi_ecs_clear`
```c
void poi_ecs_clear(POIEcsWorld* poi_world);
```
**Description**: Remove all POIs from the world (useful for reloading).

---

#### POI Queries

##### `poi_ecs_find_at_position`
```c
int poi_ecs_find_at_position(const POIEcsWorld* poi_world, float x, float y);
```
**Description**: Find a POI at the given world position (within its radius).

**Returns**: POI index if found, -1 if no POI at position

**Example**:
```c
float mouse_x = 450.0f, mouse_y = 320.0f;
int poi_idx = poi_ecs_find_at_position(&poi_world, mouse_x, mouse_y);
if (poi_idx >= 0) {
    const char* name = poi_ecs_get_name(&poi_world, poi_idx);
    printf("Found POI: %s\n", name);
}
```

---

##### `poi_ecs_find_in_range`
```c
int poi_ecs_find_in_range(const POIEcsWorld* poi_world, float x, float y, 
                          float range, int* out_indices, int max_results);
```
**Description**: Find all POIs within a certain range of a position.

**Parameters**:
- `x`, `y`: World position to search from
- `range`: Search radius
- `out_indices`: Array to fill with POI indices
- `max_results`: Maximum POIs to return

**Returns**: Number of POIs found (0 to max_results)

**Example**:
```c
int nearby_pois[10];
float ship_x = 500.0f, ship_y = 400.0f;
int count = poi_ecs_find_in_range(&poi_world, ship_x, ship_y, 
                                   200.0f, nearby_pois, 10);
printf("Found %d POIs within 200 units\n", count);
```

---

##### `poi_ecs_find_by_name`
```c
int poi_ecs_find_by_name(const POIEcsWorld* poi_world, const char* name);
```
**Description**: Find a POI by name (case-insensitive).

**Returns**: POI index if found, -1 if not found

---

##### `poi_ecs_check_visit`
```c
bool poi_ecs_check_visit(const POIEcsWorld* poi_world, int poi_index, 
                         float x, float y);
```
**Description**: Check if a position is within a POI's visit radius (legacy proximity check).

**Returns**: `true` if position triggers a visit, `false` otherwise

**Note**: This only checks close proximity (base radius). For advanced viewing criteria, use `poi_ecs_get_view_status()`.

**Example**:
```c
bool is_visiting = poi_ecs_check_visit(&poi_world, poi_idx, ship_x, ship_y);
if (is_visiting && !poi_ecs_is_visited(&poi_world, poi_idx)) {
    poi_ecs_set_visited(&poi_world, poi_idx, true);
    poi_ecs_increment_visits(&poi_world, poi_idx);
}
```

---

##### `poi_ecs_get_view_status`
```c
POIViewStatus poi_ecs_get_view_status(const POIEcsWorld* poi_world, int poi_index, 
                                     float ship_x, float ship_y,
                                     float ship_rot, float ship_speed);
```
**Description**: Calculate detailed view status for a ship observing a POI. Returns comprehensive information about viewing quality, including whether the ship meets all criteria for a proper view.

**Parameters**:
- `poi_world`: POI ECS world
- `poi_index`: Valid POI index
- `ship_x`, `ship_y`: Ship world position
- `ship_rot`: Ship rotation in degrees (0° = North, clockwise)
- `ship_speed`: Ship speed in units/second

**Returns**: `POIViewStatus` struct with detailed viewing information

**POIViewStatus Structure**:
```c
typedef struct POIViewStatus {
    bool in_range;           // Within view range (4× POI radius)
    bool speed_ok;           // Speed ≤ 60.0 units/sec
    bool angle_ok;           // Broadside orientation (alignment ≤ 0.7)
    bool is_viewable;        // All three conditions met
    
    float distance;          // Current distance to POI center
    float current_speed;     // Current ship speed
    float alignment;         // 0.0 = broadside (good), 1.0 = head-on (bad)
    float quality_score;     // 0.0 to 1.0 overall viewing quality
} POIViewStatus;
```

**Field Descriptions**:

| Field | Type | Description |
|-------|------|-------------|
| `in_range` | bool | Ship is within viewing range (4× POI radius) |
| `speed_ok` | bool | Ship speed ≤ 60.0 units/sec (slow enough to view) |
| `angle_ok` | bool | Ship is broadside to POI (alignment ≤ 0.7) |
| `is_viewable` | bool | All three criteria met (would trigger visit) |
| `distance` | float | Distance from ship to POI center |
| `current_speed` | float | Ship's current speed |
| `alignment` | float | Dot product of forward vector and to-POI vector (absolute value) |
| `quality_score` | float | Overall viewing quality (0.0-1.0), only valid if `is_viewable` is true |

**Example Usage**:
```c
// Check if ship is viewing a POI correctly
POIViewStatus status = poi_ecs_get_view_status(&poi_world, poi_idx,
                                                ship_x, ship_y,
                                                ship_rotation, ship_speed);

if (status.is_viewable) {
    printf("Good view! Quality: %.2f\n", status.quality_score);
    
    // Trigger visit if not already visited
    if (!poi_ecs_is_visited(&poi_world, poi_idx)) {
        poi_ecs_set_visited(&poi_world, poi_idx, true);
        poi_ecs_increment_visits(&poi_world, poi_idx);
    }
} else {
    // Provide player feedback on what's wrong
    if (!status.in_range) {
        printf("Too far away (%.1f > %.1f)\n", 
               status.distance, poi_ecs_get_radius(&poi_world, poi_idx) * 4.0f);
    }
    if (!status.speed_ok) {
        printf("Slow down! (%.1f > 60.0)\n", status.current_speed);
    }
    if (!status.angle_ok) {
        printf("Turn broadside! (alignment: %.2f)\n", status.alignment);
    }
}
```

**Advanced Usage - Tiered Bonuses**:
```c
// Award satisfaction based on viewing quality
if (status.is_viewable) {
    int base_bonus = poi_ecs_get_satisfaction_bonus(&poi_world, poi_idx);
    
    if (status.quality_score >= 0.9f) {
        printf("Perfect view! +%d satisfaction\n", base_bonus + 5);
    } else if (status.quality_score >= 0.7f) {
        printf("Great view! +%d satisfaction\n", base_bonus + 2);
    } else {
        printf("Good view! +%d satisfaction\n", base_bonus);
    }
}
```

**UI Feedback Example**:
```c
// Real-time viewing indicator for UI
POIViewStatus status = poi_ecs_get_view_status(&poi_world, poi_idx,
                                                ship_x, ship_y,
                                                ship_rotation, ship_speed);

// Draw viewing quality bar
if (status.in_range) {
    DrawText("Distance: OK", 10, 10, 20, status.in_range ? GREEN : RED);
    DrawText("Speed: OK", 10, 35, 20, status.speed_ok ? GREEN : RED);
    DrawText("Angle: OK", 10, 60, 20, status.angle_ok ? GREEN : RED);
    
    if (status.is_viewable) {
        int bar_width = (int)(status.quality_score * 200.0f);
        DrawRectangle(10, 85, bar_width, 20, GREEN);
        DrawText("Viewing Quality", 10, 110, 20, WHITE);
    }
}
```

**Performance Note**: The angle calculation (involving trigonometric functions) is only performed when the ship is within range, minimizing overhead for distant POIs.

---

#### POI Accessors

##### `poi_ecs_get_count`
```c
uint32_t poi_ecs_get_count(const POIEcsWorld* poi_world);
```
**Returns**: Number of active POIs in the world

---

##### `poi_ecs_get_name`
```c
const char* poi_ecs_get_name(const POIEcsWorld* poi_world, int poi_index);
```
**Returns**: POI display name (null-terminated string)

---

##### `poi_ecs_get_type`
```c
POIType poi_ecs_get_type(const POIEcsWorld* poi_world, int poi_index);
```
**Returns**: POI category (`POI_TYPE_NATURE`, `POI_TYPE_HISTORICAL`, `POI_TYPE_MILITARY`)

---

##### `poi_ecs_get_tier`
```c
POITier poi_ecs_get_tier(const POIEcsWorld* poi_world, int poi_index);
```
**Returns**: POI rarity tier (`POI_TIER_GENERAL`, `POI_TIER_SPECIAL`)

---

##### `poi_ecs_get_position`
```c
void poi_ecs_get_position(const POIEcsWorld* poi_world, int poi_index, 
                          float* x, float* y);
```
**Description**: Get POI world position.

**Parameters**:
- `x`, `y`: Output pointers for coordinates

---

##### `poi_ecs_get_satisfaction_bonus`
```c
int poi_ecs_get_satisfaction_bonus(const POIEcsWorld* poi_world, int poi_index);
```
**Returns**: Satisfaction points awarded for visiting this POI

---

##### `poi_ecs_is_visited`
```c
bool poi_ecs_is_visited(const POIEcsWorld* poi_world, int poi_index);
```
**Returns**: `true` if the player has visited this POI at least once

---

##### `poi_ecs_is_discovered`
```c
bool poi_ecs_is_discovered(const POIEcsWorld* poi_world, int poi_index);
```
**Returns**: `true` if the POI is visible through fog of war

---

#### POI State Modification

##### `poi_ecs_set_visited`
```c
void poi_ecs_set_visited(POIEcsWorld* poi_world, int poi_index, bool visited);
```
**Description**: Mark a POI as visited (or unvisited for testing).

---

##### `poi_ecs_set_discovered`
```c
void poi_ecs_set_discovered(POIEcsWorld* poi_world, int poi_index, bool discovered);
```
**Description**: Mark a POI as discovered (revealed through fog).

---

##### `poi_ecs_increment_visits`
```c
void poi_ecs_increment_visits(POIEcsWorld* poi_world, int poi_index);
```
**Description**: Increment the total visit count for statistics tracking.

---

#### POI System Update

##### `poi_ecs_system_update`
```c
void poi_ecs_system_update(POIEcsWorld* poi_world, const ECSWorld* ecs_world,
                           ComponentMask ship_mask, const POISystemContext* context);
```
**Description**: Update POI system - checks for ship visits using the **dual-trigger system** and fires callbacks when visits occur. This function should be called every frame in the game loop.

**Dual-Trigger System**:

The system checks two visit conditions for each ship-POI pair:

1. **Legacy Close Proximity**: Ship enters POI's base radius
   - Simple distance check: `distance ≤ poi_radius`
   - Always works regardless of speed or orientation
   - Ensures casual gameplay accessibility

2. **Advanced Viewing**: Ship demonstrates proper sightseeing behavior
   - Distance: Within 4× POI radius
   - Speed: Below 60.0 units/second
   - Angle: Broadside orientation (alignment ≤ 0.7)
   - All three criteria must be met simultaneously

**A visit is triggered if EITHER condition is satisfied** (logical OR). Once a POI is visited, it is marked and won't trigger again during that tour session.

**Parameters**:
- `poi_world`: POI ECS world
- `ecs_world`: Main ECS world containing ships (must have Transform and Velocity components)
- `ship_mask`: Component mask for ship entities
- `context`: System context with callbacks (optional, can be NULL)

**POISystemContext Structure**:
```c
typedef struct POISystemContext {
    POIVisitCallback on_visit;      // Called when ship visits POI
    void* user_data;                // User data for callback
} POISystemContext;

typedef void (*POIVisitCallback)(int poi_index, Entity visitor, void* user_data);
```

**Visit Callback**: The `on_visit` callback is invoked once per unique POI visit with:
- `poi_index`: Index of the visited POI
- `visitor`: Entity ID of the visiting ship
- `user_data`: Custom data passed through context

**Example - Basic Usage**:
```c
void on_poi_visit(int poi_index, Entity ship, void* user_data) {
    printf("Ship %u visited POI: %s\n", ship, 
           poi_ecs_get_name(&poi_world, poi_index));
    // Update satisfaction, play sound, etc.
}

POISystemContext context = {
    .on_visit = on_poi_visit,
    .user_data = NULL
};

// Call every frame in game loop
poi_ecs_system_update(&poi_world, &ecs_world, ship_mask, &context);
```

**Example - With Custom Data**:
```c
typedef struct GameState {
    int total_visits;
    float total_satisfaction;
} GameState;

void on_poi_visit(int poi_index, Entity ship, void* user_data) {
    GameState* state = (GameState*)user_data;
    state->total_visits++;
    
    int bonus = poi_ecs_get_satisfaction_bonus(&poi_world, poi_index);
    state->total_satisfaction += bonus;
    
    printf("Total visits: %d, Total satisfaction: %.1f\n",
           state->total_visits, state->total_satisfaction);
}

GameState game_state = {0};
POISystemContext context = {
    .on_visit = on_poi_visit,
    .user_data = &game_state
};

poi_ecs_system_update(&poi_world, &ecs_world, ship_mask, &context);
```

**Example - Quality-Based Bonuses**:
```c
void on_poi_visit_quality(int poi_index, Entity ship, void* user_data) {
    // Get ship data
    float ship_x = ecs_world.transforms.pos_x[ship];
    float ship_y = ecs_world.transforms.pos_y[ship];
    float ship_rot = ecs_world.transforms.rotation[ship];
    float ship_speed = ecs_world.velocities.speed[ship];
    
    // Check how good the view was
    POIViewStatus status = poi_ecs_get_view_status(&poi_world, poi_index,
                                                     ship_x, ship_y,
                                                     ship_rot, ship_speed);
    
    int base_bonus = poi_ecs_get_satisfaction_bonus(&poi_world, poi_index);
    int final_bonus = base_bonus;
    
    if (status.is_viewable && status.quality_score >= 0.9f) {
        final_bonus = (int)(base_bonus * 1.5f); // +50% for perfect viewing
        printf("Perfect view! +%d satisfaction (quality: %.2f)\n",
               final_bonus, status.quality_score);
    } else {
        printf("Standard visit: +%d satisfaction\n", final_bonus);
    }
    
    // Apply bonus to game state
    satisfaction_record_poi_visit(&tour, &poi_world, poi_index);
}
```

**Implementation Details**:
- **Per-Frame Processing**: Iterates all entities with ship mask and Transform component
- **Distance Check First**: Performs cheap distance check before expensive viewing calculations
- **One-Time Visits**: Uses `poi_world->pois.visited[i]` flag to prevent duplicate callbacks
- **Visit Counter**: Increments `poi_world->pois.visit_count[i]` for statistics
- **No Callback**: If `context` is NULL or `on_visit` is NULL, visits are still tracked but no callback fires

**Performance Considerations**:
- O(ships × POIs) complexity per frame
- Distance checks use squared distance (no sqrt) until needed
- Angle calculations only performed when ship is in range
- Typical performance: < 0.1ms for 10 ships × 20 POIs on modern hardware

**Integration with Tour System**:
```c
// Game loop integration
void game_update(float delta_time) {
    // Update ship physics
    ship_system_update(&ship_world, delta_time);
    
    // Check for POI visits
    POISystemContext context = {
        .on_visit = tour_on_poi_visit,  // Updates tour satisfaction
        .user_data = &current_tour
    };
    poi_ecs_system_update(&poi_world, &ecs_world, ship_mask, &context);
    
    // Update satisfaction UI
    satisfaction_update_ui(&current_tour);
}
```

---

#### Utility Functions

##### `poi_type_to_string` / `poi_tier_to_string`
```c
const char* poi_type_to_string(POIType type);
const char* poi_tier_to_string(POITier tier);
```
**Description**: Convert enums to human-readable strings.

**Example**:
```c
printf("Type: %s, Tier: %s\n", 
       poi_type_to_string(POI_TYPE_HISTORICAL),
       poi_tier_to_string(POI_TIER_SPECIAL));
// Output: "Type: Historical, Tier: Special"
```

---

##### `poi_type_from_string` / `poi_tier_from_string`
```c
POIType poi_type_from_string(const char* str);
POITier poi_tier_from_string(const char* str);
```
**Description**: Parse type/tier from JSON strings (case-insensitive).

---

### Fog of War Module (`game_fog_of_war.h`)

Manages POI visibility and discovery as ships explore the archipelago.

#### Initialization

##### `fog_init`
```c
void fog_init(FogOfWarState* fog);
```
**Description**: Initialize fog of war system. Sets all POIs to hidden state.

---

##### `fog_set_prototype_mode`
```c
void fog_set_prototype_mode(FogOfWarState* fog, bool enabled);
```
**Description**: Enable prototype mode (all POIs visible, but tracking continues).

**Note**: Currently enabled by default for testing. Disable for production gameplay.

---

#### Visibility Queries

##### `fog_get_poi_visibility`
```c
VisibilityState fog_get_poi_visibility(const FogOfWarState* fog, int poi_index);
```
**Returns**: 
- `VISIBILITY_HIDDEN`: Never seen (0)
- `VISIBILITY_DISCOVERED`: Seen before, can navigate to (1)
- `VISIBILITY_VISIBLE`: Currently in sight (2)

---

##### `fog_get_poi_alpha`
```c
float fog_get_poi_alpha(const FogOfWarState* fog, int poi_index);
```
**Returns**: Fog opacity for rendering (0.0 = clear, 1.0 = fully fogged)

---

##### `fog_is_position_visible`
```c
bool fog_is_position_visible(const FogOfWarState* fog, const ECSWorld* ecs_world,
                             ComponentMask ship_mask, float x, float y);
```
**Description**: Check if a world position is visible from any ship.

**Parameters**:
- `fog`: Fog of war state
- `ecs_world`: Main ECS world with ship positions
- `ship_mask`: Component mask to identify ships
- `x`, `y`: World position to check

---

#### Visibility Modification

##### `fog_reveal_poi`
```c
void fog_reveal_poi(FogOfWarState* fog, int poi_index);
```
**Description**: Manually reveal a POI (for story events, tutorials).

---

##### `fog_discover_poi`
```c
void fog_discover_poi(FogOfWarState* fog, int poi_index);
```
**Description**: Manually discover a POI (makes it navigable but not fully visible).

---

##### `fog_reveal_all`
```c
void fog_reveal_all(FogOfWarState* fog, const POIEcsWorld* poi_world);
```
**Description**: Cheat/debug function to reveal all POIs.

---

#### System Update

##### `fog_system_update`
```c
void fog_system_update(FogOfWarState* fog, const POIEcsWorld* poi_world,
                       const ECSWorld* ecs_world, ComponentMask ship_mask,
                       float delta_time);
```
**Description**: Update fog visibility based on ship positions. Call every frame.

**Behavior**:
- POIs within `FOG_REVEAL_RADIUS` (150.0 units) become visible
- POIs within `FOG_DISCOVERY_RADIUS` (300.0 units) become discovered
- Fog alpha fades smoothly at `FOG_FADE_SPEED` (2.0 units/sec)

**Example**:
```c
// In game update loop
fog_system_update(&fog_state, &poi_world, &ecs_world, 
                  ship_component_mask, delta_time);
```

---

### Satisfaction Module (`game_satisfaction.h`)

Calculates passenger satisfaction based on POI visits during tours.

#### Tour Lifecycle

##### `satisfaction_tour_start`
```c
void satisfaction_tour_start(TourSatisfaction* tour);
```
**Description**: Begin a new tour (resets visit tracking and bonuses).

---

##### `satisfaction_tour_end`
```c
void satisfaction_tour_end(TourSatisfaction* tour);
```
**Description**: End the tour and calculate final satisfaction score and rating.

---

##### `satisfaction_tour_is_active`
```c
bool satisfaction_tour_is_active(const TourSatisfaction* tour);
```
**Returns**: `true` if a tour is currently in progress

---

#### Recording POI Visits

##### `satisfaction_record_poi_visit`
```c
int satisfaction_record_poi_visit(TourSatisfaction* tour, 
                                  const POIEcsWorld* poi_world, int poi_index);
```
**Description**: Record a POI visit during the active tour.

**Returns**: Satisfaction bonus gained from this visit

**Behavior**:
- Duplicate visits in the same tour do not grant additional satisfaction
- Updates type tracking for variety bonuses
- Calculates bonuses based on type multipliers

**Example**:
```c
if (poi_ecs_check_visit(&poi_world, poi_idx, ship_x, ship_y)) {
    int bonus = satisfaction_record_poi_visit(&tour, &poi_world, poi_idx);
    printf("Visited POI! +%d satisfaction\n", bonus);
}
```

---

##### `satisfaction_was_poi_visited`
```c
bool satisfaction_was_poi_visited(const TourSatisfaction* tour, int poi_index);
```
**Returns**: `true` if the POI was already visited during this tour

---

#### Score Calculation

##### `satisfaction_calculate_score`
```c
int satisfaction_calculate_score(const TourSatisfaction* tour);
```
**Description**: Calculate current satisfaction score (can be called mid-tour).

**Formula**:
```
Score = BASE_SATISFACTION (50) 
      + Sum of POI bonuses (with type/tier multipliers)
      + Variety bonus (0/5/15 for 1/2/3 types)
```

---

##### `satisfaction_get_rating`
```c
SatisfactionRating satisfaction_get_rating(int score);
```
**Returns**: Rating enum based on score thresholds

**Rating Thresholds**:
- `RATING_POOR`: 0-39
- `RATING_AVERAGE`: 40-59
- `RATING_GOOD`: 60-74
- `RATING_EXCELLENT`: 75-89
- `RATING_OUTSTANDING`: 90+

---

##### `satisfaction_rating_to_string`
```c
const char* satisfaction_rating_to_string(SatisfactionRating rating);
```
**Returns**: Human-readable rating ("Poor", "Average", "Good", "Excellent", "Outstanding")

---

#### Configuration

##### `satisfaction_get_default_config`
```c
SatisfactionConfig satisfaction_get_default_config(void);
```
**Returns**: Default satisfaction configuration

**Default Values**:
```c
SatisfactionConfig {
    .nature_multiplier = 1.0f,
    .historical_multiplier = 1.2f,
    .military_multiplier = 1.1f,
    .special_tier_multiplier = 1.5f,
    .variety_bonus_2_types = 5,
    .variety_bonus_3_types = 15,
    .base_satisfaction = 50
}
```

---

##### `satisfaction_calculate_poi_bonus`
```c
int satisfaction_calculate_poi_bonus(const POIEcsWorld* poi_world, int poi_index,
                                     const SatisfactionConfig* config);
```
**Description**: Calculate satisfaction bonus for a specific POI with custom config.

**Returns**: Calculated bonus considering type and tier multipliers

---

### POI Loader Module (`game_poi_loader.h`)

Handles loading POI data from JSON files.

#### Loading Functions

##### `poi_load_from_file`
```c
POILoadResult poi_load_from_file(POIEcsWorld* poi_world, const char* filepath);
```
**Description**: Load POIs from a JSON file.

**Returns**: 
- `POI_LOAD_SUCCESS` (0) - Loaded successfully
- `POI_LOAD_FILE_NOT_FOUND` - File does not exist
- `POI_LOAD_PARSE_ERROR` - JSON syntax error
- `POI_LOAD_INVALID_SCHEMA` - Schema validation failed
- `POI_LOAD_WORLD_NOT_INITIALIZED` - POI world not initialized
- `POI_LOAD_OUT_OF_MEMORY` - Too many POIs (> MAX_POIS)

**Example**:
```c
poi_ecs_init(&poi_world);
POILoadResult result = poi_load_from_file(&poi_world, "assets/data/pois.json");
if (result != POI_LOAD_SUCCESS) {
    fprintf(stderr, "POI load failed: %s\n", 
            poi_load_result_to_string(result));
    poi_load_defaults(&poi_world);  // Load fallback POIs
}
```

---

##### `poi_load_from_string`
```c
POILoadResult poi_load_from_string(POIEcsWorld* poi_world, const char* json_string);
```
**Description**: Load POIs from a JSON string (useful for testing or embedded data).

---

##### `poi_validate_file`
```c
POILoadResult poi_validate_file(const char* filepath);
```
**Description**: Validate JSON schema without loading (for modding tools).

---

##### `poi_load_defaults`
```c
void poi_load_defaults(POIEcsWorld* poi_world);
```
**Description**: Load built-in fallback POIs (the 10 Gothenburg archipelago POIs).

---

##### `poi_get_default_filepath`
```c
const char* poi_get_default_filepath(void);
```
**Returns**: Default POI data file path (`"assets/data/pois.json"`)

---

## Controls

### Camera Zoom Controls

The POI system includes camera zoom functionality for inspecting POI locations:

| Control | Action |
|---------|--------|
| **Mouse Scroll Up** | Zoom in (closer view) |
| **Mouse Scroll Down** | Zoom out (wider view) |
| **+ Key** | Zoom in |
| **- Key** | Zoom out |
| **0 Key** | Reset zoom to default level |

**Zoom Range**: 0.5× to 2.0× (configurable)

**Usage**: Zoom in to inspect POI details and plan precise routes. Zoom out for strategic overview of the archipelago.

---

## Sample POIs

The game includes 10 placeholder POIs representing the Gothenburg Archipelago (1887-1950s setting):

### Special Tier POIs (High Value)

1. **Älvsborg Fortress** (Military, Special)
   - Position: (150, 150)
   - Satisfaction: +22
   - *Famous fortress guarding the entrance to Gothenburg*

2. **Vinga Lighthouse** (Historical, Special)
   - Position: (800, 200)
   - Satisfaction: +20
   - *Historic lighthouse marking the entrance to Gothenburg harbor*

3. **Vrångö Nature Reserve** (Nature, Special)
   - Position: (600, 500)
   - Satisfaction: +18
   - *Pristine nature reserve with rare bird species*

4. **Känsö Fortress** (Military, Special)
   - Position: (250, 280)
   - Satisfaction: +15
   - *Historic quarantine fortress from the 18th century*

### General Tier POIs (Common)

5. **Styrsö Village** (Historical, General)
   - Position: (400, 350)
   - Satisfaction: +8
   - *Traditional fishing village with charming wooden houses*

6. **Galterö Gun Battery** (Military, General)
   - Position: (720, 120)
   - Satisfaction: +8
   - *Coastal defense position from the 19th century*

7. **Köpstadsö Fishing Harbor** (Historical, General)
   - Position: (550, 420)
   - Satisfaction: +7
   - *Authentic working fishing harbor*

8. **Donsö Cliffs** (Nature, General)
   - Position: (350, 480)
   - Satisfaction: +6
   - *Scenic granite cliffs with panoramic views*

9. **Rivö Hidden Cove** (Nature, General)
   - Position: (700, 380)
   - Satisfaction: +6
   - *Secluded cove perfect for swimming*

10. **Brännö Beach** (Nature, General)
    - Position: (480, 300)
    - Satisfaction: +5
    - *Popular sandy beach with calm waters*

### Type Distribution
- **Nature**: 4 POIs (40%)
- **Historical**: 3 POIs (30%)
- **Military**: 3 POIs (30%)

### Tier Distribution
- **General**: 6 POIs (60%)
- **Special**: 4 POIs (40%)

---

## Modding Guide

### Adding Custom POIs

1. **Open** `assets/data/pois.json`
2. **Add** a new POI object to the `"pois"` array
3. **Validate** JSON syntax (use a JSON validator)
4. **Test** in-game to verify position and behavior

### Best Practices

- **Unique IDs**: Use descriptive, unique identifiers (e.g., `poi_island_beach_north`)
- **Balanced Satisfaction**: General tier 5-10, Special tier 15-25
- **Appropriate Radii**: Small POIs 30-40, Medium 40-60, Large 60-100
- **Position Planning**: Ensure POIs don't overlap (min 100 units apart recommended)
- **Type Balance**: Mix all three categories for variety bonuses
- **Tier Balance**: 60-70% General, 30-40% Special maintains good progression
- **Historical Accuracy**: For period settings, research actual landmarks

### Testing Your POIs

1. **Load Game**: Custom POIs load automatically on startup
2. **Check Logs**: Watch console for parse errors or warnings
3. **Verify Position**: Use debug tools to display POI markers
4. **Test Visits**: Sail within radius to verify visit detection
5. **Check Satisfaction**: Complete a tour and review satisfaction breakdown

### Common Issues

| Issue | Cause | Solution |
|-------|-------|----------|
| POI not appearing | Invalid position coordinates | Check world bounds (typical 0-1000) |
| Parse error on load | JSON syntax error | Validate JSON with online tool |
| No satisfaction bonus | Satisfaction_bonus set to 0 | Omit field to use tier default |
| POI not discovered | Fog of war enabled | Ensure prototype mode is on for testing |
| Wrong type/tier | String mismatch | Use exact values: `nature`, `historical`, `military`, `general`, `special` |

---

## Integration Example

Complete example of integrating the POI system into a game loop:

```c
#include "game_poi_ecs.h"
#include "game_poi_loader.h"
#include "game_fog_of_war.h"
#include "game_satisfaction.h"

// Global game state
POIEcsWorld poi_world;
FogOfWarState fog_state;
TourSatisfaction current_tour;

// Initialization
void game_init(void) {
    // Initialize POI system
    poi_ecs_init(&poi_world);
    
    // Load POIs from file
    POILoadResult result = poi_load_from_file(&poi_world, "assets/data/pois.json");
    if (result != POI_LOAD_SUCCESS) {
        printf("Failed to load POIs: %s\n", poi_load_result_to_string(result));
        poi_load_defaults(&poi_world);  // Use fallback data
    }
    
    // Initialize fog of war
    fog_init(&fog_state);
    fog_set_prototype_mode(&fog_state, true);  // All POIs visible for testing
    
    // Start first tour
    satisfaction_tour_start(&current_tour);
    
    printf("Loaded %u POIs\n", poi_ecs_get_count(&poi_world));
}

// POI visit callback
void on_poi_visited(int poi_index, Entity ship_entity, void* user_data) {
    const char* poi_name = poi_ecs_get_name(&poi_world, poi_index);
    
    // Mark as visited
    if (!poi_ecs_is_visited(&poi_world, poi_index)) {
        poi_ecs_set_visited(&poi_world, poi_index, true);
        printf("First-time visit: %s\n", poi_name);
    }
    
    // Increment statistics
    poi_ecs_increment_visits(&poi_world, poi_index);
    
    // Record for tour satisfaction
    int bonus = satisfaction_record_poi_visit(&current_tour, &poi_world, poi_index);
    printf("POI Visit: %s (+%d satisfaction)\n", poi_name, bonus);
}

// Game update loop
void game_update(ECSWorld* ecs_world, float delta_time) {
    ComponentMask ship_mask = COMPONENT_TRANSFORM | COMPONENT_SHIP;
    
    // Update fog of war visibility
    fog_system_update(&fog_state, &poi_world, ecs_world, ship_mask, delta_time);
    
    // Check for POI visits
    POISystemContext poi_context = {
        .on_visit = on_poi_visited,
        .user_data = NULL
    };
    poi_ecs_system_update(&poi_world, ecs_world, ship_mask, &poi_context);
    
    // Display current tour satisfaction (mid-tour)
    int current_score = satisfaction_calculate_score(&current_tour);
    SatisfactionRating rating = satisfaction_get_rating(current_score);
    printf("Tour Satisfaction: %d (%s)\n", current_score, 
           satisfaction_rating_to_string(rating));
}

// End of tour
void tour_complete(void) {
    satisfaction_tour_end(&current_tour);
    
    TourStats stats = satisfaction_get_tour_stats(&current_tour);
    printf("Tour Complete!\n");
    printf("  POIs Visited: %u\n", stats.pois_visited);
    printf("  Final Score: %d\n", stats.total_satisfaction);
    printf("  Rating: %s\n", satisfaction_rating_to_string(stats.rating));
    printf("  POI Bonus: %d\n", stats.poi_bonus);
    printf("  Variety Bonus: %d\n", stats.variety_bonus);
    
    // Start new tour
    satisfaction_tour_start(&current_tour);
}

// Shutdown
void game_shutdown(void) {
    poi_ecs_shutdown(&poi_world);
    fog_shutdown(&fog_state);
}
```

---

## Performance Considerations

### Data-Oriented Design

The POI system follows MS Tour's data-oriented architecture:

- **Struct-of-Arrays (SoA)**: All POI data stored in parallel arrays for cache efficiency
- **Batch Processing**: POI queries process multiple POIs sequentially
- **Minimal Indirection**: Direct array access, no pointer chasing
- **Predictable Memory**: Fixed MAX_POIS (256) with known memory footprint

### Performance Metrics

- **Max POIs**: 256 (configurable via `MAX_POIS`)
- **Memory Usage**: ~150 KB per 256 POIs
- **Visit Check**: O(n) where n = POI count (typically < 100)
- **Spatial Queries**: Basic linear search (future: spatial partitioning)
- **Load Time**: < 50ms for typical JSON files (< 1MB)

### Optimization Tips

- **Limit Active POIs**: Keep total count under 100 for best performance
- **Reduce Radius Checks**: Only check visits for POIs within discovery radius
- **Batch Queries**: Use `poi_ecs_find_in_range` instead of repeated `find_at_position`
- **Cache Results**: Store nearby POI indices per ship to avoid repeated searches
- **Static Data**: POIs never move, so positions can be cached

---

## Future Enhancements

Planned features for future releases:

1. **Spatial Partitioning**: Quadtree/grid for O(1) position queries
2. **Dynamic POIs**: Time-based availability (seasonal attractions, events)
3. **POI State**: Open/closed based on time of day or story progression
4. **Passenger Preferences**: Different passenger types prefer different POI categories
5. **Discovery Rewards**: XP, prestige, or unlocks for first-time discoveries
6. **POI Challenges**: Special objectives related to specific POIs
7. **Weather Effects**: Reduced satisfaction for nature POIs in bad weather
8. **Historical Events**: Story-triggered POI changes reflecting archipelago history
9. **Localization**: Multi-language support for POI names/descriptions
10. **Mod Tools**: GUI editor for creating and testing custom POI layouts

---

## Troubleshooting

### POIs Not Loading

**Symptom**: Game starts with no POIs or default fallback POIs

**Causes**:
- File not found at `assets/data/pois.json`
- JSON parse error (syntax mistake)
- Invalid schema (missing required fields)

**Solutions**:
1. Check file path relative to executable
2. Validate JSON with online tool (jsonlint.com)
3. Review console logs for detailed error messages
4. Test with `poi_validate_file()` before runtime

### POI Visits Not Detected

**Symptom**: Ship passes through POI but no visit event fires

**Causes**:
- POI radius too small
- Ship not matching component mask
- POI system update not called
- Visit callback not registered

**Solutions**:
1. Increase POI `radius` (try 60.0 or higher)
2. Verify ship has required components (Transform + Ship)
3. Ensure `poi_ecs_system_update()` called every frame
4. Check `POISystemContext.on_visit` is set

### Satisfaction Not Increasing

**Symptom**: POI visited but no satisfaction bonus applied

**Causes**:
- Tour not started with `satisfaction_tour_start()`
- Duplicate visit in same tour
- Custom satisfaction_bonus set to 0

**Solutions**:
1. Call `satisfaction_tour_start()` before first visit
2. Check `satisfaction_was_poi_visited()` returns false
3. Omit `satisfaction_bonus` field to use tier default

### Fog of War Issues

**Symptom**: POIs not becoming visible/discovered

**Causes**:
- Fog of war not initialized
- Prototype mode disabled in testing
- Ships too far from POIs

**Solutions**:
1. Call `fog_init()` at startup
2. Enable prototype mode: `fog_set_prototype_mode(&fog, true)`
3. Check ship positions with debug rendering

---

## Additional Resources

- **Engine ECS Documentation**: [ENGINE_ECS.md](ENGINE_ECS.md)
- **Game Architecture**: [ARCHITECTURE.md](ARCHITECTURE.md)
- **Code Style Guide**: [STYLE_GUIDE.md](STYLE_GUIDE.md)
- **Build Instructions**: [BUILD.md](BUILD.md)

For questions or contributions, see [CONTRIBUTING.md](../CONTRIBUTING.md).

---

*Last Updated: 2025*
