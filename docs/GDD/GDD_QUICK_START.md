# MS TOUR GDD - Quick Start by Role

**Choose your role below for a tailored onboarding path**

---

## 🎯 I'm a Developer

### Your Path: Code from Specs

**Start Here** (30 minutes):
1. Read [GDD_VISUAL_OVERVIEW.md](GDD_VISUAL_OVERVIEW.md) - Visual summary
2. Skim [GDD_MASTER.md](GDD_MASTER.md) - Section: Technical Requirements Summary
3. Read [GDD_QUICK_REFERENCE.md](GDD_QUICK_REFERENCE.md) - Section: Implementation Priorities

**Deep Dive** (2 hours):
4. Read [GDD_Core_Gameplay_Loop.md](./CoreGameplay/GDD_Core_Gameplay_Loop.md) - Understand what you're building
5. Read [GDD_Ship_Direct_Control.md](./ShipControl/GDD_Ship_Direct_Control.md) - Most complex system, fully specified

**Implementation Focus**:
- **Week 1-2**: Implement ship control from [GDD_Ship_Direct_Control.md](./ShipControl/GDD_Ship_Direct_Control.md)
  - Control scheme is documented (WASD, etc.)
  - Movement formulas are provided (pseudo-code)
  - Tuning parameters are listed
  - Data-oriented design approach suggested

- **Week 3-4**: Wait for Physics and Grounding documents (to be created)
  - Or prototype arcade physics based on existing doc
  - Focus on "feel" - iterate with designers

**Your Documents to Create**:
- None - focus on implementation
- But provide feedback on technical feasibility

**Questions? Check**:
- [GDD_QUICK_REFERENCE.md](GDD_QUICK_REFERENCE.md) for system dependencies
- [GDD_INDEX.md](GDD_INDEX.md) to find specific topics
- Slack #game-dev for clarifications

---

## 🎨 I'm a Designer

### Your Path: Create Missing Specs

**Start Here** (1 hour):
1. Read [GDD_README.md](GDD_README.md) - How this GDD works
2. Read [GDD_MASTER.md](GDD_MASTER.md) - Complete overview
3. Read [GDD_STRUCTURE.md](GDD_STRUCTURE.md) - Documentation standards

**Understand the Vision** (2 hours):
4. Read [GDD_Three_Act_Structure.md](./Narrative/GDD_Three_Act_Structure.md) - Narrative core
5. Read [GDD_Core_Gameplay_Loop.md](./CoreGameplay/GDD_Core_Gameplay_Loop.md) - Gameplay core
6. Read [GDD_Passenger_System.md](./Tourism/GDD_Passenger_System.md) - Example of depth

**Your Mission**:
Create the **7 Phase 1 Priority documents** (see [GDD_QUICK_REFERENCE.md](GDD_QUICK_REFERENCE.md)):

**Week 1-2**:
1. `GDD_Ship_Physics.md` - Expand on [GDD_Ship_Direct_Control.md](./ShipControl/GDD_Ship_Direct_Control.md), add detailed formulas
2. `GDD_Grounding_System.md` - Depth mechanics, chart quality, consequences
3. `GDD_Docking_System.md` - Expand docking section, add XP progression

**Week 3-4**:
4. `GDD_Economy_Pricing.md` - How tour prices work, by class and quality
5. `GDD_Economy_Costs.md` - All costs (ships, fuel, wages, repairs)
6. `GDD_Economy_Resources.md` - Resource management, cash flow

**Document Template** (use this structure):
```markdown
# [System Name]

**Document Type:** System Design
**Version:** 1.0
**Last Updated:** [Date]
**Owner:** [Your Name]
**Related Documents:** [Links]

## Overview
[Brief description]

## Design Intent
[Why this system exists, what it achieves]

## Player-Facing Experience
[What the player sees and does]

## Core Mechanics
[How it works - formulas, rules]

## Balance Parameters
[Tuning values]

## Implementation Notes
[For developers]

## Success Metrics
[How to evaluate success]
```

**Tools**:
- See [GDD_STRUCTURE.md](GDD_STRUCTURE.md) for full template
- Reference existing docs for style
- Use Markdown formatting

**Questions? Check**:
- [GDD_INDEX.md](GDD_INDEX.md) for where info might already exist
- Existing documents for cross-references
- Slack #game-design for discussions

---

## 📖 I'm a Writer/Narrative Designer

### Your Path: Expand the Story

**Start Here** (1 hour):
1. Read [GDD_THREE_ACT_STRUCTURE.md](./Narrative/GDD_Three_Act_Structure.md) - Complete narrative arc (27k chars!)
2. Read [GDD_MASTER.md](GDD_MASTER.md) - Section: Player Fantasy
3. Read [GDD_VISUAL_OVERVIEW.md](GDD_VISUAL_OVERVIEW.md) - Section: Emotional Core

**Understand Integration** (1 hour):
4. Read [GDD_Core_Gameplay_Loop.md](./CoreGameplay/GDD_Core_Gameplay_Loop.md) - How story fits gameplay
5. Read [GDD_Passenger_System.md](./Tourism/GDD_Passenger_System.md) - Character depth example

**Your Mission**:
Create the **narrative detail documents** (Phase 4, but can start early):

**Priority Documents**:
1. `GDD_Framing_Device.md` - "Till havs" integration, deathbed scenes
2. `GDD_Character_Design.md` - Wife, children, VIPs detailed
3. `GDD_Wife_System.md` - Romance arc, co-narrator role
4. `GDD_Children_System.md` - Birth, traits, development
5. `GDD_VIP_System.md` - Oscar II, Nobel, Strindberg, Lagerlöf
6. `GDD_Historical_Context.md` - 1887 Sweden research

**Your Deliverables**:
- Dialogue trees for key story beats
- Character backstories and arcs
- Historical accuracy research
- VIP personality profiles
- Newspaper headline samples

**Key Considerations**:
- Story moments happen **between voyages**, not during
- Wife is **co-narrator** - she comments throughout
- Children have **randomized traits** - write for variation
- Historical figures are **authentic** - research their lives
- Endings **vary** - 5 categories, multiple variations

**Resources**:
- 1887 Sweden historical records
- Swedish folk music research ("Till havs")
- Biographies of VIPs (Oscar II, Nobel, etc.)
- Gothenburg archipelago geography

---

## 🎬 I'm a Producer/Project Manager

### Your Path: Manage Development

**Start Here** (30 minutes):
1. Read [GDD_MASTER.md](GDD_MASTER.md) - Sections: Executive Summary, Development Roadmap
2. Read [GDD_QUICK_REFERENCE.md](GDD_QUICK_REFERENCE.md) - Complete implementation guide
3. Read [GDD_VISUAL_OVERVIEW.md](GDD_VISUAL_OVERVIEW.md) - Quick visual reference

**Understand Scope** (1 hour):
4. Review [GDD_MASTER.md](GDD_MASTER.md) - Section: Quick Reference (system summary)
5. Review [GDD_INDEX.md](GDD_INDEX.md) - All 60+ documents planned
6. Review completion percentages in [GDD_QUICK_REFERENCE.md](GDD_QUICK_REFERENCE.md)

**Your Tracking Tools**:
- **[GDD_QUICK_REFERENCE.md](GDD_QUICK_REFERENCE.md)** - Phase deliverables and priorities
- **[GDD_INDEX.md](GDD_INDEX.md)** - Document completion tracking (currently 15%)
- **[GDD_CREATION_SUMMARY.md](GDD_CREATION_SUMMARY.md)** - What's been done

**Key Milestones** (from [GDD_MASTER.md](GDD_MASTER.md)):

| Phase | Duration | Goal | Key Deliverable |
|-------|----------|------|-----------------|
| **Phase 1** | Months 1-6 | Core loop | 30-min playable prototype |
| **Phase 2** | Months 7-12 | Tourism depth | 2-3 hour engaging sessions |
| **Phase 3** | Months 13-18 | Fleet scale | Managing 10+ ships |
| **Phase 4** | Months 19-24 | Narrative | 20-30 hour playthrough |
| **Phase 5** | Months 25-30 | Polish | Shippable product |

**Resource Allocation**:
- **Phase 1**: 2 developers, 1 designer, prototyping
- **Phase 2**: 3 developers, 2 designers, 1 content designer
- **Phase 3**: 4 developers, 2 designers, 1 systems designer
- **Phase 4**: Full team + narrative designer + audio
- **Phase 5**: Full team + QA

**Risk Areas**:
1. **Ship control feel** - Must nail this in Phase 1 (iterative)
2. **Performance** - 50+ ships at 60 FPS (data-oriented design critical)
3. **Narrative integration** - Story between voyages, not during (design discipline)
4. **Scope creep** - 60+ documents, must prioritize ruthlessly

**Success Metrics** (from [GDD_MASTER.md](GDD_MASTER.md)):
- 50%+ players reach Act 3
- 40%+ complete the game
- 60 FPS stable
- 60%+ emotional connection

**Budget Estimates**:
- See technical requirements in [GDD_MASTER.md](GDD_MASTER.md)
- 30-month timeline = ~2.5 year dev cycle
- Team size: 8-12 people (varies by phase)

---

## 🎨 I'm an Artist/Audio Designer

### Your Path: Visual and Audio Style

**Start Here** (30 minutes):
1. Read [GDD_VISUAL_OVERVIEW.md](GDD_VISUAL_OVERVIEW.md) - Quick visual summary
2. Read [GDD_MASTER.md](GDD_MASTER.md) - Section: Technical Requirements Summary (visual style)
3. Read [GDD_Three_Act_Structure.md](GDD_Three_Act_Structure.md) - Section: Emotional Core

**Understand Visual Needs** (1 hour):
4. Review [GDD_Ship_Direct_Control.md](./ShipControl/GDD_Ship_Direct_Control.md) - Section: Camera System
5. Review [GDD_Passenger_System.md](./Tourism/GDD_Passenger_System.md) - Section: Passenger Behavior (animations)

**Art Direction** (from [GDD_MASTER.md](GDD_MASTER.md)):
- **Orthographic view**: 30-45° angle, top-down
- **3D ships**: Fully modeled, animated, customizable
- **2D backgrounds**: Pre-rendered islands, water, terrain
- **Historical aesthetic**: 1887 Sweden, authentic period details
- **Emotional tone**: Nostalgic, warm, bittersweet

**Asset Needs** (Phase by Phase):
- **Phase 1**: 1 ship model, 1 island, harbor, water
- **Phase 2**: 5-8 ship types, 20+ islands, weather effects
- **Phase 3**: Ship customization (paint, flags), 10+ amenities
- **Phase 4**: Character models (wife, children, VIPs), cutscene art
- **Phase 5**: Polish, effects, UI art

**Audio Direction**:
- **"Till havs" (To the Sea)**: Swedish folk song, emotional anchor
  - Opening: Piano version
  - Throughout: Recurring motif
  - Ending: Full orchestral with vocals
- **Period music**: 1887-1950s Swedish folk and classical
- **Sound design**: Ships, water, weather, passengers, UI
  - Reference: [GDD_Ship_Direct_Control.md](./ShipControl/GDD_Ship_Direct_Control.md) - Section: Audio Feedback

**Future Documents to Create**:
- `GDD_Visual_Style.md` - Complete art direction (Phase 5)
- `GDD_Audio_Music.md` - Music design and "Till havs" integration (Phase 4)
- `GDD_Audio_Sound.md` - Sound design specifications (Phase 4)

**References**:
- Historical photos of 1887 Gothenburg
- Swedish archipelago references
- Victorian-era ship designs (sailing + early steam)
- Swedish folk music archives

---

## 💼 I'm an Executive/Stakeholder

### Your Path: Understand the Investment

**Start Here** (15 minutes):
1. Read [GDD_MASTER.md](GDD_MASTER.md) - Sections: Executive Summary, Vision Statement
2. Read [GDD_VISUAL_OVERVIEW.md](GDD_VISUAL_OVERVIEW.md) - One-page overview
3. Read [GDD_CREATION_SUMMARY.md](GDD_CREATION_SUMMARY.md) - What's been done

**The Pitch** (5 minutes):
- **Genre**: Shipping company management simulation
- **Unique Hook**: Hands-on piloting + strategic management + 60-year life story
- **Setting**: Gothenburg archipelago, Sweden (1887-1950s)
- **Target**: Strategy/sim enthusiasts, history buffs, narrative-driven players
- **Platform**: Windows, Linux (PC focus)
- **Timeline**: 30 months development
- **Scope**: 20-30 hour game experience

**Key Differentiators**:
1. **Skill + Strategy Hybrid**: Unlike pure management sims, direct ship control matters
2. **Complete Life Narrative**: Not just a business story - romance, family, legacy
3. **Historical Authenticity**: Real Sweden, real VIPs (King Oscar II, Nobel)
4. **Emotional Resonance**: "Till havs" folk song + deathbed framing creates nostalgia

**Market Position**:
- **Similar to**: Anno series (building), Patrician (trading), Stardew Valley (life sim)
- **Different from**: More narrative, more skill-based, more emotional
- **Target Audience**: 25-45 year old strategy/sim players, 200k-500k units potential

**Investment Overview**:
- **Development**: 30 months (5 phases)
- **Team**: 8-12 people (varies by phase)
- **Milestones**: Prototype (Month 6), Alpha (Month 12), Beta (Month 24), Launch (Month 30)
- **Budget**: See detailed breakdown (separate business document)

**Success Metrics**:
- **Player Retention**: 50%+ reach Act 3, 40%+ complete game
- **Technical**: 60 FPS stable, < 0.1% crash rate
- **Commercial**: 50,000 units Year 1, 80+ Metacritic/Steam score

**Risks**:
1. **Niche Genre**: Management sims have dedicated but smaller audience
2. **Narrative Integration**: Story + gameplay balance is challenging
3. **Performance**: 50+ ships at 60 FPS requires data-oriented design
4. **Scope**: 60+ design documents, must control scope creep

**Opportunities**:
1. **Underserved Market**: Few nautical management games
2. **Emotional Hook**: "Till havs" and life story differentiate
3. **Historical Appeal**: Educational + immersive
4. **DLC Potential**: Goods transport, international expansion

**Current Status**:
- **GDD**: 15% complete (9 of 60+ docs), foundation solid
- **Prototype**: Ready to start (Phase 1 specs complete)
- **Team**: Ready to scale

**Next Steps**:
1. Approve GDD foundation
2. Greenlight Phase 1 prototype
3. Allocate Phase 1 resources (6 months)
4. Review prototype at Month 6 milestone

---

## 🧪 I'm a QA Tester

### Your Path: Test Against Specs

**Start Here** (1 hour):
1. Read [GDD_MASTER.md](GDD_MASTER.md) - Section: Success Metrics
2. Read [GDD_Core_Gameplay_Loop.md](./CoreGameplay/GDD_Core_Gameplay_Loop.md) - Understand complete loops
3. Read [GDD_Ship_Direct_Control.md](./ShipControl/GDD_Ship_Direct_Control.md) - Most testable system

**Your Testing Framework**:

**Phase 1 Testing** (Prototype):
- Ship control responsiveness (7+/10 feel rating)
- Grounding detection accuracy
- Docking precision requirements
- 30-minute loop completable
- No critical bugs

**Phase 2 Testing** (Content):
- Passenger satisfaction formula accuracy
- POI trigger zones work correctly
- Weather effects as specified
- Fog of war reveal correct
- 2-3 hour sessions engaging

**Phase 3 Testing** (Scale):
- 10+ ships stable at 60 FPS
- AI captain pathfinding works
- No ship collision bugs
- Employee XP tracking correct
- Fleet management UI usable

**Phase 4 Testing** (Narrative):
- Story beats trigger correctly
- Family system saves/loads
- Multiple endings achievable
- VIP contracts work
- 20-30 hour playthrough stable

**Phase 5 Testing** (Polish):
- All UI polished and clear
- No graphical glitches
- Audio plays correctly
- Difficulty modes balanced
- Cross-platform parity

**Test Cases** (derive from specs):
- Every document has "Success Metrics" section
- Use these as acceptance criteria
- Example from [GDD_Ship_Direct_Control.md](./ShipControl/GDD_Ship_Direct_Control.md):
  - "90%+ can pilot without grounding within 15 minutes"
  - "Median docking time < 10 seconds by mid Act 1"

**Bug Severity** (by system criticality):
- **Critical**: Ship control, saving, performance, crashes
- **High**: Passenger satisfaction, economy, progression
- **Medium**: UI polish, audio, visual effects
- **Low**: Minor text, rare edge cases

---

## 🎓 I'm New to the Project

### Your Path: Understand MS Tour

**5-Minute Overview**:
1. Read [GDD_VISUAL_OVERVIEW.md](GDD_VISUAL_OVERVIEW.md) - Visual summary with ASCII art

**30-Minute Deep Dive**:
2. Read [GDD_MASTER.md](GDD_MASTER.md) - Sections: Executive Summary, Vision, Three-Act Structure
3. Read [GDD_README.md](GDD_README.md) - How to use this documentation

**2-Hour Immersion**:
4. Read [GDD_Three_Act_Structure.md](./Narrative/GDD_Three_Act_Structure.md) - Complete story (this will hook you)
5. Read [GDD_Core_Gameplay_Loop.md](./CoreGameplay/GDD_Core_Gameplay_Loop.md) - How gameplay works
6. Read [GDD_Passenger_System.md](./Tourism/GDD_Passenger_System.md) - Example of system depth

**What You'll Understand**:
- MS Tour is not just a management sim - it's a life story
- You pilot ships directly (skill-based) while managing a business (strategic)
- The game spans 60 years (1887-1950s), three acts, with family and legacy
- "Till havs" (Swedish folk song) creates emotional resonance
- Historical authenticity: Real VIPs, real events, real Sweden

**The Elevator Pitch**:
*"It's like Anno meets Stardew Valley meets Papers Please's emotional storytelling, set on ships in historical Sweden."*

**What Makes It Special**:
- **Hands-on + Strategic**: Direct piloting + business management
- **Life Story**: Romance, family, children, succession over 60 years
- **Humanized Systems**: Passengers and employees are people, not numbers
- **Historical Immersion**: 1887 Sweden with Oscar II, Nobel, WWI, WWII
- **Emotional Core**: "Till havs" and deathbed framing create nostalgia

---

## 🔄 Universal Next Steps

**No matter your role:**

1. **Bookmark These Files**:
   - [GDD_MASTER.md](GDD_MASTER.md) - Your north star
   - [GDD_QUICK_REFERENCE.md](GDD_QUICK_REFERENCE.md) - Implementation guide
   - [GDD_INDEX.md](GDD_INDEX.md) - Find anything fast

2. **Join Communication Channels**:
   - Slack #game-dev - Development discussions
   - Slack #game-design - Design decisions
   - Slack #game-narrative - Story content

3. **Understand the Vision**:
   - MS Tour is about **life, legacy, and the sea**
   - Every design decision serves the player fantasy
   - We're building something **emotionally resonant**, not just mechanically sound

4. **Respect the Pillars**:
   - Strategic Optimization
   - Progressive Complexity
   - Environmental Interaction
   - Business Management with Heart

5. **Ask Questions**:
   - No question is too small
   - Better to clarify than implement wrong
   - Documentation should answer most questions
   - If docs are unclear, flag for improvement

---

## 📚 Additional Resources

**In This Repository**:
- **PROJECT_CONTEXT.md** - Technical architecture details
- **Mind maps** - Visual references for passengers and tours

**External References**:
- Jesse Schell: "The Art of Game Design: A Book of Lenses"
- Raph Koster: "A Theory of Fun for Game Design"
- Ernest Adams: "Fundamentals of Game Design"

**Historical Research**:
- 1887 Gothenburg history
- Swedish archipelago geography
- Royal Swedish history (Oscar II)
- Swedish folk music ("Till havs")

---

## ✨ Welcome to MS Tour

**The sea awaits. Let's build something memorable.**

*"Till havs, till havs, till livets kamp och fara..."*

---

**Document Version**: 1.0  
**Last Updated**: 2025-01-01  
**Purpose**: Role-specific onboarding guide
