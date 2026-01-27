# Ship Direct Control System

**Document Type:** System Design  
**Last Updated:** 2026-01-27  
**Status:** Ready for Confluence Migration

* * *

## Overview

The Ship Direct Control System allows players to manually pilot ships through
the archipelago using keyboard, mouse, or gamepad input. This is the core
moment-to-moment gameplay of MS Tour, where player skill directly impacts
success. The system uses arcade-style physics to create accessible yet skill-
expressive ship handling.

## Design Intent

**Primary Goals** : 1\. **Feel Good** : Ship control should be immediately
satisfying 2\. **Skill Expression** : Clear difference between novice and
expert piloting 3\. **Learnable** : Simple to understand, challenging to
master 4\. **Responsive** : Player inputs feel impactful, not floaty 5\.
**Contextual** : Handling changes with ship type, weather, cargo

**Design Philosophy** (Raph Koster): \- Control scheme is a "pattern language"
players learn through repetition \- Mastery curve should be visible: players
see themselves improving \- Feedback must be immediate and clear

* * *

## Control Actions & Input Bindings

The control system is designed around **player actions** rather than hardcoded
keys, enabling input remapping, accessibility options, and multi-platform
support.

### Navigation Actions

Action | Description | Default Keyboard | Default Gamepad | Hold Behavior  
---|---|---|---|---  
**Increase Throttle** | Accelerate forward | W / Up Arrow | Right Trigger | Accelerates to max speed  
**Decrease Throttle** | Slow down or reverse | S / Down Arrow | Left Trigger | Slows down, then reverses  
**Steer Left** | Turn ship counterclockwise | A / Left Arrow | Left Stick (Left) | Continuous turning  
**Steer Right** | Turn ship clockwise | D / Right Arrow | Left Stick (Right) | Continuous turning  
**Emergency Stop** | Full reverse thrust | Space | B Button | Full brake, 2-3 sec stop  
**Fine Control Modifier** | Reduce input to 50% | Shift | ΓÇö | Precise maneuvering  
  
### Interaction Actions

Action | Description | Default Keyboard | Default Gamepad | Context  
---|---|---|---|---  
**Context Interact** | Dock, rescue, pickup | E | A Button | Shows when in range of POI  
**Toggle Autopilot** | Engage/disengage AI control | Tab | Y Button | Enable/disable AI captain  
  
### Mouse Input (Alternative Control Method)

Action | Description | Mouse Input | Notes  
---|---|---|---  
**Set Waypoint** | Ship steers toward point | Left Click (Hold) | Point-and-click navigation  
**Quick Turn** | Emergency avoidance | Right Click | Context-sensitive rapid turn  
**Adjust Throttle** | Fine-tune speed | Scroll Wheel | Incremental speed control  
  
### Camera Actions (Optional)

Action | Description | Default Keyboard | Default Gamepad  
---|---|---|---  
**Rotate Camera Left** | Rotate view counterclockwise | Q | Right Stick (Left)  
**Rotate Camera Right** | Rotate view clockwise | E | Right Stick (Right)  
**Zoom In/Out** | Adjust camera distance | +/- | D-Pad Up/Down  
**Center Camera** | Reset view on ship | C | Right Stick (Click)  
  
**Input Design Notes** : \- All bindings are **default mappings** and should
be remappable \- Actions are implementation-agnostic (can be bound to any
input) \- Support for accessibility remapping (one-handed play, custom
layouts) \- Keyboard layouts (QWERTY/AZERTY/QWERTZ) handled via remapping \-
Gamepad support is "nice-to-have" but should be considered in action design

* * *

## Ship Movement Model

### Core Parameters (Per Ship)

Each ship has these base stats that affect handling:

Parameter | Description | Typical Range | Impact  
---|---|---|---  
**Max Speed** | Top velocity (knots) | 5-15 knots | How fast ship can go  
**Acceleration** | Time to max speed | 3-10 seconds | Responsiveness  
**Turn Rate** | Degrees per second at speed | 10-30 deg/s | Maneuverability  
**Inertia** | Resistance to velocity changes | 0.5-2.0 | How "heavy" it feels  
**Drift** | Sideways slide in turns | 0.1-0.5 | Realism vs arcade feel  
**Reverse Speed** | Max backward velocity | 2-5 knots | Reversing power  
  
### Ship Type Comparison

Ship Type | Speed | Acceleration | Turn Rate | Feel  
---|---|---|---|---  
**Small Sailing** | 8 knots | 5 sec | 25 deg/s | Quick, responsive, light  
**Medium Sailing** | 7 knots | 7 sec | 18 deg/s | Balanced, predictable  
**Large Sailing** | 6 knots | 10 sec | 12 deg/s | Slow, deliberate, heavy  
**Small Steam** | 10 knots | 3 sec | 20 deg/s | Fast, agile, modern  
**Medium Steam** | 12 knots | 4 sec | 15 deg/s | Powerful, reliable  
**Large Steam** | 14 knots | 6 sec | 10 deg/s | Fast but ponderous  
**Hybrid** | 11 knots | 4 sec | 17 deg/s | Versatile, balanced  
  
_Note: These are base values. Weather, cargo, and damage modify these stats._

* * *

## Movement Physics (Simplified)

### Arcade-Style Approach

MS Tour uses **arcade physics** , not simulation physics. The goal is fun and
accessible control, not nautical realism.

**Key Simplifications** : \- Ships turn at constant rate (not dependent on
speed) \- No complex hydrodynamics (drag, buoyancy are simplified) \- Momentum
is present but exaggerated for feel \- Wind and waves affect movement but
don't dominate

### Movement Calculation (Pseudo-Code)

    
    
    // Each frame update
    Update_Ship_Movement(ship, deltaTime, inputs):
    
        // 1. Handle throttle input
        targetSpeed = inputs.throttle * ship.maxSpeed
        currentSpeed = Lerp(currentSpeed, targetSpeed, ship.acceleration * deltaTime)
    
        // 2. Handle steering input
        if inputs.steering != 0:
            ship.rotation += inputs.steering * ship.turnRate * deltaTime
    
        // 3. Calculate velocity vector
        forwardDirection = Vector2(cos(ship.rotation), sin(ship.rotation))
        velocity = forwardDirection * currentSpeed
    
        // 4. Apply inertia (momentum from previous frame)
        velocity = Lerp(velocity, previousVelocity, ship.inertia)
    
        // 5. Apply environmental effects
        velocity += CalculateWindEffect(ship, wind)
        velocity += CalculateWaveEffect(ship, waves)
    
        // 6. Apply drift (for arcade feel)
        lateralDrift = CrossProduct(velocity, turnInput) * ship.driftFactor
        velocity += lateralDrift
    
        // 7. Update position
        ship.position += velocity * deltaTime
    
        // 8. Check collisions and constraints
        HandleGrounding(ship)
        HandleCollisions(ship)
    
        // 9. Store for next frame
        previousVelocity = velocity
    

### Feel Tuning Parameters

These parameters are tweakable to adjust how ships "feel":

Parameter | Effect | Recommended Range  
---|---|---  
**Inertia Factor** | How much ships coast | 0.7-0.9 (higher = more coast)  
**Drift Factor** | Sideways slide in turns | 0.1-0.3 (higher = more drift)  
**Acceleration Curve** | Linear vs exponential speed-up | 1.0-2.0 (higher = faster initial response)  
**Turn Smoothing** | Input smoothing for turns | 0.1-0.5 (higher = slower response)  
  
> **Design Note** : These should be tuned through playtesting. Goal is "feels
> like a ship" but "responds like a game character."

* * *

## Environmental Effects on Control

### Wind

Wind affects ship velocity, creating a constant force in the wind direction.

**Formula** :

    
    
    windForce = windSpeed * windDirection * ship.windSusceptibility
    ship.velocity += windForce * deltaTime
    

**Ship Susceptibility** : \- **Sailing ships** : High susceptibility (1.0) -
wind is propulsion \- **Steam ships** : Medium susceptibility (0.3) - wind
still matters \- **Hybrid** : Variable (0.5-0.8) - depends on sail/engine mode

**Player Experience** : \- Ships drift noticeably with strong winds \- Sailing
ships can use wind to their advantage (faster downwind) \- Steering into wind
requires compensation

### Waves

Waves create oscillating forces that affect stability and comfort.

**Effects** : \- **Rocking** : Visual ship rotation (aesthetic, not gameplay)
\- **Speed Reduction** : Large waves slow ships by 10-20% \- **Steering
Difficulty** : Random micro-adjustments to heading \- **Comfort Impact** :
High waves reduce passenger satisfaction

**Formula** :

    
    
    waveEffect = waveHeight * sin(time * waveFrequency)
    ship.speedMultiplier = 1.0 - (waveHeight * 0.2)
    ship.headingNoise = RandomRange(-waveHeight, +waveHeight) * 2 degrees
    

### Fog

Fog reduces visibility but doesn't affect ship handling directly.

**Effects** : \- **Visibility Radius** : How far player can see (100m to 500m)
\- **Depth Gauge Reliability** : Fog reduces chart accuracy slightly \-
**Collision Warning** : Less time to react to hazards

_Details in Weather System (future migration)_

### Current (Simplified)

Some areas have ocean currents (simplified, not realistic).

**Implementation** :

    
    
    current = areaCurrentVector * currentStrength
    ship.velocity += current * deltaTime
    

**Player Experience** : \- Ships move faster in current direction \- Docking
against current is harder \- Narrow passages with current are challenges

* * *

## Camera System

### Camera View

**Orthographic Top-Down** (Angled): \- Camera is 30-45 degrees from horizontal
\- Follows ship smoothly with slight lag \- Shows ~200m radius around ship \-
Rotates slightly to keep ship oriented "forward"

### Camera Behavior

Situation | Camera Action  
---|---  
**Normal Navigation** | Smooth follow with 0.5s lag  
**High Speed** | Zoom out slightly for more view  
**Docking** | Zoom in for precision  
**POI Viewing** | Pan to show POI and ship together  
**Grounding** | Shake slightly, zoom to show problem  
**Capsizing** | Dramatic zoom + rotation to emphasize danger  
  
_Note: Camera controls are optional. Default behavior should be sufficient for
most players._

* * *

## Docking Mechanics

### Manual Docking

When approaching a dock or POI mooring:

**Alignment Requirements** : 1\. **Distance** : Within 10m of dock point 2\.
**Angle** : Within 15 degrees of dock orientation 3\. **Speed** : Below 2
knots 4\. **Approach** : Moving toward dock (not away)

**Visual Feedback** : \- **Dock Zone** : Highlighted area (green when aligned,
red when not) \- **Alignment Guides** : Dotted lines showing ideal approach
angle \- **Speed Indicator** : Changes color (green < 2 knots, yellow 2-4, red
> 4)

**Docking Process** : 1\. Player approaches dock zone 2\. UI shows "Press
[Context Interact] to dock" when conditions met 3\. Player triggers Context
Interact action 4\. Ship auto-completes final approach (1-2 seconds) 5\.
Docked state: ship stops, can't move, options appear

**Skill Expression** : \- **Fast Dock** : Conditions met in < 5 seconds =
"Smooth Docking" XP bonus \- **Slow Dock** : Conditions met in > 20 seconds =
No bonus \- **Bad Dock** : Hit dock too fast (> 4 knots) = Damage, reputation
penalty

### Auto-Dock (Unlock)

Unlocked through piloting skill progression.

**Behavior** : \- Player triggers "Auto-Dock" action when near dock \- Ship AI
takes over, completes docking automatically \- Always safe (no damage risk) \-
Slower than skilled manual docking (10-15 seconds)

**When to Use** : \- Player is managing multiple ships \- Player wants to
focus on strategy, not piloting \- Time-saving when docking is routine

* * *

## Special Maneuvers

### Emergency Stop

**Action** : Emergency Stop  
**Effect** : Full reverse thrust immediately  
**Use Case** : Avoid collision, respond to sudden hazard

**Mechanics** : \- Speed reduced to 0 in 2-3 seconds (faster than normal
deceleration) \- Sharp braking animation/effect \- Passenger comfort penalty
(if passengers aboard) \- Cooldown: 5 seconds (can't spam)

### Rescue Positioning

When near a ship in distress:

**Goal** : Position alongside distressed ship for crew/passenger transfer

**Requirements** : 1\. Within 15m of target ship 2\. Parallel alignment
(within 20 degrees) 3\. Speed < 1 knot (near stop)

**Process** : \- Similar to docking, but target is another ship \- More
difficult in waves/wind (ships are moving) \- Successful positioning enables
rescue action

_Details in Rescue Mechanics (future migration)_

* * *

## Skill Progression

### Player Piloting Skill

Players naturally improve through practice. The game tracks and recognizes
this:

**Tracked Metrics** : \- **Groundings** : Frequency decreases as skill
improves \- **Docking Speed** : Time to complete docks reduces \- **Course
Efficiency** : Path directness increases \- **Collision Avoidance** : Near-
misses decrease

**XP Rewards** : \- **Clean Voyage** : No groundings = +50 XP \- **Fast
Docking** : < 5 seconds = +25 XP \- **Efficient Route** : Straight path to
waypoint = +10 XP \- **Weather Mastery** : Navigate storm without issues =
+100 XP

**Skill Unlocks** (via XP): \- **Level 1 (0 XP)** : Manual control only \-
**Level 2 (500 XP)** : Auto-dock unlocked \- **Level 3 (1500 XP)** : Weather
handling tips \- **Level 4 (3000 XP)** : Advanced route suggestions \- **Level
5 (5000 XP)** : Master captain (prestige bonus)

_Full progression in Skill Trees (future migration)_

* * *

## AI Captain Control (Delegation)

When player delegates a ship to an AI captain:

**AI Behavior** : \- Follows assigned route with waypoints \- Avoids grounding
(uses chart quality) \- Adjusts speed for weather \- Docks automatically at
POIs \- Responds to dynamic events (basic logic)

**AI Skill Level** : \- **Novice** (Level 1): Slow, cautious, occasional
mistakes \- **Competent** (Level 2-3): Reliable, safe, decent speed \-
**Expert** (Level 4-5): Optimal pathing, fast docking, excellent

**Player Visibility** : \- AI-controlled ships visible on map \- Can observe
AI piloting if desired \- Receive summary after voyage completion

_Details in Delegation Mechanics (future migration)_

* * *

## Feedback Systems

### Visual Feedback

Element | What It Shows  
---|---  
**Wake Trail** | Path ship has taken (3-5 seconds of trail)  
**Speed Lines** | Intensity increases with speed (aesthetic)  
**Depth Overlay** | Color-coded water (green/yellow/red) under ship  
**Turning Indicator** | Visual arc showing turn radius  
**Momentum Vector** | Arrow showing direction of movement vs facing  
**Wind Indicators** | Arrows showing wind direction and strength  
**Wave Animations** | Visual waves moving across water  
  
### Audio Feedback

Sound | Trigger | Purpose  
---|---|---  
**Engine Hum** | Continuous, pitch varies with throttle | Sense of power  
**Water Splash** | Bow wave, scales with speed | Sense of movement  
**Creaking** | Turning, especially in wind | Sense of strain  
**Warning Beep** | Approaching shallow water | Alert  
**Success Chime** | Clean docking, POI arrival | Positive reinforcement  
**Crash Sound** | Grounding, collision | Negative feedback  
  
### Haptic Feedback (Gamepad)

Event | Haptic Pattern  
---|---  
**Grounding** | Sharp, strong pulse  
**Wave Impact** | Rhythmic, medium pulses  
**Docking Success** | Short, satisfying pulse  
**Collision** | Long, harsh rumble  
  
* * *

## Edge Cases

### Stuck Ship

**Problem** : Ship grounded and can't move  
**Solution** : \- Allow full reverse even when grounded (slowly) \- "Call for
Help" option (costs money, teleports ship to harbor) \- Time penalty
(reputation hit)

### Multi-Ship Collision

**Problem** : Two player/AI ships collide  
**Solution** : \- Ships have collision volumes, push apart slightly \- Damage
both ships (minor) \- AI ships avoid other ships in pathfinding

### Input Conflicts

**Problem** : Player presses opposing actions (Increase+Decrease Throttle,
Steer Left+Right)  
**Solution** : \- Prioritize most recent input \- Or cancel out (neutral, ship
maintains momentum)

### Camera Orientation Loss

**Problem** : Player loses sense of direction during complex maneuvers  
**Solution** : \- Minimap always shows ship orientation \- Compass rose in UI
(North always up) \- Center Camera action resets view

* * *

## Performance Considerations

### Optimization Goals

  * **60 FPS** with player ship + 49 AI ships
  * Ship movement calculations must be efficient (batch processing)
  * Physics can be simplified for distant AI ships

### Implementation Notes

**Data-Oriented Design** :

    
    
    // Ship movement data (Struct-of-Arrays)
    struct ShipMovementData {
        float[] positions_x;
        float[] positions_y;
        float[] rotations;
        float[] velocities_x;
        float[] velocities_y;
        float[] throttles;
        float[] turnInputs;
        // ... other arrays
    }
    

**Update Strategy** : 1\. Process all AI ships in batch (parallel if possible)
2\. Process player ship separately (more detailed) 3\. Apply environmental
effects in batch 4\. Collision detection uses spatial partitioning

_Details in Technical Architecture (future migration)_

* * *

## Tuning Values (Reference)

Parameter | Default | Min | Max | Notes  
---|---|---|---|---  
**Turn Smoothing** | 0.2 | 0.0 | 0.5 | Lower = snappier  
**Inertia Factor** | 0.8 | 0.5 | 0.95 | Higher = more coast  
**Drift Factor** | 0.15 | 0.0 | 0.5 | Arcade feel  
**Camera Lag** | 0.5s | 0.2s | 1.0s | Smoothness  
**Docking Threshold Distance** | 10m | 5m | 20m | Docking zone size  
**Docking Threshold Angle** | 15┬░ | 10┬░ | 30┬░ | Alignment strictness  
**Emergency Stop Time** | 2.5s | 1.5s | 4.0s | Braking power  
  
These should be exposed in a config file for easy iteration.

* * *

## Success Metrics

### Feel Metrics (Playtesting)

  * **Responsive** : 80%+ players say controls feel good (survey)
  * **Learnable** : 90%+ can pilot without grounding within 15 minutes
  * **Satisfying** : 75%+ enjoy moment-to-moment piloting (survey)

### Objective Metrics (Telemetry)

  * **Grounding Rate** : < 2 per hour by Act 2 (learning curve)
  * **Docking Time** : Median < 10 seconds by mid Act 1
  * **Path Efficiency** : 80%+ direct paths by Act 2

### Engagement Metrics

  * **Manual vs Auto** : 60%+ of docks are manual in Act 1 (skill expression)
  * **Piloting Time** : 40%+ of session time is active piloting (not excessive)

* * *

## Design Notes

> **On Accessibility** : The control scheme must accommodate players with
> different skill levels. Auto-dock is essential for this, allowing less
> skilled players to progress. However, manual piloting should always be more
> rewarding (faster, XP bonuses). Action-based input design enables remapping
> for accessibility needs.

> **On Realism vs Fun** : We lean toward fun. Real ship handling is slow,
> ponderous, and wind-dependent to an extreme. Our ships should _suggest_
> ships without _simulating_ them. Think "Sid Meier's Pirates!" not "Ship
> Simulator 2025."

> **On Skill Expression** : The gap between novice and expert piloting should
> be obvious. Expert players should complete routes 20-30% faster, have zero
> groundings, and dock like butter. This makes skill progression satisfying.

> **On Delegation** : As players scale to 20+ ships, they can't manually pilot
> everything. AI captains must be competent enough that delegation feels safe,
> but not so perfect that manual piloting feels pointless. There should be
> situations where the player says "I need to take the helm myself for this
> one."

* * *

## Related Documents

This system connects with: \- **Ship Physics Model** \- Detailed physics
calculations (future migration) \- **Grounding System** \- Grounding detection
and recovery (future migration) \- **Docking System** \- Full docking
mechanics (future migration) \- **Weather System** \- Environmental effects
detail (future migration) \- **Rescue Mechanics** \- Rescue positioning and
actions (future migration) \- **Delegation Mechanics** \- AI captain behavior
(future migration) \- **Skill Trees** \- Full skill progression system (future
migration) \- **Technical Architecture** \- Implementation details (future
migration) \- **Core Gameplay Loop** \- How direct control fits into core loop
(future migration)

* * *

## Future Enhancements (Post-Launch)

  * **Gamepad support** : Full analog control (currently "nice-to-have")
  * **Advanced maneuvers** : Drift turns, quick stops, advanced techniques
  * **Ship customization** : Visual customization affects handling feedback
  * **Captain personalities** : AI captains have distinct piloting styles
  * **Challenge modes** : Speed runs, precision docking challenges

* * *

**Document Status:** Ready for Confluence - Revised with action-based control
design


