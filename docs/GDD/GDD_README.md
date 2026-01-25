# MS TOUR - Game Design Documentation

## Welcome to the MS Tour GDD

This directory contains the comprehensive, modular Game Design Document (GDD) for **MS Tour**, a shipping company management simulation set in the Gothenburg archipelago (1887-1950s).

---

## 📖 Getting Started

### For New Team Members

**Start Here**:
1. Read **[GDD_MASTER.md](GDD_MASTER.md)** first - the master overview document
2. Review **[GDD_QUICK_REFERENCE.md](GDD_QUICK_REFERENCE.md)** - implementation guide and document status
3. Check **[GDD_STRUCTURE.md](GDD_STRUCTURE.md)** - documentation organization standards

**Then Dive Deeper**:
- For gameplay understanding: [Core Gameplay Loop](GDD_Core_Gameplay_Loop.md)
- For narrative understanding: [Three-Act Structure](GDD_Three_Act_Structure.md)
- For technical understanding: [Ship Direct Control](ShipControl/GDD_Ship_Direct_Control.md)
- For systems understanding: [Passenger System](Tourism/GDD_Passenger_System.md)

### For Developers

**Implementation Path**:
1. Read [GDD_QUICK_REFERENCE.md](GDD_QUICK_REFERENCE.md) for phase priorities
2. Check system dependency map (in Quick Reference)
3. Read relevant system documents in dependency order
4. Cross-reference technical documents (when created)

### For Designers

**Design Work**:
1. Review [GDD_MASTER.md](GDD_MASTER.md) for vision and pillars
2. Check [GDD_QUICK_REFERENCE.md](GDD_QUICK_REFERENCE.md) for missing documents
3. Use document templates from [GDD_STRUCTURE.md](GDD_STRUCTURE.md)
4. Follow design philosophy (Schell, Koster, Adams principles)

---

## 📁 Documentation Structure

### Modular Approach

This GDD uses a **modular structure** instead of one massive file:

**Benefits**:
- ✅ Easier to maintain (update individual documents)
- ✅ Parallel work (multiple designers can work simultaneously)
- ✅ Faster navigation (find relevant info quickly)
- ✅ Version control friendly (smaller diffs)
- ✅ Implementation-focused (documents align with system development)

### Current Documents

**Core Documents** ✅ (Complete):
- `GDD_MASTER.md` - Master overview, vision, pillars, roadmap
- `GDD_Core_Gameplay_Loop.md` - Gameplay loops at all scales
- `GDD_Ship_Direct_Control.md` - Ship piloting system
- `GDD_Passenger_System.md` - Passenger classes, traits, satisfaction
- `GDD_Three_Act_Structure.md` - Complete narrative arc
- `GDD_STRUCTURE.md` - Documentation standards
- `GDD_QUICK_REFERENCE.md` - Implementation guide and status

**System Documents** 📝 (To Be Created):
See [GDD_QUICK_REFERENCE.md](GDD_QUICK_REFERENCE.md) for complete list of planned documents organized by priority.

---

## 🎯 Design Vision

### One-Sentence Pitch
*"Build a shipping empire in the Gothenburg archipelago, mastering both the helm and the ledger, from ambitious young captain to patriarch leaving a legacy."*

### Core Experience
MS Tour combines:
- **Hands-On Piloting**: Direct ship control with skill expression
- **Strategic Management**: Route planning, fleet building, employee hiring
- **Emotional Storytelling**: Life journey spanning 60 years with family and legacy
- **Historical Integration**: Real VIPs, events from 1887-1950s

### Four Design Pillars

1. **Strategic Optimization**: Multiple paths to success, meaningful choices
2. **Progressive Complexity**: Systems layer gradually across three acts
3. **Environmental Interaction**: Archipelago is alive with weather, seasons, hazards
4. **Business Management with Heart**: People, not just numbers

*Full details in [GDD_MASTER.md](GDD_MASTER.md)*

---

## 🎮 Core Gameplay

### The Three Loops

**Minute-to-Minute** (5-15 minutes):
- Pilot ship through route
- Navigate hazards (grounding, weather)
- Visit Points of Interest (POIs)
- Monitor passenger satisfaction
- Dock at harbor

**Session-to-Session** (60-90 minutes):
- Review voyage results
- Plan routes in Planning Room
- Purchase ships and upgrades
- Hire and assign employees
- Execute 3-5 voyages
- Experience story moments

**Act-to-Act** (20-30 hours total):
- **Act 1**: Young Captain - Learn piloting, court wife, start family (5-8 hours)
- **Act 2**: Shipping Magnate - Build empire, raise family, VIP contracts (8-12 hours)
- **Act 3**: Patriarch/Matriarch - Legacy, succession, historical challenges (7-10 hours)

*Full details in [GDD_Core_Gameplay_Loop.md](GDD_Core_Gameplay_Loop.md) and [GDD_Three_Act_Structure.md](GDD_Three_Act_Structure.md)*

---

## 🚢 Key Systems Overview

### Ship Control
- WASD/Arrow key piloting
- Arcade-style physics (accessible but skill-expressive)
- Grounding risk (depth, chart quality)
- Docking (manual → auto-unlock)
- Weather effects (wind, waves, fog)

*Details: [GDD_Ship_Direct_Control.md](ShipControl/GDD_Ship_Direct_Control.md)*

### Passengers
- **Four Classes**: Knegare (working), Borgare (middle), Adel (upper), VIP (royalty/celebrities)
- **30+ Traits**: Easily Pleased, Demanding, Seasick, History Buff, etc.
- **Preferences**: Different classes prefer different POI types and services
- **Satisfaction System**: Real-time feedback, tips, reputation impact

*Details: [GDD_Passenger_System.md](Tourism/GDD_Passenger_System.md)*

### Tourism
- **Interest Points**: Nature, Historical, Military POIs (100+ across archipelago)
- **Route Design**: Plan circuits with multiple POIs
- **Discovery**: Fog of war, chart quality, famous landmarks
- **Prestige**: Reputation tiers unlock VIPs, areas, features

*Details: [Tours Design], [Interest Points], [Prestige System] (to be created)*

### Fleet Management
- **Multi-Ship Operations**: Scale from 1 to 20+ ships
- **Ship Types**: Sailing, Steam, Hybrid (historical progression)
- **Customization**: Modular upgrades, amenities, paint schemes
- **Delegation**: Hire AI captains to pilot ships

*Details: [Modular Ships], [Ship Types], [Delegation] (to be created)*

### Family & Narrative
- **Wife**: Romance in Act 1, co-narrator throughout
- **Children**: 2-4 children with unique personalities and traits
- **Succession**: Choose and train heir in Act 3
- **Historical Events**: WWI, WWII, economic crashes
- **VIPs**: Oscar II, Alfred Nobel, August Strindberg, Selma Lagerlöf

*Details: [GDD_Three_Act_Structure.md](GDD_Three_Act_Structure.md), [Character Design], [VIP System] (to be created)*

---

## 🎨 Design Philosophy

### Grounded in Theory

MS Tour's design is informed by established game design principles:

**Jesse Schell** ("The Art of Game Design"):
- Design the complete player experience, not just mechanics
- Use design lenses: Essential Experience, Challenge, Skill, Time
- Every system serves the player fantasy

**Raph Koster** ("A Theory of Fun"):
- Fun is learning; systems teach clear patterns
- Mastery curves are visible and rewarding
- Complexity increases as competence grows

**Ernest Adams** ("Fundamentals of Game Design"):
- Player-centric design in all decisions
- Clear mechanics with understandable trade-offs
- Explicit balance parameters (tunable, documented)

### Player Fantasy

**"I am a shipping captain who built an empire with my own hands and lived a full life."**

Players should feel:
- **Mastery** of ship piloting
- **Pride** in building a business
- **Connection** to family and history
- **Nostalgia** through deathbed framing
- **Accomplishment** in leaving a legacy

---

## ⚙️ Technical Overview

### Performance Targets
- **60 FPS** with 50+ active ships, 20+ islands
- **< 5 seconds** load time between scenes
- **Data-Oriented Design** for hot paths (ship movement, passenger AI)

### Platform
- **Windows** (primary)
- **Linux** (full support)
- **Input**: Keyboard + Mouse (gamepad post-launch)

### Architecture
- **Orthographic View**: 3D ships on pre-rendered 2D backgrounds
- **Struct-of-Arrays (SoA)**: Efficient data layouts for batch processing
- **Hybrid Rendering**: 3D dynamic elements, 2D static world

*Full technical specs in [Technical Architecture] (to be created)*

---

## 📅 Development Roadmap

### Phase 1: Core Loop (Months 1-6)
**Goal**: Prove core gameplay is fun
- Ship control + physics
- Grounding system
- Simple routes, basic economy
- One island + harbor
- 30-minute playable loop

### Phase 2: Tourism & Content (Months 7-12)
**Goal**: Add depth and variety
- Passenger system (4 classes)
- 20+ POIs across archipelago
- Satisfaction mechanics
- Prestige system
- Weather and discovery

### Phase 3: Management & Scale (Months 13-18)
**Goal**: Enable fleet operations
- Multi-ship fleet (10-20 ships)
- Employee hiring and delegation
- Ship customization
- Seasonal cycle
- Advanced planning tools

### Phase 4: Narrative & Polish (Months 19-24)
**Goal**: Implement story
- Three-act structure
- Family system (wife, children)
- Succession mechanics
- VIP passengers
- Historical events
- Multiple endings

### Phase 5: Expansion & Launch (Months 25-30)
**Goal**: Polish and release
- Final balance pass
- UI/UX polish
- Audio implementation
- Accessibility features
- Performance optimization
- Launch preparation

*Detailed roadmap in [GDD_MASTER.md](GDD_MASTER.md)*

---

## 📊 Success Metrics

### Player Experience
- **80%+** complete Act 1 (tutorial effectiveness)
- **50%+** reach Act 3 (engagement retention)
- **40%+** finish the game (narrative payoff)
- **60%+** report emotional connection (story effectiveness)

### Technical
- **95%+** gameplay at 60 FPS
- **< 0.1%** crash rate per hour
- **Cross-platform** parity (Windows/Linux)

### Design
- **3+** viable strategies (volume, premium, balanced)
- **70%+** remember core controls after 1 hour
- **Multiple** meaningful endings

*Full metrics in [GDD_MASTER.md](GDD_MASTER.md)*

---

## 🔄 Document Maintenance

### Version Control
- Each document has version number and last updated date
- Major changes increment version
- Document history tracked in version control (Git)

### Review Schedule
- **Monthly**: Review all core documents
- **As Needed**: Update when design decisions are made
- **Pre-Implementation**: Review relevant docs before coding

### Contribution Guidelines
1. Read existing documents first (avoid duplication)
2. Follow templates from [GDD_STRUCTURE.md](GDD_STRUCTURE.md)
3. Include all required sections (overview, intent, specs, etc.)
4. Cross-reference related documents
5. Use clear, implementable language
6. Include examples and edge cases
7. Define success metrics

---

## 📞 Getting Help

### Finding Information

**"Where do I find...?"**
- Overall vision → [GDD_MASTER.md](GDD_MASTER.md)
- Implementation priorities → [GDD_QUICK_REFERENCE.md](GDD_QUICK_REFERENCE.md)
- Gameplay loops → [GDD_Core_Gameplay_Loop.md](GDD_Core_Gameplay_Loop.md)
- Ship piloting → [GDD_Ship_Direct_Control.md](ShipControl/GDD_Ship_Direct_Control.md)
- Passenger behavior → [GDD_Passenger_System.md](Tourism/GDD_Passenger_System.md)
- Story structure → [GDD_Three_Act_Structure.md](GDD_Three_Act_Structure.md)
- Documentation standards → [GDD_STRUCTURE.md](GDD_STRUCTURE.md)

**"What should I build next?"**
- Check [GDD_QUICK_REFERENCE.md](GDD_QUICK_REFERENCE.md) for phase priorities
- Follow dependency map (build foundational systems first)
- Align with current development phase

**"How do I write a new design document?"**
1. Check [GDD_STRUCTURE.md](GDD_STRUCTURE.md) for templates
2. Review existing documents for style examples
3. Follow formatting standards (headers, tables, cross-references)
4. Include all required sections
5. Make it implementation-ready (developers can code from it)

### Document Owners

See [GDD_QUICK_REFERENCE.md](GDD_QUICK_REFERENCE.md) for area owners and responsibilities.

---

## 🎵 Emotional Core

### "Till havs" (To the Sea)

The Swedish folk song "Till havs" is the emotional anchor of MS Tour:
- Plays during opening deathbed scene
- Recurring theme throughout game
- Full version during ending/credits
- Represents life journey, the sea, legacy

**Lyrics** (excerpt):
```
Till havs, till havs, till livets kamp och fara,
Till ärans prål, till guld och gyllne skatt!
```

**Translation**:
```
To the sea, to the sea, to life's struggle and danger,
To honor's splendor, to gold and golden treasure!
```

*Full emotional design in [Framing Device] (to be created)*

---

## 🌊 The Archipelago

### Setting: Gothenburg, Sweden

**Historical Period**: 1887-1950s
- Late Victorian era → Modern era
- Industrialization → Steam age
- Two World Wars
- Swedish Golden Age of culture

**Geography**: 
- Gothenburg harbor as main base
- 20+ islands in archipelago
- Nature POIs: Lighthouses, fjords, wildlife
- Historical POIs: Fortresses, churches, estates
- Military POIs: Naval bases, fortifications

*Full content in [Archipelago Map], [Historical Context] (to be created)*

---

## 🚀 Next Steps

### For the Team

1. **Review** core documents (Master, Core Loop, Three Acts)
2. **Prioritize** Phase 1 missing documents
3. **Create** documents in dependency order
4. **Prototype** based on specifications
5. **Playtest** and iterate
6. **Update** docs as design evolves

### For Management

1. **Approve** core vision and scope (GDD Master)
2. **Allocate** resources per phase roadmap
3. **Track** progress against milestones
4. **Review** prototypes at phase gates
5. **Adjust** timeline as needed

### For Partners

1. **Understand** core vision and player fantasy
2. **Review** relevant documents (audio, visual, narrative)
3. **Align** deliverables with specifications
4. **Collaborate** on integration and polish

---

## 📚 Additional Resources

### In This Repository
- **PROJECT_CONTEXT.md**: Technical architecture details
- **Original GDD_MS_TOUR.md**: Earlier design document (being integrated)
- **Mind maps**: Visual references for passengers and tours

### External References
- Game design books (Schell, Koster, Adams)
- Historical resources (1887 Sweden, Gothenburg)
- Nautical references (ship handling, archipelago navigation)

---

## ✨ Final Note

MS Tour is a game about **life, legacy, and the sea**. Every design decision should serve that vision. When in doubt, ask:

1. **Does this serve the player fantasy?** (Captain who built an empire)
2. **Does it align with our pillars?** (Optimization, Complexity, Environment, Heart)
3. **Is it implementable?** (Within technical constraints)
4. **Does it create meaningful choices?** (Multiple valid strategies)
5. **Does it teach clearly?** (Players can learn and master)

If yes to all five, it belongs in MS Tour.

---

**The sea awaits. Let's build something memorable.**

*"Till havs, till havs, till livets kamp och fara..."*

---

**Document Version**: 1.0  
**Last Updated**: 2025-01-01  
**Status**: Active Development  
**Completeness**: ~10% (6 of 60+ documents complete)

For questions or suggestions, contact the Lead Designer or open a discussion in the project management system.
