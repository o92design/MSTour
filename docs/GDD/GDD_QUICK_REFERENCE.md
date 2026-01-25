# MS TOUR - GDD Quick Reference & Implementation Guide

**Document Type:** Reference / Implementation Guide  
**Version:** 1.0  
**Last Updated:** 2026-01-25  
**Owner:** Lead Designer  
**Purpose:** Quick reference for developers and designers, implementation priorities

---

## Document Status Overview

### ✅ Completed Core Documents

The following comprehensive design documents have been created:

1. **[GDD_MASTER.md](GDD_MASTER.md)** - Master overview document
   - Executive summary, vision, pillars
   - High-level systems overview
   - Technical requirements
   - Documentation map
   - Development roadmap
   - Success metrics

2. **[GDD_Core_Gameplay_Loop.md](./CoreGameplay/GDD_Core_Gameplay_Loop.md)** - Core loops at all scales
   - Minute-to-minute (tactical piloting)
   - Session-to-session (strategic management)
   - Act-to-act (narrative progression)
   - Loop integration and pacing

3. **[GDD_Ship_Direct_Control.md](./ShipControl/GDD_Ship_Direct_Control.md)** - Ship piloting system
   - Control scheme (keyboard/mouse/gamepad)
   - Movement physics (arcade-style)
   - Environmental effects (wind, waves, current)
   - Camera system
   - Docking mechanics
   - Skill progression
   - AI captain control

4. **[GDD_Passenger_System.md](./Tourism/GDD_Passenger_System.md)** - Passenger simulation
   - Four class system (Knegare/Borgare/Adel/VIP)
   - Passenger traits (30+ defined)
   - Preferences and expectations
   - Satisfaction calculation
   - Passenger generation
   - Strategic implications

5. **[GDD_Three_Act_Structure.md](./Narrative/GDD_Three_Act_Structure.md)** - Complete narrative arc
   - Deathbed framing device
   - Act 1: The Young Captain (setup, romance, foundation)
   - Act 2: The Shipping Magnate (growth, VIPs, family)
   - Act 3: The Patriarch/Matriarch (legacy, succession, endings)
   - Story beats timeline
   - Multiple ending variations
   - Historical event integration

6. **[GDD_STRUCTURE.md](GDD_STRUCTURE.md)** - Documentation organization guide
   - Directory structure layout
   - Document standards and templates
   - Cross-referencing guidelines
   - Maintenance procedures

---

## 📋 Required Documents (To Be Created)

The following documents are referenced but need detailed specifications:

### High Priority (Phase 1-2: Months 1-12)

**Ship Systems**:
- [ ] `GDD_Ship_Physics.md` - Detailed physics model, formulas, tuning
- [ ] `GDD_Grounding_System.md` - Depth detection, chart quality, grounding mechanics
- [ ] `GDD_Docking_System.md` - Manual/auto docking, skill progression
- [ ] `GDD_Capsizing_System.md` - Stability, weather impacts, failure states

**Tourism & Content**:
- [ ] `GDD_Interest_Points.md` - POI types, two-tier system, discovery
- [ ] `GDD_Tours_Design.md` - Route planning, optimization, profitability
- [ ] `GDD_Satisfaction_Mechanics.md` - Detailed satisfaction formula, factors
- [ ] `GDD_Prestige_System.md` - Reputation tiers, unlocks, progression

**Economy**:
- [ ] `GDD_Economy_Pricing.md` - Tour pricing model by class and quality
- [ ] `GDD_Economy_Costs.md` - Ship purchase, maintenance, wages, fuel
- [ ] `GDD_Economy_Resources.md` - Resource management, cash flow

**Planning & Discovery**:
- [ ] `GDD_Planning_Room.md` - HQ interface, map table, route design UI
- [ ] `GDD_Sea_Charts.md` - Chart quality tiers, discovery mechanics
- [ ] `GDD_Fog_Of_War.md` - Discovery system, reveal mechanics

**Ship Building**:
- [ ] `GDD_Modular_Ships.md` - Bow/Mid/Stern system, upgrades
- [ ] `GDD_Ship_Types.md` - Sailing/Steam/Hybrid, historical accuracy
- [ ] `GDD_Ship_Amenities.md` - Galley, bar, dining, observation deck

### Medium Priority (Phase 3-4: Months 13-24)

**Employee Management**:
- [ ] `GDD_Employee_Types.md` - Captain, Guide, Service, Engineer, Navigator
- [ ] `GDD_Employee_Hiring.md` - Hiring system, skills, traits
- [ ] `GDD_Employee_Skills.md` - XP progression, specializations
- [ ] `GDD_Delegation.md` - AI captain assignment, trust system

**Risk & Challenge**:
- [ ] `GDD_Weather.md` - Wind, waves, fog, visibility, dynamics
- [ ] `GDD_Seasons.md` - Four seasons, ice, economic impacts
- [ ] `GDD_Bankruptcy.md` - Debt mechanics, failure states
- [ ] `GDD_Rescue.md` - Rescue mechanics, reputation effects
- [ ] `GDD_Dynamic_Events.md` - Emergent challenges and opportunities

**Narrative & Characters**:
- [ ] `GDD_Framing_Device.md` - "Till havs", deathbed reflection, emotional design
- [ ] `GDD_Character_Design.md` - Wife, children, detailed character arcs
- [ ] `GDD_Historical_Context.md` - 1887 Sweden, accuracy standards

**Family & Succession**:
- [ ] `GDD_Wife_System.md` - Romance, co-narrator role, character depth
- [ ] `GDD_Children_System.md` - Birth, traits, personality, integration
- [ ] `GDD_Succession.md` - Heir selection, training, evaluation
- [ ] `GDD_Gender_Choice.md` - Grand-papa vs Grand-mama, difficulty

**Historical Integration**:
- [ ] `GDD_VIP_System.md` - Oscar II, Nobel, Strindberg, Lagerlöf
- [ ] `GDD_Historical_Events.md` - WWI, WWII, economic events, timeline
- [ ] `GDD_Newspaper_System.md` - Weekly editions, headlines, coverage
- [ ] `GDD_Historical_Accuracy.md` - Research standards, authenticity

**Progression**:
- [ ] `GDD_Act_Progression.md` - System unlocks across acts
- [ ] `GDD_Skill_Trees.md` - Captain and employee skill trees
- [ ] `GDD_Unlock_Systems.md` - Ships, islands, routes, VIPs
- [ ] `GDD_Economic_Progression.md` - Income growth, cost scaling

### Lower Priority (Phase 5+: Polish & Post-Launch)

**UI/UX**:
- [ ] `GDD_UIUX_Harbor.md` - Harbor scene design
- [ ] `GDD_UIUX_Planning.md` - Planning interface
- [ ] `GDD_UIUX_Voyage.md` - In-voyage HUD
- [ ] `GDD_UIUX_Menus.md` - Menu flows, information architecture

**Content**:
- [ ] `GDD_Content_Map.md` - Complete archipelago map, islands
- [ ] `GDD_Content_POIs.md` - Full POI database
- [ ] `GDD_Content_Characters.md` - All named characters
- [ ] `GDD_Content_Tours.md` - Example tour catalog

**Audio & Aesthetics**:
- [ ] `GDD_Audio_Music.md` - "Till havs" integration, period music
- [ ] `GDD_Audio_Sound.md` - Sound design specifications
- [ ] `GDD_Visual_Style.md` - Art direction, 1887 aesthetic

**Technical**:
- [ ] `GDD_Tech_Architecture.md` - System architecture, data-oriented design
- [ ] `GDD_Tech_Data_Structures.md` - Key data layouts
- [ ] `GDD_Tech_Performance.md` - Optimization strategies
- [ ] `GDD_Tech_Platforms.md` - Windows/Linux considerations

**Difficulty & Accessibility**:
- [ ] `GDD_Difficulty_Modes.md` - Easy/Normal/Hard differences
- [ ] `GDD_Assist_Options.md` - Accessibility features
- [ ] `GDD_Onboarding.md` - Tutorial design
- [ ] `GDD_Player_Guidance.md` - Learning systems

**Future Expansion**:
- [ ] `GDD_Future_Goods_DLC.md` - Cargo transport expansion
- [ ] `GDD_Future_International_DLC.md` - Beyond archipelago
- [ ] `GDD_Future_Post_Launch.md` - Post-launch content plans

---

## 🎯 Implementation Priorities

### Phase 1: Core Loop (Months 1-6)

**Goal**: Prove the core gameplay loop is fun and functional

**Required Documents**:
1. Core Gameplay Loop ✅
2. Ship Direct Control ✅
3. Ship Physics
4. Grounding System
5. Docking System
6. Economy Costs
7. Economy Pricing

**Prototype Deliverables**:
- Playable ship with good "feel"
- Basic route (3-5 waypoints)
- Grounding risk and depth system
- Simple profit/loss economy
- One island + harbor
- Manual docking
- 30-minute gameplay loop

**Success Criteria**:
- Piloting feels good (playtester feedback 7+/10)
- Route completion is satisfying
- Economic loop is clear
- Can play 30-min session start-to-finish

---

### Phase 2: Tourism & Content (Months 7-12)

**Goal**: Add depth through passengers, POIs, and discovery

**Required Documents**:
1. Passenger System ✅
2. Interest Points
3. Tours Design
4. Satisfaction Mechanics
5. Prestige System
6. Sea Charts
7. Fog of War
8. Weather
9. Content Map
10. Content POIs

**Feature Deliverables**:
- Full passenger system with 4 classes
- 20+ POIs across 3 types
- Satisfaction calculation and feedback
- Prestige tiers and unlocks
- Fog of war discovery
- Weather system (wind, waves)
- Full archipelago map (20+ islands)
- Route optimization challenge

**Success Criteria**:
- Passengers feel like people, not numbers
- Route design has strategic depth
- Discovery is engaging
- Weather creates meaningful challenge
- 2-3 hour gameplay sessions are engaging

---

### Phase 3: Management & Scale (Months 13-18)

**Goal**: Enable fleet operations and delegation

**Required Documents**:
1. Modular Ships
2. Ship Types
3. Ship Amenities
4. Employee Types
5. Employee Hiring
6. Employee Skills
7. Delegation
8. Seasons
9. Planning Room
10. Route Design (UI)

**Feature Deliverables**:
- Multi-ship fleet support (10+ ships)
- AI captain delegation
- Employee hiring and management
- Ship customization and upgrades
- Seasonal cycle
- Advanced planning tools
- Fleet management UI

**Success Criteria**:
- Managing 10+ ships feels good
- Delegation is reliable
- Employee system has depth
- Seasonal planning matters
- Scale doesn't sacrifice fun

---

### Phase 4: Narrative & Polish (Months 19-24)

**Goal**: Implement story and emotional beats

**Required Documents**:
1. Three-Act Structure ✅
2. Framing Device
3. Character Design
4. Wife System
5. Children System
6. Succession
7. VIP System
8. Historical Events
9. Newspaper System
10. Capsizing System
11. Bankruptcy
12. Historical Context

**Feature Deliverables**:
- Complete three-act implementation
- Wife romance and co-narrator
- Children system with succession
- VIP passengers and contracts
- Historical events (WWI, WWII)
- Newspaper weekly editions
- Multiple endings (5 types)
- Framing device and "Till havs"

**Success Criteria**:
- Story is emotionally engaging
- Characters are memorable
- Historical integration feels authentic
- Endings are satisfying and varied
- 20-30 hour complete playthrough

---

### Phase 5: Expansion & Launch (Months 25-30)

**Goal**: Polish, balance, and prepare for launch

**Required Documents**:
1. All UI/UX specs
2. Audio specs
3. Visual style guide
4. Difficulty modes
5. Assist options
6. Onboarding
7. Technical optimization
8. All remaining content

**Feature Deliverables**:
- Complete UI polish
- Full audio implementation
- Visual effects and polish
- Difficulty modes (Easy/Normal/Hard)
- Accessibility features
- Tutorial/onboarding
- Performance optimization (60 FPS target)
- Balance pass (all systems)
- Localization prep
- Bug fixing and QA

**Success Criteria**:
- Stable 60 FPS with 50+ ships
- Zero critical bugs
- All UI is polished and clear
- Tutorial is effective (80% completion)
- Balance feels right (playtesting)
- Ready to ship

---

## 📊 System Dependency Map

Understanding which systems depend on others for implementation:

### Core Dependencies (Build First)
```
Ship Direct Control
    ├─> Ship Physics
    ├─> Grounding System
    └─> Docking System

Economy Foundation
    ├─> Economy Costs
    ├─> Economy Pricing
    └─> Economy Resources
```

### Mid-Level Dependencies (Build Second)
```
Tourism Layer
    ├─> Passenger System ✅
    ├─> Interest Points
    ├─> Tours Design
    └─> Satisfaction Mechanics

Discovery Layer
    ├─> Sea Charts
    ├─> Fog of War
    ├─> Planning Room
    └─> Content Map

Ship Customization
    ├─> Modular Ships
    ├─> Ship Types
    └─> Ship Amenities
```

### High-Level Dependencies (Build Third)
```
Management Layer
    ├─> Employee System (Types, Hiring, Skills)
    ├─> Delegation
    └─> Fleet Management UI

Prestige & Progression
    ├─> Prestige System
    ├─> Unlock Systems
    ├─> Skill Trees
    └─> Act Progression

Risk & Challenge
    ├─> Weather
    ├─> Seasons
    ├─> Dynamic Events
    ├─> Capsizing
    └─> Bankruptcy
```

### Narrative Dependencies (Build Last)
```
Story Layer
    ├─> Three-Act Structure ✅
    ├─> Framing Device
    ├─> Character Design
    ├─> Family System (Wife, Children, Succession)
    ├─> VIP System
    ├─> Historical Events
    └─> Newspaper System
```

---

## 🔧 Technical Implementation Notes

### Data-Oriented Design Approach

**Key Systems for SoA (Struct-of-Arrays)**:
1. **Ship Movement** (50+ ships updated every frame)
2. **Passenger Groups** (hundreds of groups across fleet)
3. **Employee Data** (50+ employees with skills)
4. **POI Data** (100+ POIs with states)

**Example Structure**:
```cpp
// Ship Movement System (hot path)
struct ShipMovementData {
    float[] positions_x;
    float[] positions_y;
    float[] rotations;
    float[] velocities_x;
    float[] velocities_y;
    float[] throttles;
    // ... SIMD-friendly layout
};

// Process all ships in parallel
void UpdateShipMovement(ShipMovementData& data, float deltaTime) {
    // Batch process all positions, velocities
    // Vectorized operations where possible
}
```

**Performance Targets**:
- 60 FPS with 50 active ships
- < 5ms per frame for ship physics
- < 10ms per frame for all game logic
- Remaining time for rendering

*Details in: [Technical Architecture](./Technical/GDD_Tech_Architecture.md) (to be created)*

---

## 📝 Documentation Standards Reminder

### Every Design Document Should Include:

1. **Header Block**:
   ```markdown
   # [System/Feature Name]
   
   **Document Type:** [System Design / Feature Spec / Content Design]
   **Version:** X.X
   **Last Updated:** YYYY-MM-DD
   **Owner:** [Name/Role]
   **Related Documents:** [Links]
   ```

2. **Overview Section**: Brief description, purpose

3. **Design Intent**: Why this system exists, what it achieves

4. **Detailed Specifications**: The meat of the document
   - Mechanics, formulas, data structures
   - Player-facing experience
   - UI/UX considerations
   - Balance parameters

5. **Examples**: Concrete examples of system in action

6. **Edge Cases**: Unusual situations, how to handle

7. **Integration Notes**: How this connects to other systems

8. **Success Metrics**: How to evaluate if system works

9. **Design Notes**: Callouts with design philosophy

10. **Implementation Priorities**: What to build first

### Cross-Referencing
- Use relative links: `[Link Text](GDD_Other_Document.md)`
- Link to main GDD: `[Main GDD](GDD_MASTER.md)`
- Bidirectional links when systems interact

### Formatting
- Use tables for comparisons and data
- Use code blocks for formulas and algorithms
- Use callout quotes (`>`) for design notes
- Use clear headers (##, ###) for structure

---

## 🎨 Design Philosophy Quick Reference

### From Jesse Schell's "The Art of Game Design"
- **Lens of Essential Experience**: What must the player feel?
  - MS Tour: Mastery of piloting + Pride in building empire + Legacy satisfaction
- **Lens of Challenge**: Is the challenge tuned correctly?
  - MS Tour: Act 1 easy, Act 2 moderate, Act 3 hard (with spikes)
- **Lens of Skill**: Can players express skill?
  - MS Tour: Piloting skill, strategic optimization, family relationship management

### From Raph Koster's "A Theory of Fun"
- **Fun is learning**: Systems teach clear patterns
  - MS Tour: Grounding teaches depth awareness, passengers teach preferences
- **Mastery is satisfying**: Visible progression
  - MS Tour: Grounding rate decreases, docking time improves, fleet scales
- **Grok**: Players should "get it" through play
  - MS Tour: Don't over-explain, let players discover through feedback

### From Ernest Adams' "Fundamentals of Game Design"
- **Player-centric**: Features serve player experience
  - MS Tour: Every system asks "Does this make the player feel like a captain?"
- **Clear mechanics**: Rules are understandable
  - MS Tour: Satisfaction formula is complex but factors are clear
- **Explicit balance**: Tuning values documented
  - MS Tour: All formulas have tunable parameters in configs

---

## ✅ Review Checklist for New Documents

Before marking a design document as complete:

- [ ] Includes all required header information
- [ ] Has clear overview and design intent
- [ ] Specifies player-facing experience
- [ ] Documents all mechanics and formulas
- [ ] Includes concrete examples
- [ ] Addresses edge cases
- [ ] Explains integration with other systems
- [ ] Defines success metrics
- [ ] Has design philosophy notes
- [ ] Specifies implementation priorities
- [ ] Follows formatting standards
- [ ] Cross-references related documents
- [ ] Is implementation-ready (developers can code from it)

---

## 🚀 Next Steps

1. **Review** this quick reference and the completed core documents
2. **Prioritize** which missing documents are needed for Phase 1
3. **Create** documents in dependency order (core systems first)
4. **Validate** through prototyping (build what's documented)
5. **Iterate** based on playtesting (update docs as design evolves)
6. **Maintain** regularly (keep docs in sync with implementation)

---

## 📞 Document Owners & Contacts

| Area | Owner Role | Responsible For |
|------|-----------|-----------------|
| **Core Gameplay** | Lead Designer | Loops, pillars, vision |
| **Ship Systems** | Systems Designer | Physics, control, mechanics |
| **Tourism & Economy** | Systems Designer | Passengers, tours, pricing |
| **Narrative** | Narrative Designer | Story, characters, dialogue |
| **Progression** | Systems Designer | Unlocks, skills, balance |
| **Technical** | Technical Director | Architecture, performance |
| **UI/UX** | UX Designer | Interfaces, information architecture |
| **Content** | Content Designer | Islands, POIs, tours catalog |
| **Audio** | Audio Director | Music, sound, "Till havs" |

*(Assign actual names as team forms)*

---

## 📚 Additional Resources

### Reference Materials
- **PROJECT_CONTEXT.md**: Technical architecture and engine details
- **Existing GDD_MS_TOUR.md**: Original design document (to be integrated)
- **Mind maps**: Passenger system and Tours system visual references

### External References
- Jesse Schell: "The Art of Game Design: A Book of Lenses"
- Raph Koster: "A Theory of Fun for Game Design"
- Ernest Adams: "Fundamentals of Game Design"
- Historical references: 1887 Sweden, Gothenburg archipelago

---

**Document End**

*This quick reference should be updated as new documents are created and implementation progresses. Version this document alongside the main GDD.*

**Current Status**: 6 of 60+ documents completed (10%)  
**Priority**: Create Phase 1 documents (Ship Physics, Grounding, Economy) next  
**Timeline**: Aim for 2-3 documents per week to stay on track

*"Till havs, till havs..."*
