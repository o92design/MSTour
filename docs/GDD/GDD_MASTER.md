# MS TOUR - Game Design Document
## Master Document

**Document Type:** Master GDD  
**Version:** 1.0  
**Last Updated:** 2026-01-25  
**Owner:** Lead Designer

---

## Table of Contents

1. [Executive Summary](#executive-summary)
2. [Vision Statement](#vision-statement)
3. [Core Design Pillars](#core-design-pillars)
4. [High-Level Gameplay Loop](#high-level-gameplay-loop)
5. [Three-Act Structure](#three-act-structure)
6. [Player Fantasy](#player-fantasy)
7. [Technical Requirements](#technical-requirements)
8. [Documentation Map](#documentation-map)
9. [Quick Reference](#quick-reference)
10. [Development Roadmap](#development-roadmap)
11. [Success Metrics](#success-metrics)

---

## Executive Summary

**Title:** MS Tour  
**Genre:** Shipping Company Management Simulation  
**Setting:** Gothenburg Archipelago, Sweden (1887-1950s)  
**Platform:** Windows, Linux  
**Target Performance:** 60 FPS with 50+ ships, 20+ islands  
**Core Experience:** Build and manage a passenger shipping company through direct ship control and strategic business decisions

### One-Sentence Pitch
*"Build a shipping empire in the Gothenburg archipelago, mastering both the helm and the ledger, from ambitious young captain to patriarch leaving a legacy."*

### Target Audience
- **Primary**: Strategy/simulation enthusiasts (25-45 years old)
- **Secondary**: History buffs, nautical fans, narrative-driven players
- **Appeal**: Skill-based gameplay + strategic depth + emotional storytelling

### Key Features
- **Direct Ship Control**: Pilot ships with arcade-style physics
- **Business Management**: Routes, pricing, fleet optimization
- **Progressive Narrative**: Three-act life story with family and succession
- **Historical Integration**: Real VIPs, events from 1887-1950s
- **Discovery System**: Fog of war archipelago exploration
- **Risk & Challenge**: Weather, grounding, capsizing, bankruptcy
- **Emotional Framing**: Deathbed reflection with "Till havs" folk song

---

## Vision Statement

**MS Tour** is a shipping company management simulation that combines **hands-on ship piloting** with **strategic business management** in the historically rich Gothenburg archipelago. Players experience the life journey of a shipping magnate—from ambitious young captain to established patriarch—building a tourism empire while navigating literal and metaphorical waters.

The game is framed as a **deathbed reflection**, with the Swedish folk song *"Till havs"* serving as the emotional through-line. Players relive their life's work, building relationships, raising a family, and ultimately choosing a successor to carry on their legacy.

### What Makes MS Tour Unique?

1. **Hands-On + Strategic**: Unlike pure management sims, players pilot their own ships. Unlike pure simulation games, players also manage a growing business empire.

2. **Life Story Arc**: The three-act structure spanning decades creates a complete life narrative, not just a business story.

3. **Meaningful Risk**: Grounding, capsizing, and bankruptcy aren't just failure states—they're dramatic moments that shape your story.

4. **Historical Authenticity**: Real Swedish history, real VIPs, real locations. Players aren't just playing a game; they're experiencing history.

5. **Emotional Resonance**: "Till havs" and the deathbed framing create nostalgia and reflection. Players should feel they've lived a life, not just played a game.

---

## Core Design Pillars

### 1. Strategic Optimization
**Definition**: Players make meaningful choices about routes, fleet composition, employee management, and business strategy. Multiple paths to success reward different playstyles.

**Implementation**:
- Route profitability depends on passenger preferences, distance, POIs, seasonality
- Fleet composition: Sailing (cheap, slow) vs Steam (expensive, reliable) vs Hybrid
- Employee skills: Generalists vs specialists, cost vs effectiveness
- Pricing strategies: Volume (cheap tours) vs Premium (luxury service)

**Design Principle** (Ernest Adams): Offer clear choices with understandable trade-offs. No single "best" strategy.

### 2. Progressive Complexity
**Definition**: The game introduces systems gradually across three acts, teaching through narrative context. Act 1 focuses on piloting fundamentals, Act 2 introduces fleet management, Act 3 adds historical events and succession planning.

**Implementation**:
- **Act 1**: Single ship, manual piloting, basic routes, simple satisfaction
- **Act 2**: Fleet of 5-10 ships, employee delegation, advanced amenities, prestige system
- **Act 3**: Fleet of 20+ ships, VIP contracts, historical events, succession choices

**Design Principle** (Raph Koster): Teach patterns clearly before adding complexity. Each act introduces new "grammars" of play.

### 3. Environmental Interaction
**Definition**: The archipelago is a living, challenging environment. Weather, seasons, depth charts, grounding risks, and sea conditions create moment-to-moment gameplay challenge.

**Implementation**:
- Real-time weather: wind affects ship handling, fog reduces visibility
- Seasonal cycles: summer is peak tourism, winter brings ice and storms
- Chart quality: poor charts increase grounding risk, discovering new areas
- Sea hazards: shallow waters, narrow passages, rocky shores

**Design Principle** (Jesse Schell): The environment is a character in the game, not just a backdrop.

### 4. Business Management with Heart
**Definition**: Behind the spreadsheets and routes are people—passengers with preferences, employees with skills, a family with needs, and historical figures with stories. Management is humanized through character and narrative.

**Implementation**:
- Passengers have classes, traits, preferences (not just numbers)
- Employees have personalities, backstories, skill progression
- Wife is co-narrator, deeply developed character
- Children have unique traits, ambitions, relationships
- VIPs unlock through reputation, bring stories and contracts

**Design Principle** (Jesse Schell): Emotional engagement transforms mechanical systems into memorable experiences.

---

## High-Level Gameplay Loop

### The Core Loop (Session-to-Session)

```
┌─────────────────────────────────────────────────────────────┐
│                    STRATEGIC LAYER                          │
│                    (Harbor & Planning Room)                 │
│                                                             │
│  1. Review previous voyages (profit, satisfaction,          │
│     reputation changes)                                     │
│  2. Make strategic decisions:                               │
│     - Design new routes or optimize existing ones           │
│     - Purchase better charts for new areas                  │
│     - Hire employees or upgrade skills                      │
│     - Buy/upgrade ships and amenities                       │
│     - Set pricing and tour parameters                       │
│  3. Assign ships to routes, assign captains to ships        │
└─────────────────────────────────────────────────────────────┘
                            ↓
┌─────────────────────────────────────────────────────────────┐
│                    TACTICAL LAYER                           │
│                    (Direct Ship Control)                    │
│                                                             │
│  4. Execute voyages:                                        │
│     - Pilot ship manually (or delegate to captain)          │
│     - Navigate safely (avoid grounding/capsizing)           │
│     - Visit planned POIs in sequence                        │
│     - Manage passenger satisfaction in real-time            │
│     - Respond to weather, events, opportunities             │
│  5. Dock at islands, manage cargo/passengers                │
└─────────────────────────────────────────────────────────────┘
                            ↓
┌─────────────────────────────────────────────────────────────┐
│                    OUTCOME LAYER                            │
│                    (Results & Progression)                  │
│                                                             │
│  6. Receive outcomes:                                       │
│     - Passenger satisfaction → Revenue                      │
│     - Reputation changes → Unlock VIPs, areas, contracts    │
│     - Employee XP → Skill improvements                      │
│     - Family interactions → Story progression               │
│  7. Unlock new content:                                     │
│     - New islands and POIs                                  │
│     - Advanced ship types                                   │
│     - Historical VIPs and events                            │
│     - Family milestones and story beats                     │
└─────────────────────────────────────────────────────────────┘
                            ↓
                    (Return to Strategic Layer)
```

### Minute-to-Minute Gameplay (During Voyage)

1. **Navigate**: Steer ship using WASD/arrows, adjust speed
2. **Monitor**: Watch depth indicator, weather changes, passenger mood
3. **React**: Avoid shallow water, adjust for wind, respond to events
4. **Manage**: Balance speed (faster arrival) vs comfort (smoother ride)
5. **Arrive**: Dock at POIs, observe passengers, move to next waypoint

### Session Length
- **Short session (30 min)**: Complete 2-3 routes, make minor adjustments
- **Medium session (60-90 min)**: Full tour circuit, strategic upgrades, story beats
- **Long session (2-3 hours)**: Significant progression through an act, major decisions

---

## Three-Act Structure

### Act 1: The Young Captain (1887-1900s)
**Duration**: ~5-8 hours of gameplay  
**Theme**: Foundation and Romance  
**Player Status**: Solo captain with one ship

#### Focus
- Learning ship piloting mechanics
- Establishing first profitable routes
- Courting your future wife
- Building local reputation

#### Systems Introduced
- **Direct Ship Control**: WASD piloting, throttle, turning
- **Grounding System**: Depth indicator, chart quality affects risk
- **Docking**: Manual docking (auto-dock unlocks later)
- **Basic Tours**: Simple routes with 1-3 POIs
- **Passenger Satisfaction**: Simple model (comfort + POI quality)
- **Economy Basics**: Revenue, costs, profit

#### Narrative Beats
1. **Opening**: Deathbed scene, "Till havs" begins, flashback to 1887
2. **First Command**: Inherit/purchase your first modest ship
3. **Meeting Her**: Encounter future wife (passenger or related to shipping)
4. **First Successful Season**: Prove yourself as a captain
5. **Courtship**: Tours become opportunities to impress her
6. **The Wedding**: Marriage ceremony, wife joins your story
7. **First Child**: Son or daughter born, family begins
8. **Company Established**: Officially founded business, name your company

#### Gameplay Characteristics
- **Single ship operations**: You pilot it yourself
- **Learning curve**: Tutorial is embedded in narrative
- **Local routes**: Starting area around Gothenburg
- **Personal scale**: You know your passengers, hands-on management
- **Resource constraints**: Money is tight, every decision matters

#### End State
- Proficient at ship piloting
- 1-2 ships owned
- Basic route network established
- Wife and one child
- Local reputation built

### Act 2: The Shipping Magnate (1900s-1920s)
**Duration**: ~8-12 hours of gameplay  
**Theme**: Growth and Delegation  
**Player Status**: Business owner with growing fleet

#### Focus
- Fleet management (5-15 ships)
- Employee hiring and delegation
- Business optimization
- Raising family (2-4 children)
- Expanding to full archipelago
- Securing VIP contracts

#### Systems Introduced
- **Multiple Ships**: Manage fleet, not just one ship
- **Captain Delegation**: Assign AI captains to ships, trust system
- **Advanced Ship Types**: Steam ships, hybrid vessels
- **Prestige System**: Reputation tiers unlock content
- **VIP Contracts**: Oscar II, Alfred Nobel, famous passengers
- **Employee Skills**: Specializations, XP progression
- **Advanced Amenities**: Dining rooms, observation decks, bars
- **Route Optimization Tools**: Planning room enhancements

#### Narrative Beats
1. **Growth Pains**: Expanding faster than you can manage
2. **First Hire**: Hiring your first captain, learning to delegate
3. **Children Growing**: Kids show personalities, interests
4. **Oscar II Contract**: Royal patronage, prestige boost
5. **Nobel Aboard**: Alfred Nobel as passenger, historical conversations
6. **Celebrity Status**: Newspaper coverage, household name
7. **Business Peak**: Your company dominates archipelago tourism
8. **Family Challenges**: Balancing business success with family time

#### Gameplay Characteristics
- **Multi-ship coordination**: Can't pilot everything yourself
- **Strategic focus**: Less piloting, more planning and delegation
- **Archipelago-wide**: Access to 15-20 islands
- **Employee management**: Hiring, training, assigning
- **Reputation matters**: VIPs unlock through prestige
- **Family integration**: Kids appear in scenes, have dialogue

#### End State
- Fleet of 10-20 ships
- Skilled employee roster
- Full archipelago access
- Multiple children (teenagers/young adults)
- VIP relationships established
- Peak business success

### Act 3: The Patriarch/Matriarch (1920s-1950s)
**Duration**: ~7-10 hours of gameplay  
**Theme**: Legacy and Succession  
**Player Status**: Established magnate facing historical challenges

#### Focus
- Maintaining empire through turbulent times
- Navigating WWI and WWII impacts
- Economic volatility and adaptability
- Training potential successor
- Reflecting on life choices
- Preparing for legacy transfer

#### Systems Introduced
- **Historical Events**: WWI, WWII, economic crashes, tech changes
- **Succession System**: Evaluate children as heirs
- **Economic Volatility**: Wars and depressions affect business
- **Training Mechanics**: Teach chosen heir the business
- **Reflection Moments**: Deathbed narration intensifies
- **End-Game Choices**: Who inherits, what legacy you leave

#### Narrative Beats
1. **WWI Begins**: War impacts tourism, difficult decisions
2. **Economic Challenges**: Depression, maintaining operations
3. **Children's Paths**: Kids make life choices, some leave business
4. **Choosing Heir**: Decision about who will inherit
5. **Training Successor**: Teaching next generation
6. **WWII**: Even greater challenges, survival mode
7. **Post-War Recovery**: Rebuilding, reflecting on resilience
8. **Final Years**: Preparing to pass torch
9. **Deathbed**: Full circle to opening, final reflections
10. **Legacy**: Epilogue showing heir's first years, your impact

#### Gameplay Characteristics
- **Established empire**: More about maintaining than building
- **Historical reactivity**: Events force adaptation
- **Succession focus**: Evaluating and training heir
- **Reflection**: More narrative, less new systems
- **Legacy decisions**: Choices that affect ending
- **Emotional weight**: "Till havs" reaches conclusion

#### Multiple Endings
Endings vary based on:
- Which child inherits (if any)
- Economic state at death (prosperous vs bankrupt)
- Reputation/prestige level achieved
- Family relationships (close vs estranged)
- Historical choices made during wars/depressions

**Possible Endings**:
- **Golden Years**: Successful heir, prosperous company, happy family
- **Bittersweet Success**: Successful company, but family estranged
- **Humble Legacy**: Modest company, but close family and integrity
- **Tragic Decline**: Bankruptcy, loss, but lessons learned
- **No Heir**: Company sold/dissolved, but money secured for family

---

## Player Fantasy

### Core Fantasy
**"I am a shipping captain who built an empire with my own hands and lived a full life."**

### Emotional Goals

#### Players Should Feel:

1. **Mastery** of ship piloting and navigation
   - "I've learned to handle any weather, any passage"
   - Visible skill progression from struggling novice to expert captain

2. **Pride** in building a successful business
   - "I built this from one small ship to a fleet of 20"
   - Seeing ships with your company livery all over archipelago

3. **Connection** to family and historical figures
   - "My wife was there through everything"
   - "I carried Oscar II himself on my ships"
   - Children feel like real people with personalities

4. **Nostalgia** through the deathbed framing
   - "Looking back, those were good years"
   - "Till havs" evokes emotional response
   - Bittersweet reflection on life well-lived

5. **Accomplishment** in leaving a legacy
   - "My child will carry on what I built"
   - "I made my mark on history"
   - Epilogue shows lasting impact

### The Experience We're Creating

MS Tour should feel like:
- **70% Management/Strategy**: Planning routes, managing business, making decisions
- **20% Skill/Action**: Piloting ships, navigating challenges
- **10% Narrative/Emotion**: Story moments, family interactions, reflection

But the 10% narrative carries outsized emotional weight, transforming the 90% gameplay into a story worth telling.

### What Players Talk About

After playing MS Tour, players should say:
- "Remember that time I almost capsized in a storm with Nobel aboard?"
- "My daughter inherited the business and she did even better than me"
- "I cried during the deathbed scene with my wife"
- "I've never felt more accomplished in a game"
- "The song 'Till havs' still gives me chills"

---

## Technical Requirements

### Performance Targets

| Metric | Target | Critical Limit |
|--------|--------|----------------|
| Frame Rate | 60 FPS | 30 FPS minimum |
| Ships Active | 50+ | 30 minimum |
| Islands Rendered | 20+ visible | 15 minimum |
| Load Time (scene) | < 3 seconds | < 5 seconds |
| Memory Usage | < 2 GB | < 4 GB |
| Save/Load Time | < 1 second | < 2 seconds |

### Platform Requirements

#### Windows
- **OS**: Windows 10/11 (64-bit)
- **CPU**: Intel i5-6600K / AMD Ryzen 5 1600 or equivalent
- **RAM**: 8 GB
- **GPU**: GTX 1060 / RX 580 or equivalent (4 GB VRAM)
- **Storage**: 5 GB

#### Linux
- **OS**: Ubuntu 20.04+ or equivalent
- **CPU**: Intel i5-6600K / AMD Ryzen 5 1600 or equivalent
- **RAM**: 8 GB
- **GPU**: GTX 1060 / RX 580 or equivalent (4 GB VRAM)
- **Storage**: 5 GB

### Input Support
- **Keyboard + Mouse**: Primary input method
  - WASD or Arrow keys for ship control
  - Mouse for UI navigation and planning
- **Gamepad**: Nice-to-have (post-launch)

### Architecture Principles

#### Data-Oriented Design
- Systems process data in batch (all ships, all passengers)
- Struct-of-Arrays (SoA) for hot paths
- Cache-friendly memory layouts
- Minimize pointer chasing

#### Visual Architecture
- **Orthographic camera**: Top-down/angled view
- **3D ships**: Fully modeled, animated, customizable
- **Pre-rendered backgrounds**: 2D islands, water, terrain
- **Hybrid approach**: 3D for dynamic elements, 2D for static world

#### System Architecture
Core systems (detailed in Technical/EngineArchitecture.md):
- **Ship Physics System**: Movement, collision, grounding
- **Route System**: Waypoints, pathfinding, execution
- **Passenger System**: Satisfaction, traits, behavior
- **Employee System**: Skills, AI captains, delegation
- **Economy System**: Revenue, costs, cash flow
- **Weather System**: Wind, waves, seasonal effects
- **Narrative System**: Story beats, dialogue, family
- **Progression System**: Unlocks, XP, prestige

### Technical Constraints
- **50+ ships**: Must support AI captains running routes simultaneously
- **Real-time weather**: Affects all ships dynamically
- **Fog of war**: Efficient visibility/discovery system
- **Save system**: Complete game state, quick save/load
- **Cross-platform**: Same experience on Windows and Linux

---

## Documentation Map

This GDD is structured as a modular system. Each area has detailed design documents.

### Core Experience Documents
- **[Core Gameplay Loop](docs/GDD_Core_Gameplay_Loop.md)** - Detailed minute-to-minute, session-to-session, and act-to-act loops
- **[Player Fantasy](docs/GDD_Player_Fantasy.md)** - Deep dive into what makes MS Tour compelling and emotionally resonant
- **[Design Pillars](docs/GDD_Design_Pillars.md)** - Full exploration of the four pillars with examples

### System Design Documents (Organized by Area)

#### Narrative & Story
- **[Three-Act Structure](Narrative/GDD_Three_Act_Structure.md)** - Complete act breakdown, narrative beats, pacing
- **[Framing Device](docs/GDD_Framing_Device.md)** - Deathbed opening, "Till havs", emotional design
- **[Character Design](docs/GDD_Character_Design.md)** - Wife, children, succession characters, development arcs
- **[Historical Context](docs/GDD_Historical_Context.md)** - 1887 Sweden, accuracy standards, research

#### Ship Control & Physics
- **[Direct Control System](docs/GDD_Ship_Direct_Control.md)** - Controls, camera, handling, arcade physics
- **[Grounding System](docs/GDD_Grounding_System.md)** - Depth, charts, grounding mechanics, severity
- **[Capsizing System](docs/GDD_Capsizing_System.md)** - Stability, weather, waves, failure states
- **[Docking System](docs/GDD_Docking_System.md)** - Manual vs auto, skill progression, XP
- **[Physics Model](docs/GDD_Ship_Physics.md)** - Arcade physics, environmental factors, feel

#### Tourism & Passengers
- **[Interest Points System](docs/GDD_Interest_Points.md)** - Nature/Historical/Military POIs, two-tier system
- **[Passenger System](docs/GDD_Passenger_System.md)** - Classes, traits, preferences, behavior
- **[Tours Design](docs/GDD_Tours_Design.md)** - Route planning, satisfaction, optimization
- **[Satisfaction Mechanics](docs/GDD_Satisfaction_Mechanics.md)** - Factors, feedback, scoring
- **[Prestige System](docs/GDD_Prestige_System.md)** - Reputation tiers, unlocks, progression

#### Ship Building & Fleet
- **[Modular Ship System](docs/GDD_Modular_Ships.md)** - Bow/Mid/Stern, upgrades, progression
- **[Ship Types](docs/GDD_Ship_Types.md)** - Sailing/Steam/Hybrid, historical accuracy, handling
- **[Amenities System](docs/GDD_Ship_Amenities.md)** - Galley, bar, dining, observation deck, quality tiers
- **[Visual Customization](docs/GDD_Ship_Customization.md)** - Paint schemes, flags, identity

#### Employee Management
- **[Employee Types](docs/GDD_Employee_Types.md)** - Captain, Guide, Service, Engineer, Navigator roles
- **[Hiring System](docs/GDD_Employee_Hiring.md)** - Skills, traits, costs, hiring decisions
- **[Skill Progression](docs/GDD_Employee_Skills.md)** - XP, leveling, specializations, mastery
- **[Delegation Mechanics](docs/GDD_Delegation.md)** - AI captains, trust system, automation

#### Planning & Discovery
- **[Planning Room](docs/GDD_Planning_Room.md)** - HQ, map table, UI/UX, functionality
- **[Sea Charts System](docs/GDD_Sea_Charts.md)** - Quality tiers, cost, discovery, fog of war
- **[Route Design Tools](docs/GDD_Route_Design.md)** - Planning UI, waypoints, optimization
- **[Fog of War](docs/GDD_Fog_Of_War.md)** - Discovery mechanics, famous landmarks, reveal

#### Progression Systems
- **[Act Progression](docs/GDD_Act_Progression.md)** - How gameplay evolves, system unlocks
- **[Skill Trees](docs/GDD_Skill_Trees.md)** - Captain and employee progression trees
- **[Unlock Systems](docs/GDD_Unlock_Systems.md)** - Ships, islands, routes, VIPs, features
- **[Economic Progression](docs/GDD_Economic_Progression.md)** - Income growth, cost scaling, milestones

#### Historical Integration
- **[VIP System](docs/GDD_VIP_System.md)** - Oscar II, Nobel, Strindberg, Lagerlöf, unlocks
- **[Historical Events](docs/GDD_Historical_Events.md)** - WWI, WWII, economic events, impacts
- **[Newspaper System](docs/GDD_Newspaper_System.md)** - Weekly editions, headlines, tracking
- **[Historical Accuracy](docs/GDD_Historical_Accuracy.md)** - Research standards, authenticity

#### Family & Succession
- **[Wife System](docs/GDD_Wife_System.md)** - Romance, character depth, co-narrator role
- **[Children System](docs/GDD_Children_System.md)** - Birth, traits, personality, integration
- **[Succession Mechanics](docs/GDD_Succession.md)** - Training heir, evaluation, consequences
- **[Gender Choice](docs/GDD_Gender_Choice.md)** - Grand-papa vs Grand-mama, difficulty, barriers

#### Risk & Challenge
- **[Weather System](docs/GDD_Weather.md)** - Wind, waves, fog, rain, visibility, dynamics
- **[Seasonal Cycle](docs/GDD_Seasons.md)** - Four seasons, ice, storms, economic impacts
- **[Bankruptcy System](docs/GDD_Bankruptcy.md)** - Debt mechanics, failure states, somber ending
- **[Rescue Mechanics](docs/GDD_Rescue.md)** - Helping grounded ships, reputation effects
- **[Dynamic Events](docs/GDD_Dynamic_Events.md)** - Emergent challenges, opportunities

#### Economy
- **[Pricing Model](docs/GDD_Economy_Pricing.md)** - Tour pricing by class, route, quality
- **[Cost Structure](docs/GDD_Economy_Costs.md)** - Ship purchase, maintenance, wages, repairs
- **[Profit Optimization](docs/GDD_Economy_Optimization.md)** - Margins, efficiency, strategies
- **[Resource Management](docs/GDD_Economy_Resources.md)** - Fuel, provisions, cash flow

#### UI/UX
- **[Harbor Scene](docs/GDD_UIUX_Harbor.md)** - Main hub, interactions, visual design
- **[Planning Interface](docs/GDD_UIUX_Planning.md)** - Map table, route planning UI
- **[In-Voyage UI](docs/GDD_UIUX_Voyage.md)** - Ship control HUD, passenger feedback, nav aids
- **[Menus and Screens](docs/GDD_UIUX_Menus.md)** - All screens, flows, information architecture

#### Content
- **[Archipelago Map](docs/GDD_Content_Map.md)** - All islands, distances, historical accuracy
- **[Interest Point Database](docs/GDD_Content_POIs.md)** - Complete catalog of POIs
- **[Character Roster](docs/GDD_Content_Characters.md)** - All named NPCs, VIPs, family
- **[Tour Catalog](docs/GDD_Content_Tours.md)** - Example tours, pricing, target audiences

#### Audio & Aesthetics
- **[Music Design](docs/GDD_Audio_Music.md)** - "Till havs" integration, period music, emotional beats
- **[Sound Design](docs/GDD_Audio_Sound.md)** - Ships, weather, passengers, UI feedback
- **[Visual Style](docs/GDD_Visual_Style.md)** - Orthographic view, 1887 aesthetic, art direction

#### Technical
- **[Engine Architecture](docs/GDD_Tech_Architecture.md)** - System mapping, data-oriented design
- **[Data Structures](docs/GDD_Tech_Data_Structures.md)** - SoA approach, key data layouts
- **[Performance Targets](docs/GDD_Tech_Performance.md)** - 60 FPS requirements, optimization
- **[Platform Considerations](docs/GDD_Tech_Platforms.md)** - Windows/Linux, cross-platform

#### Difficulty & Accessibility
- **[Difficulty Modes](docs/GDD_Difficulty_Modes.md)** - Easy/Normal/Hard differences
- **[Assist Options](docs/GDD_Assist_Options.md)** - Toggleable aids, accessibility
- **[Onboarding](docs/GDD_Onboarding.md)** - Tutorial structure, learning curve
- **[Player Guidance](docs/GDD_Player_Guidance.md)** - How systems teach themselves

#### Future Content
- **[Goods DLC](docs/GDD_Future_Goods_DLC.md)** - Cargo transport expansion design
- **[International DLC](docs/GDD_Future_International_DLC.md)** - Beyond archipelago
- **[Post-Launch Content](docs/GDD_Future_Post_Launch.md)** - Additional VIPs, events, content

---

## Quick Reference: Systems at a Glance

### Core Systems Summary

| System | What It Does | Player Interaction | Why It Matters |
|--------|-------------|-------------------|----------------|
| **Ship Control** | Direct piloting with arcade physics | WASD/arrows, throttle, steering | Core skill expression, moment-to-moment gameplay |
| **Grounding** | Depth hazard, depends on chart quality | Avoid shallow water, buy better charts | Risk/reward of exploring, skill check |
| **Capsizing** | Stability affected by weather/waves | Manage speed, angle in rough seas | Dramatic failure state, weather matters |
| **Docking** | Precision arrival at ports/POIs | Manual control until auto-dock unlocked | Skill progression, satisfying to master |
| **Tourism** | Route planning + passenger satisfaction | Design tours, select POIs, set pricing | Core business mechanic, optimization |
| **Passengers** | Classes, traits, preferences | Choose target audience, manage expectations | Variety, strategy, humanization |
| **Satisfaction** | Feedback system for tour quality | Comfort + POI quality + service | Success metric, drives revenue |
| **Prestige** | Reputation ladder | Build through satisfied passengers, VIPs | Unlocks content, validates success |
| **Ships** | Fleet vehicles with stats | Buy, upgrade, customize, assign | Progression, variety, identity |
| **Amenities** | Ship facilities (galley, bar, etc.) | Purchase and upgrade | Affects satisfaction, premium strategy |
| **Employees** | AI helpers with skills | Hire, assign, train | Delegation, scale up operations |
| **Captains** | AI pilots for delegated ships | Assign to ships, trust system | Multi-ship management, Act 2 focus |
| **Routes** | Planned tour circuits | Create in planning room, optimize | Strategic layer, puzzle-like |
| **Charts** | Maps with varying detail | Purchase better quality | Discovery, fog of war, progression |
| **Planning Room** | Strategy headquarters | Design routes, buy charts, analyze | Strategic hub, "war room" feel |
| **VIPs** | Historical figures as passengers | Unlock through prestige, special contracts | Historical integration, prestige |
| **Historical Events** | WWI, WWII, economic changes | React and adapt strategy | Act 3 challenge, historical immersion |
| **Weather** | Dynamic conditions | Adapt piloting, route timing | Environmental challenge, variety |
| **Seasons** | Four seasonal cycles | Plan around peak tourism seasons | Economic timing, visual variety |
| **Family** | Wife and children characters | Dialogue, story choices, training heir | Narrative depth, emotional investment |
| **Succession** | Choose and train heir | Evaluate children, make choice | End-game focus, legacy theme |
| **Newspapers** | Weekly news coverage | Read about events, see your reputation | World building, feedback, immersion |
| **Economy** | Money management | Revenue - costs = profit, invest | Resource constraint, business strategy |
| **Bankruptcy** | Failure state | Manage debt, avoid ruin | Stakes, dramatic tension |

### System Interactions (Key Relationships)

```
Ship Control + Charts = Navigation Risk
  └─> Better charts = Less grounding risk

Tourism + Passengers = Route Design
  └─> Match POIs to passenger preferences

Prestige + VIPs = Historical Integration
  └─> High prestige unlocks famous passengers

Employees + Delegation = Scale
  └─> Hire captains to manage multiple ships

Weather + Ship Control = Skill Expression
  └─> Adapt piloting to conditions

Economy + Satisfaction = Business Success
  └─> Satisfied passengers = Revenue = Growth

Family + Narrative = Emotional Investment
  └─> Characters transform mechanics into story

Historical Events + Economy = Act 3 Challenge
  └─> Wars and depressions test adaptability
```

---

## Design Philosophy

### Inspiration & Principles

This GDD is informed by established game design theory:

#### Jesse Schell's "The Art of Game Design"
- **Lens of Essential Experience**: What is the one thing players must feel? (Mastery + Legacy)
- **Lens of Challenge**: Challenges are tuned to create flow state
- **Lens of Skill**: Clear skill expression in piloting and strategy
- **Lens of Time**: Three-act structure creates meaningful time arc

**Application**: Every system asks "Does this serve the essential experience?"

#### Raph Koster's "A Theory of Fun"
- **Fun is learning**: Systems teach clear patterns (grounding → depth awareness)
- **Mastery is satisfying**: Visible progression from novice to expert captain
- **Complexity layering**: Each act adds new "grammar" to learn

**Application**: Progressive complexity aligns with skill acquisition curve

#### Ernest Adams' "Fundamentals of Game Design"
- **Player-centric design**: Features exist to serve player experience, not just realism
- **Clear mechanics**: Rules are understandable and consistent
- **Explicit balance**: Tuning values are documented and adjustable

**Application**: All systems have clear player-facing goals and feedback

### Design Values

1. **Clarity over Complexity**: Systems should be understandable before they're deep
2. **Meaning over Mechanics**: Every number should represent something tangible
3. **Choice over Optimization**: Multiple viable strategies, not one best path
4. **Story over Systems**: Narrative transforms mechanics into memorable experiences
5. **Skill over Stats**: Player ability matters as much as ship/employee stats

---

## Development Roadmap

### Phase 1: Core Loop (Months 1-6)
**Goal**: Prove the core gameplay loop is fun

**Deliverables**:
- Ship control with arcade physics (playable, feels good)
- Basic grounding system (depth, visual feedback, grounding)
- Simple route system (waypoints, execution)
- Single ship operations (player-controlled only)
- Core economy loop (costs, revenue, profit)
- One playable island + harbor area (Gothenburg)
- Basic UI (harbor, in-voyage HUD)

**Milestone**: Can play 30-minute loop (plan → execute → profit → upgrade)

### Phase 2: Tourism & Content (Months 7-12)
**Goal**: Add depth and variety to tours and passengers

**Deliverables**:
- Passenger system (classes, traits, preferences)
- Interest Points (3 types, 20+ POIs)
- Satisfaction mechanics (formula, feedback)
- Prestige system (tiers, unlocks)
- Full archipelago map (20+ islands)
- Weather system (wind, waves, fog)
- Sea charts (quality tiers, fog of war)
- Planning Room UI (route design, chart purchase)

**Milestone**: Engaging loop with strategic route optimization

### Phase 3: Management & Scale (Months 13-18)
**Goal**: Enable fleet operations and delegation

**Deliverables**:
- Multi-ship fleet support (AI pathfinding)
- Employee system (types, skills, progression)
- Captain delegation (AI pilots, trust)
- Ship building (types, modular upgrades, amenities)
- Advanced route tools (optimization, profitability analysis)
- Seasonal cycle (4 seasons, visual changes, economic impact)
- Docking progression (manual → auto-dock unlock)
- Fleet management UI

**Milestone**: Managing 10+ ships with delegated captains

### Phase 4: Narrative & Polish (Months 19-24)
**Goal**: Implement three-act structure and family story

**Deliverables**:
- Three-act implementation (pacing, system unlocks)
- Wife character system (romance, co-narrator, dialogue)
- Children system (birth, traits, personality, growth)
- Succession mechanics (evaluation, training, choice)
- VIP system (Oscar II, Nobel, etc., contracts)
- Historical events (WWI, WWII, economic events)
- Newspaper system (weekly editions, headlines)
- Framing device ("Till havs", deathbed reflection)
- Capsizing system (stability, failure state)
- Bankruptcy system (debt, failure narrative)

**Milestone**: Complete playthrough from Act 1 to Act 3 ending

### Phase 5: Expansion & Launch (Months 25-30)
**Goal**: Polish, balance, and prepare for launch

**Deliverables**:
- Full balance pass (difficulty modes, tuning)
- Complete historical event timeline
- All VIPs implemented
- Assist options (accessibility features)
- Onboarding/tutorial polish
- Audio implementation (music, sound effects)
- Visual polish (effects, animations, UI)
- Performance optimization (60 FPS target)
- Localization preparation (Swedish + English minimum)
- Bug fixing and QA
- Launch preparation (store pages, marketing)

**Milestone**: Shippable, polished product

---

## Success Metrics

### Player Experience Metrics

#### Engagement
- **Tutorial Completion**: 80%+ of players complete Act 1
  - Measures: Onboarding effectiveness, initial hook
- **Act 2 Reach**: 60%+ of players reach Act 2
  - Measures: Core loop satisfaction
- **Act 3 Reach**: 50%+ of players reach Act 3
  - Measures: Long-term engagement
- **Completion Rate**: 40%+ finish the game
  - Measures: Narrative payoff, sustained interest

#### Session Metrics
- **Average Session**: 60-90 minutes
  - Measures: Loop satisfaction, clear stopping points
- **Sessions per Week**: 3-4 for active players
  - Measures: Sustained interest without burnout
- **Return Rate**: 70%+ return within 7 days
  - Measures: Compulsion loop effectiveness

#### Satisfaction
- **Player-Reported Mastery**: 8+/10 feel skilled at piloting by Act 2
  - Measures: Learning curve effectiveness
- **Emotional Connection**: 60%+ report emotional investment in story
  - Measures: Narrative effectiveness
- **Strategic Satisfaction**: 75%+ feel their decisions matter
  - Measures: Choice meaningfulness
- **Recommendation**: 70%+ would recommend to friends
  - Measures: Overall satisfaction

### Technical Metrics

#### Performance
- **Frame Rate**: 95%+ of gameplay at 60+ FPS
  - Critical for responsive ship control
- **Load Times**: < 3 seconds average between scenes
  - Maintains flow, reduces frustration
- **Crash Rate**: < 0.1% per hour of gameplay
  - Stability is critical for long sessions
- **Save/Load**: < 1 second for save, < 2 seconds for load
  - Frequent saving without interruption

#### Compatibility
- **Platform Parity**: Feature parity Windows/Linux
  - Measures: Cross-platform commitment
- **Hardware Range**: Playable on 5-year-old mid-range PCs
  - Measures: Accessibility

### Design Metrics

#### Balance
- **Strategy Diversity**: 3+ viable approaches to success
  - Volume/Premium/Balanced all work
  - Measures: Strategic depth
- **Difficulty Appropriateness**:
  - Normal: 40% complete without assist options
  - Easy: 60% complete
  - Hard: 20% complete
  - Measures: Challenge tuning
- **Grounding Frequency**: 1-2 groundings per hour in Act 1, reducing to 0.5 in Act 3
  - Measures: Skill progression visibility

#### Content
- **POI Engagement**: 80%+ of POIs visited by end of playthrough
  - Measures: Discovery appeal
- **VIP Encounters**: 70%+ of players unlock at least 3 VIPs
  - Measures: Prestige system appeal
- **Ship Variety**: Average player uses 4+ different ship types
  - Measures: Build diversity

#### Narrative
- **Family Engagement**: 75%+ read all family dialogues
  - Measures: Character investment
- **Succession Care**: 80%+ spend time evaluating heir options
  - Measures: End-game investment
- **"Till Havs" Recognition**: 60%+ mention the song in feedback
  - Measures: Emotional theming effectiveness

### Business Metrics (Post-Launch)

- **Sales Target**: 50,000 units in first year
- **Review Score**: 80+ on Metacritic / Steam
- **DLC Interest**: 30%+ would purchase Goods DLC
- **Player Retention**: 40%+ play 20+ hours

---

## Conclusion

MS Tour is a game about mastery, growth, and legacy. It combines the immediate satisfaction of skill-based ship piloting with the strategic depth of business management, all wrapped in an emotionally resonant narrative spanning a lifetime.

This modular GDD structure allows the development team to:
- **Work independently**: Each area has clear specifications
- **Maintain vision**: Main GDD ensures cohesion
- **Iterate efficiently**: Update individual documents without touching everything
- **Onboard quickly**: New team members can find relevant info fast

### The North Star

Every design decision should be evaluated against:
1. **Does it serve the player fantasy?** (Captain who built an empire)
2. **Does it align with our pillars?** (Strategic Optimization, Progressive Complexity, Environmental Interaction, Business with Heart)
3. **Is it implementable within constraints?** (60 FPS, data-oriented design)
4. **Does it create meaningful choices?** (Multiple valid strategies)
5. **Does it teach clearly?** (Players can learn and master)

If a feature doesn't pass these tests, it should be reconsidered or redesigned.

### Next Steps

1. **Review and approve** this master GDD
2. **Create detailed area documents** (see Documentation Map)
3. **Begin Phase 1 development** (Core Loop)
4. **Iterate based on playtesting** (Test the fantasy)
5. **Build toward launch** (Following roadmap)

**The sea awaits. Let's build something memorable.**

---
*"Till havs, till storms, du djärva jakt*  
*Till storms, till havs, var man på vakt*  
*Till havs!"*  
**Lyricist: Gustav Nordqvist**

---

**Document End**

*For detailed specifications on any system, see the linked area documents in the Documentation Map section.*
