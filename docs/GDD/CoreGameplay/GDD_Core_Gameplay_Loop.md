# Core Gameplay Loop - MS Tour

**Document Type:** System Design  
**Version:** 1.0  
**Last Updated:** 2026-01-25  
**Owner:** Lead Designer  
**Related Documents:** [Main GDD](../GDD_MASTER.md), [Player Fantasy](../Narrative/GDD_Player_Fantasy.md), [Design Pillars](../CoreGameplay/GDD_Design_Pillars.md)

---

## Overview

The Core Gameplay Loop is the fundamental cycle of actions players repeat throughout MS Tour. It operates at three temporal scales: **minute-to-minute** (tactical), **session-to-session** (strategic), and **act-to-act** (narrative). This document details all three levels and how they interconnect.

## Design Intent

The loop must:
1. **Be satisfying at all scales**: Fun moment-to-moment, rewarding session-to-session, meaningful act-to-act
2. **Teach through repetition**: Each cycle reinforces learning
3. **Build anticipation**: Strategic planning creates anticipation for tactical execution
4. **Show progress**: Each completion moves visible progression forward
5. **Offer variety**: Multiple approaches prevent monotony

---

## Minute-to-Minute Loop (Tactical)

### Context
This is the moment-to-moment gameplay during a voyage. The player is actively piloting a ship through a route.

### Duration
5-15 minutes per voyage, depending on route length and number of POIs.

### The Loop

```
┌─────────────────────────────────────────────────────┐
│ 1. NAVIGATE TO NEXT WAYPOINT                        │
│    ├─ Steer ship (WASD/Arrows)                      │
│    ├─ Manage throttle (speed vs comfort)            │
│    ├─ Monitor depth indicator (avoid grounding)     │
│    └─ Check distance/ETA to waypoint                │
└─────────────────────────────────────────────────────┘
              ↓
┌─────────────────────────────────────────────────────┐
│ 2. RESPOND TO CONDITIONS                            │
│    ├─ Adjust for wind (ship drift)                  │
│    ├─ Handle waves (stability, speed)               │
│    ├─ Navigate fog (visibility reduction)           │
│    └─ React to dynamic events (ship in distress)    │
└─────────────────────────────────────────────────────┘
              ↓
┌─────────────────────────────────────────────────────┐
│ 3. MONITOR PASSENGERS                               │
│    ├─ Check satisfaction meter                      │
│    ├─ Read passenger comments/reactions             │
│    ├─ Balance speed vs comfort                      │
│    └─ Assess POI approach (good angle for viewing)  │
└─────────────────────────────────────────────────────┘
              ↓
┌─────────────────────────────────────────────────────┐
│ 4. ARRIVE AT POI                                    │
│    ├─ Slow down for optimal viewing                 │
│    ├─ Position ship (best view angle)               │
│    ├─ Wait for passenger enjoyment                  │
│    └─ Observe satisfaction feedback                 │
└─────────────────────────────────────────────────────┘
              ↓
┌─────────────────────────────────────────────────────┐
│ 5. CONTINUE OR COMPLETE ROUTE                       │
│    ├─ If more POIs: Return to step 1                │
│    └─ If route complete: Dock at harbor             │
└─────────────────────────────────────────────────────┘
              ↓
┌─────────────────────────────────────────────────────┐
│ 6. DOCK AND RECEIVE RESULTS                         │
│    ├─ Dock at harbor (manual or auto)               │
│    ├─ View satisfaction scores                      │
│    ├─ Receive revenue                               │
│    └─ See reputation changes                        │
└─────────────────────────────────────────────────────┘
```

### Skill Expression Points

| Action | Skill Required | Feedback | Mastery Indicator |
|--------|---------------|----------|-------------------|
| **Steering** | Anticipate momentum, smooth arcs | Ship trail, visual position | Smooth, efficient paths |
| **Depth Awareness** | Read depth gauge, memorize hazards | Color-coded depth, grounding warnings | No groundings on known routes |
| **Speed Management** | Balance ETA vs comfort | Passenger comfort meter, comments | Optimal speed for conditions |
| **Weather Adaptation** | Adjust steering/speed for wind/waves | Visual effects, handling changes | Smooth handling in any weather |
| **Docking** | Precision approach, speed control | Alignment guides, speed indicator | Fast, clean docks every time |

### Player Decisions (Moment-to-Moment)

1. **Route Path**: Follow direct path vs scenic but longer route
2. **Speed**: Fast (quick completion, lower comfort) vs Slow (high comfort, longer time)
3. **POI Approach**: Quick pass vs optimal viewing angle
4. **Risk Taking**: Cut through shallow waters vs safe deep channels
5. **Event Response**: Help distressed ship (time cost) vs ignore (reputation cost)

### Feedback Systems

**Visual Feedback**:
- Depth indicator (green → yellow → red as water shallows)
- Wake trail (shows path taken, turning efficiency)
- Passenger animations (happy/unhappy body language)
- Weather effects (wind direction arrows, wave intensity)
- POI highlight (when in good viewing position)

**Audio Feedback**:
- Engine sounds (pitch varies with throttle)
- Water sounds (intensity with speed and waves)
- Passenger chatter (happy murmurs vs complaints)
- Warning sounds (proximity to shallow water)
- Success chimes (good POI viewing, clean dock)

**UI Feedback**:
- Satisfaction meter (real-time updates)
- Depth gauge (numerical + color-coded)
- Speed indicator (knots)
- ETA to next waypoint
- Passenger comments (speech bubbles)

---

## Session-to-Session Loop (Strategic)

### Context
This is the planning and management layer between voyages. The player is in the harbor or planning room making strategic decisions.

### Duration
One session = 60-90 minutes, containing 3-5 voyages plus strategic time.

### The Loop

```
┌──────────────────────────────────────────────────────────┐
│ SESSION START: HARBOR SCENE                              │
│ └─ Player sees their ships, harbor activity, options     │
└──────────────────────────────────────────────────────────┘
              ↓
┌──────────────────────────────────────────────────────────┐
│ 1. REVIEW PREVIOUS RESULTS                               │
│    ├─ Check voyage profits/losses                        │
│    ├─ Read passenger feedback (satisfaction, comments)   │
│    ├─ Review reputation changes                          │
│    ├─ Check employee XP gains                            │
│    └─ Assess fleet status (repairs needed, fuel)         │
└──────────────────────────────────────────────────────────┘
              ↓
┌──────────────────────────────────────────────────────────┐
│ 2. STRATEGIC PLANNING (Planning Room)                    │
│    ├─ Analyze route profitability                        │
│    ├─ Design new routes or optimize existing ones        │
│    ├─ Purchase better sea charts (unlock new areas)      │
│    ├─ Identify underserved markets (passenger types)     │
│    └─ Plan seasonal strategy (upcoming weather)          │
└──────────────────────────────────────────────────────────┘
              ↓
┌──────────────────────────────────────────────────────────┐
│ 3. RESOURCE ALLOCATION                                   │
│    ├─ Purchase new ships or upgrades                     │
│    ├─ Hire new employees or train existing ones          │
│    ├─ Invest in ship amenities                           │
│    ├─ Buy fuel and provisions                            │
│    └─ Manage cash reserves vs investments                │
└──────────────────────────────────────────────────────────┘
              ↓
┌──────────────────────────────────────────────────────────┐
│ 4. FLEET ASSIGNMENT                                      │
│    ├─ Assign ships to routes                             │
│    ├─ Assign captains to ships                           │
│    ├─ Set tour parameters (pricing, passenger classes)   │
│    └─ Decide which ship(s) to pilot personally           │
└──────────────────────────────────────────────────────────┘
              ↓
┌──────────────────────────────────────────────────────────┐
│ 5. EXECUTE VOYAGES (3-5 per session)                    │
│    ├─ Pilot selected ship(s) through routes              │
│    ├─ Delegated ships execute automatically              │
│    ├─ Handle events and challenges                       │
│    └─ Return to harbor with results                      │
└──────────────────────────────────────────────────────────┘
              ↓
┌──────────────────────────────────────────────────────────┐
│ 6. STORY MOMENTS (Variable Timing)                      │
│    ├─ Family interactions (wife, children dialogues)     │
│    ├─ VIP unlock notifications                           │
│    ├─ Newspaper weekly edition                           │
│    ├─ Historical event triggers                          │
│    └─ Character development scenes                       │
└──────────────────────────────────────────────────────────┘
              ↓
┌──────────────────────────────────────────────────────────┐
│ 7. REFLECTION & PROGRESSION                             │
│    ├─ Check overall business health (net worth)          │
│    ├─ Review prestige level and unlocks                  │
│    ├─ See skill/XP progression                           │
│    ├─ Assess progress toward goals (Act objectives)      │
│    └─ Save game, plan next session                       │
└──────────────────────────────────────────────────────────┘
```

### Strategic Decision Points

| Decision Category | Options | Trade-offs |
|-------------------|---------|------------|
| **Route Strategy** | New routes vs optimize existing | Exploration vs efficiency |
| **Fleet Expansion** | More ships vs better ships | Quantity vs quality |
| **Market Targeting** | Volume (Knegare) vs Premium (Adel) | Revenue vs margin |
| **Employee Investment** | Hire more vs train existing | Scale vs expertise |
| **Chart Investment** | Explore new areas vs maximize known areas | Discovery vs optimization |
| **Amenity Focus** | Basic services vs luxury | Cost vs satisfaction potential |

### Progression Markers (Per Session)

**Economic**:
- Net worth increase: 10-20% per session (Act 1), 5-10% (Act 2), 0-5% (Act 3)
- New ship acquisition: Every 3-5 sessions in Act 1-2
- Amenity unlocks: 1-2 per session in Act 1, as needed in Act 2-3

**Content**:
- New islands discovered: 1-2 per session (while fog of war remains)
- New POIs visited: 3-5 per session
- Chart quality upgrades: Every 4-6 sessions

**Reputation**:
- Prestige tier increase: Every 8-12 sessions (slower in late game)
- VIP unlocks: Every 10-15 sessions (Act 2-3)

**Narrative**:
- Family moments: 1-2 per session
- Historical events: Variable (timeline-based)
- Story beats: Major beats every 5-8 sessions

### Session Goals (Player-Set)

Players should leave each session with clear goals for the next:
- "Next session I'll save up for a steam ship"
- "Next time I'll explore the northern islands"
- "I need to unlock the next prestige tier to get the Oscar II contract"
- "I'll focus on training my navigator to level 5"

---

## Act-to-Act Loop (Narrative)

### Context
This is the longest-scale progression, spanning the entire playthrough across three acts.

### Duration
Each act = 6-10 hours of gameplay.

### The Flow

```
┌──────────────────────────────────────────────────────────┐
│ DEATHBED OPENING (Framing Device)                        │
│ └─ "Till havs" begins, old protagonist reflects          │
│ └─ Flashback to 1887, young and ambitious                │
└──────────────────────────────────────────────────────────┘
              ↓
┌──────────────────────────────────────────────────────────┐
│ ACT 1: THE YOUNG CAPTAIN (1887-1900s)                   │
│ Duration: 5-8 hours                                      │
│ Theme: Foundation and Romance                            │
│                                                          │
│ Key Progression:                                         │
│ ├─ Learn ship piloting fundamentals                     │
│ ├─ Establish first profitable routes                     │
│ ├─ Court and marry wife                                  │
│ ├─ Welcome first child                                   │
│ ├─ Build local reputation                                │
│ └─ Found company officially                              │
│                                                          │
│ Gameplay Evolution:                                      │
│ ├─ Start: One ship, manual piloting, basic routes        │
│ ├─ Mid: 2-3 ships, starting to delegate simple tasks     │
│ └─ End: 3-5 ships, comfortable with piloting and basics  │
│                                                          │
│ Systems Unlocked:                                        │
│ ├─ Ship control, grounding, docking                      │
│ ├─ Basic tours, passenger satisfaction                   │
│ ├─ Simple economy (costs, revenue, profit)               │
│ ├─ Chart purchases, fog of war                           │
│ └─ Auto-dock (skill unlock near end)                     │
└──────────────────────────────────────────────────────────┘
              ↓
┌──────────────────────────────────────────────────────────┐
│ ACT TRANSITION 1 → 2                                     │
│ ├─ Successful company established                        │
│ ├─ First child is young (3-5 years old)                  │
│ ├─ Wife is established co-narrator                       │
│ ├─ Local area is fully known                             │
│ └─ Ready to expand to full archipelago                   │
└──────────────────────────────────────────────────────────┘
              ↓
┌──────────────────────────────────────────────────────────┐
│ ACT 2: THE SHIPPING MAGNATE (1900s-1920s)               │
│ Duration: 8-12 hours                                     │
│ Theme: Growth and Delegation                             │
│                                                          │
│ Key Progression:                                         │
│ ├─ Scale fleet to 10-20 ships                            │
│ ├─ Hire and train employees                              │
│ ├─ Welcome 2-3 more children                             │
│ ├─ Secure VIP contracts (Oscar II, Nobel)                │
│ ├─ Achieve archipelago-wide reputation                   │
│ └─ Reach peak business success                           │
│                                                          │
│ Gameplay Evolution:                                      │
│ ├─ Start: 5-10 ships, learning to delegate               │
│ ├─ Mid: 10-15 ships, strategic focus over tactical       │
│ └─ End: 15-20+ ships, empire management                  │
│                                                          │
│ Systems Unlocked:                                        │
│ ├─ Captain delegation, employee skills                   │
│ ├─ Advanced ship types (steam, hybrid)                   │
│ ├─ Ship amenities, customization                         │
│ ├─ Prestige system, VIP contracts                        │
│ ├─ Route optimization tools                              │
│ ├─ Weather system (full complexity)                      │
│ └─ Newspaper system                                      │
└──────────────────────────────────────────────────────────┘
              ↓
┌──────────────────────────────────────────────────────────┐
│ ACT TRANSITION 2 → 3                                     │
│ ├─ Empire is at its peak                                 │
│ ├─ Children are teenagers/young adults                   │
│ ├─ Historical events begin (approaching WWI)             │
│ ├─ Thoughts turn to legacy and succession                │
│ └─ Deathbed narration becomes more prominent             │
└──────────────────────────────────────────────────────────┘
              ↓
┌──────────────────────────────────────────────────────────┐
│ ACT 3: THE PATRIARCH/MATRIARCH (1920s-1950s)            │
│ Duration: 7-10 hours                                     │
│ Theme: Legacy and Succession                             │
│                                                          │
│ Key Progression:                                         │
│ ├─ Navigate WWI and WWII                                 │
│ ├─ Handle economic volatility                            │
│ ├─ Children make life choices                            │
│ ├─ Select and train heir                                 │
│ ├─ Make final legacy decisions                           │
│ └─ Experience deathbed reflection                        │
│                                                          │
│ Gameplay Evolution:                                      │
│ ├─ Start: Established empire, new challenges             │
│ ├─ Mid: Adapting to historical events                    │
│ └─ End: Training successor, wrapping up                  │
│                                                          │
│ Systems Unlocked:                                        │
│ ├─ Historical events (wars, depressions)                 │
│ ├─ Succession system (evaluate, train heir)              │
│ ├─ Economic volatility mechanics                         │
│ ├─ Late-game VIPs (Strindberg, Lagerlöf)                 │
│ ├─ Capsizing system (late challenge)                     │
│ ├─ Bankruptcy risk (economic events)                     │
│ └─ Reflection scenes (deathbed integration)              │
└──────────────────────────────────────────────────────────┘
              ↓
┌──────────────────────────────────────────────────────────┐
│ ENDING & EPILOGUE                                        │
│ ├─ Deathbed scene conclusion                             │
│ ├─ "Till havs" completes                                 │
│ ├─ Heir takes over (or company ends)                     │
│ ├─ Legacy summary (achievements, impact)                 │
│ ├─ Epilogue (heir's first years)                         │
│ └─ Credits (with emotional music)                        │
└──────────────────────────────────────────────────────────┘
```

### Act-Level Objectives

Each act has clear goals that drive the player forward:

**Act 1 Goals**:
- Establish 3-5 profitable routes
- Reach "Local Hero" prestige tier
- Marry your wife
- Purchase 3-5 ships
- Unlock 5-8 islands
- Net worth: 50,000 kr

**Act 2 Goals**:
- Build fleet of 15+ ships
- Reach "Renowned Captain" prestige tier
- Secure 2+ VIP contracts
- Raise 3+ children
- Unlock full archipelago (20+ islands)
- Net worth: 500,000 kr

**Act 3 Goals**:
- Maintain empire through historical events
- Select and train heir
- Achieve highest prestige tier
- Complete major VIP contracts
- Make peace with choices (narrative resolution)
- Leave legacy (ending variations)

---

## Loop Integration (How They Connect)

### Minute → Session
- Each voyage's **outcome** (satisfaction, profit, reputation) feeds into **session-level strategic decisions**
- Skill improvement in piloting (minute-level) enables **more challenging routes** (session-level)
- Passenger feedback (minute-level) informs **route optimization** (session-level)

### Session → Act
- Accumulated wealth (session) enables **major purchases** (ships, upgrades) that drive **act progression**
- Reputation gains (session) unlock **VIPs and historical integration** (act beats)
- Employee XP (session) enables **delegation** which shifts **act gameplay style**
- Family moments (session) build toward **succession choice** (act 3)

### Act → Minute
- New ship types (act unlocks) change **handling characteristics** (minute-level piloting)
- Historical events (act) create **new weather/economic conditions** (minute and session)
- Succession training (act 3) adds **new objectives to voyages** (minute-level)

---

## Flow State & Pacing

### Maintaining Flow

**Schell's Flow Channel**:
- **Challenge must match skill**: Act 1 is easier, Act 3 is harder
- **Clear goals**: Each loop level has obvious objectives
- **Immediate feedback**: All actions have visible consequences
- **Concentration**: Voyages require attention but aren't exhausting
- **Control**: Players feel agency in all decisions

### Pacing Techniques

**Tension and Release**:
- **Tension**: Voyages (active piloting, risk, passenger management)
- **Release**: Harbor (strategic planning, story moments, no time pressure)
- **Rhythm**: 3-5 voyages → strategic break → story moment → repeat

**Novelty and Familiarity**:
- **Familiar**: Core piloting remains consistent (comfort)
- **Novel**: New routes, POIs, passengers, events (interest)
- **Balance**: 70% familiar, 30% novel per session

**Challenge Curve**:
- **Act 1**: Gentle slope, focused on teaching
- **Act 2**: Steeper slope, new systems, scaling challenges
- **Act 3**: High baseline, historical events add spikes

---

## Loop Variations by Playstyle

### Volume Player (Mass Market Strategy)
- **Minute**: Fast routes, maximize throughput
- **Session**: More ships, simple routes, Knegare class focus
- **Act**: Scale through quantity, efficiency optimization

### Premium Player (Luxury Strategy)
- **Minute**: Careful piloting, optimal POI viewing
- **Session**: Fewer ships, complex routes, Adel class focus
- **Act**: Scale through reputation, VIP contracts

### Explorer Player (Discovery Strategy)
- **Minute**: Curious routing, discovering new areas
- **Session**: Chart purchases, fog of war clearing
- **Act**: Scale through content unlocks, POI completionism

### Story Player (Narrative Focus)
- **Minute**: Relaxed piloting, enjoy scenery
- **Session**: Family interactions prioritized
- **Act**: Scale through story beats, character development

All playstyles are viable and supported by the loop structure.

---

## Success Metrics

### Minute-to-Minute
- **Engagement**: Players focused, not distracted (> 80% attention)
- **Satisfaction**: Voyages feel rewarding to complete (> 7/10 rating)
- **Learning**: Skills visibly improve over time (fewer groundings, faster docks)

### Session-to-Session
- **Completion**: 90%+ of started sessions reach natural stopping point
- **Anticipation**: 75%+ of players look forward to next session
- **Variety**: No two sessions feel exactly the same (route variety, events)

### Act-to-Act
- **Completion**: 50%+ reach Act 3, 40%+ finish the game
- **Emotional Investment**: 60%+ report caring about story outcome
- **Replayability**: 30%+ want to try different approaches (new playthroughs)

---

## Design Notes

> **On Loop Length**: Minute-to-minute must be short enough not to fatigue (5-15 min voyages). Session-to-session must have clear stopping points (after strategic planning). Act-to-act must feel like distinct chapters, not arbitrary divisions.

> **On Skill Expression**: The minute-to-minute loop is where skill matters most. Players must feel their piloting ability improves and matters. Session-to-session is where strategic thinking matters. Act-to-act is where narrative choices matter.

> **On Progressive Complexity**: Each act adds layers without removing the core loop. Act 1 players pilot one ship. Act 3 players pilot one ship the same way, but now it's surrounded by 20 delegated ships, historical events, and family considerations.

> **On Emotional Beats**: Story moments should never interrupt the tactical loop (mid-voyage), but should punctuate the strategic loop (between voyages). This keeps gameplay responsive while allowing narrative breathing room.

---

## Implementation Priorities

### Phase 1 (Core Loop Prototype)
- Minute-to-minute loop must be fun and polished first
- Can test with simple session loop (one route, basic economy)
- Act progression can be simulated/skipped

### Phase 2 (Strategic Layer)
- Session-to-session loop with route planning
- Multi-ship basics (even without full delegation)
- Resource management and upgrades

### Phase 3 (Narrative Integration)
- Act structure implementation
- Story beats and pacing
- Family and historical elements

The core loop must be fun before adding narrative depth.

---

**Document End**

*For related systems, see: [Player Fantasy](../Narrative/GDD_Player_Fantasy.md), [Act Progression](../Progression/GDD_Act_Progression.md), [Ship Direct Control](../ShipControl/GDD_Ship_Direct_Control.md)*
