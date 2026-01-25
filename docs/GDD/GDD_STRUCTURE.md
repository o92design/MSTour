# MS TOUR - GDD Directory Structure

This document outlines the modular Game Design Document (GDD) structure for MS Tour.

## Directory Structure

```
docs/design/GDD/
├── MS_TOUR_GDD.md                          # Main GDD (master document)
│
├── CoreGameplay/
│   ├── CoreLoop.md
│   ├── PlayerFantasy.md
│   └── DesignPillars.md
│
├── Narrative/
│   ├── ThreeActStructure.md
│   ├── FramingDevice.md
│   ├── CharacterDesign.md
│   └── HistoricalContext.md
│
├── ShipControl/
│   ├── DirectControl.md
│   ├── GroundingSystem.md
│   ├── CapsizingSystem.md
│   ├── DockingSystem.md
│   └── Physics.md
│
├── Tourism/
│   ├── InterestPoints.md
│   ├── PassengerSystem.md
│   ├── ToursDesign.md
│   ├── SatisfactionMechanics.md
│   └── PrestigeSystem.md
│
├── ShipBuilding/
│   ├── ModularSystem.md
│   ├── ShipTypes.md
│   ├── Amenities.md
│   └── VisualCustomization.md
│
├── Employees/
│   ├── EmployeeTypes.md
│   ├── HiringSystem.md
│   ├── SkillProgression.md
│   └── DelegationMechanics.md
│
├── PlanningDiscovery/
│   ├── PlanningRoom.md
│   ├── SeaCharts.md
│   ├── RouteDesign.md
│   └── FogOfWar.md
│
├── Progression/
│   ├── ActProgression.md
│   ├── SkillTrees.md
│   ├── UnlockSystems.md
│   └── EconomicProgression.md
│
├── Historical/
│   ├── VIPSystem.md
│   ├── HistoricalEvents.md
│   ├── NewspaperSystem.md
│   └── Accuracy.md
│
├── FamilySuccession/
│   ├── WifeSystem.md
│   ├── ChildrenSystem.md
│   ├── SuccessionMechanics.md
│   └── GenderChoice.md
│
├── RiskChallenge/
│   ├── WeatherSystem.md
│   ├── SeasonalCycle.md
│   ├── BankruptcySystem.md
│   ├── RescueMechanics.md
│   └── DynamicEvents.md
│
├── Economy/
│   ├── PricingModel.md
│   ├── CostStructure.md
│   ├── ProfitOptimization.md
│   └── ResourceManagement.md
│
├── UIUX/
│   ├── HarborScene.md
│   ├── PlanningInterface.md
│   ├── InVoyageUI.md
│   └── MenusAndScreens.md
│
├── ContentScope/
│   ├── ArchipelagoMap.md
│   ├── InterestPointDatabase.md
│   ├── CharacterRoster.md
│   └── TourCatalog.md
│
├── AudioAesthetics/
│   ├── MusicDesign.md
│   ├── SoundDesign.md
│   └── VisualStyle.md
│
├── Technical/
│   ├── EngineArchitecture.md
│   ├── DataStructures.md
│   ├── PerformanceTargets.md
│   └── PlatformConsiderations.md
│
├── DifficultyAccessibility/
│   ├── DifficultyModes.md
│   ├── AssistOptions.md
│   ├── Onboarding.md
│   └── PlayerGuidance.md
│
└── FutureExpansion/
    ├── GoodsDLC.md
    ├── InternationalDLC.md
    └── PostLaunchContent.md
```

## Setup Instructions

To create this structure:

1. Create the base directory: `docs/design/GDD/`
2. Create each subdirectory as listed above
3. Place the respective `.md` files in each directory
4. Start with `MS_TOUR_GDD.md` as the master index

## Document Standards

All documents follow these standards:

### Header Format
```markdown
# [System/Feature Name]

**Document Type:** [System Design / Feature Specification / Content Design]  
**Version:** 1.0  
**Last Updated:** [Date]  
**Owner:** [Designer Name]  
**Related Documents:** [Links to related docs]

---

## Overview
[Brief description]

## [Content Sections]
...
```

### Cross-Referencing
- Use relative links: `[Link Text](../OtherArea/Document.md)`
- Reference the main GDD: `[Main GDD](../MS_TOUR_GDD.md)`
- Link bidirectionally when systems interact

### Formatting Conventions
- Use `##` for major sections
- Use `###` for subsections
- Use tables for comparisons and data
- Use code blocks for formulas and pseudo-code
- Use callout quotes `>` for design notes
- Include examples where helpful

## Usage

The main GDD (`MS_TOUR_GDD.md`) serves as the entry point and provides:
- Executive summary
- Vision statement
- High-level overviews
- Links to all detailed documents

Each area document provides implementation-level detail for its specific system or feature.

## Maintenance

- Update version numbers when making significant changes
- Keep the "Last Updated" date current
- Add to version history section when making major revisions
- Ensure cross-references remain valid
- Review related documents when making changes
