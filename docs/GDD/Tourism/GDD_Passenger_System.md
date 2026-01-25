# Passenger System - MS Tour

**Document Type:** System Design  
**Version:** 1.0  
**Last Updated:** 2026-01-25  
**Owner:** Systems Designer  
**Related Documents:** [Main GDD](../GDD_MASTER.md), [Tours Design](../Tourism/GDD_Tours_Design.md), [Satisfaction Mechanics](../Tourism/GDD_Satisfaction_Mechanics.md), [Interest Points](../Tourism/GDD_Interest_Points.md)

---

## Overview

The Passenger System simulates individual passengers and passenger groups with distinct classes, traits, and preferences. This system humanizes the tourism business, transforming abstract "customer satisfaction" into relatable characters with personalities and desires. Passengers are not just numbers—they're people the player can understand and cater to.

## Design Intent

**Primary Goals**:
1. **Humanization**: Passengers feel like people, not statistics
2. **Strategic Depth**: Different passenger types require different strategies
3. **Feedback Clarity**: Players understand why passengers are satisfied or not
4. **Variety**: Tours feel different based on passenger composition
5. **Progression**: More sophisticated passenger types unlock over time

**Design Philosophy** (Jesse Schell):
- "Numbers with faces" - every statistic represents something human
- Player should think "This tour is perfect for wealthy art lovers"
- Not all passengers should be happy—trade-offs create strategy

---

## Passenger Classes

MS Tour features a **four-tier class system** based on 1887 Swedish society:

### Knegare (Working Class)

**Historical Context**: Factory workers, laborers, servants  
**Economic Level**: Lowest income, price-sensitive  
**Proportion of Population**: 60% (largest class)

**Characteristics**:
- **Price Expectations**: Low (10-30 kr per tour)
- **Service Expectations**: Basic (no fancy amenities required)
- **POI Preferences**: Nature, local landmarks, simple pleasures
- **Group Size**: Often large groups (families, workers' associations)
- **Satisfaction Drivers**: Fun, affordability, escape from daily toil
- **Tipping**: Rare, small (1-5% of tour price)

**Example Names**: Erik, Karin, Johan, Ingrid, Anders, Maja  
**Example Traits**: "Budget-Conscious," "Easily Pleased," "Family-Oriented"

**Unlock**: Available from start (Act 1)

### Borgare (Middle Class)

**Historical Context**: Merchants, shopkeepers, clerks, minor officials  
**Economic Level**: Moderate income, value-conscious  
**Proportion of Population**: 25%

**Characteristics**:
- **Price Expectations**: Medium (30-80 kr per tour)
- **Service Expectations**: Decent (want comfortable seating, basic galley)
- **POI Preferences**: Historical sites, cultural landmarks, respectable destinations
- **Group Size**: Small families, couples
- **Satisfaction Drivers**: Quality, respectability, cultural enrichment
- **Tipping**: Moderate (5-10% if satisfied)

**Example Names**: Karl, Sofia, Fredrik, Emma, Gustaf, Helena  
**Example Traits**: "Cultured," "Expects Quality," "Respectable"

**Unlock**: Available from start (Act 1)

### Adel (Nobility/Upper Class)

**Historical Context**: Aristocrats, wealthy landowners, successful businessmen  
**Economic Level**: High income, quality-focused  
**Proportion of Population**: 10%

**Characteristics**:
- **Price Expectations**: High (80-200 kr per tour)
- **Service Expectations**: Luxury (require dining room, observation deck, bar)
- **POI Preferences**: Exclusive sites, scenic beauty, prestigious destinations
- **Group Size**: Small parties, often with servants
- **Satisfaction Drivers**: Exclusivity, comfort, prestige, service quality
- **Tipping**: Generous (10-20% if impressed)

**Example Names**: Baron Erik, Lady Astrid, Count Gustav, Baroness Elisabet  
**Example Traits**: "Demanding," "Appreciates Luxury," "Status-Conscious"

**Unlock**: Mid Act 1 (requires reaching "Rising Star" prestige tier)

### VIP (Royalty and Celebrities)

**Historical Context**: Swedish Royal Family, famous authors, scientists, artists  
**Economic Level**: Extremely high, prestige-driven  
**Proportion of Population**: < 1% (unique individuals)

**Characteristics**:
- **Price Expectations**: Very high (200-500 kr per tour, often contracted)
- **Service Expectations**: Impeccable (best amenities, perfect service)
- **POI Preferences**: Curated experiences, exclusive access, historical significance
- **Group Size**: Individual or small entourage
- **Satisfaction Drivers**: Privacy, perfection, memorable experiences
- **Tipping**: N/A (contract-based, reputation reward instead)

**Example VIPs**: King Oscar II, Alfred Nobel, August Strindberg, Selma Lagerlöf  
**Example Traits**: "Influential," "Discerning," "Historically Significant"

**Unlock**: Act 2+ (requires "Renowned Captain" or higher prestige, specific contracts)

*Detailed VIP system in [VIP System](GDD_VIP_System.md)*

---

## Passenger Traits

Each passenger (or passenger group) has **1-3 traits** that modify their behavior and satisfaction.

### Positive Traits (Easy to Satisfy)

| Trait | Effect | Appears In |
|-------|--------|------------|
| **Easily Pleased** | +20% base satisfaction | Knegare, Borgare |
| **Nature Lover** | +30% satisfaction from nature POIs | All classes |
| **History Buff** | +30% satisfaction from historical POIs | Borgare, Adel |
| **Adventurous** | +20% satisfaction, -50% discomfort from rough seas | Knegare, Borgare |
| **Curious** | +10% satisfaction from any new POI (first visit) | All classes |
| **Patient** | -50% dissatisfaction from delays | Borgare, Adel |
| **Grateful** | Tips +50% if satisfied | All classes |

### Neutral Traits (Flavor)

| Trait | Effect | Appears In |
|-------|--------|------------|
| **Photographer** | Slow down at POIs (wants time for photos), +10% satisfaction if allowed | Borgare, Adel |
| **Romantic Couple** | +20% satisfaction from scenic POIs at sunset/sunrise | Borgare, Adel |
| **Family Group** | Satisfaction averaged across multiple members (can be volatile) | Knegare, Borgare |
| **First-Timer** | +10% satisfaction (everything is new), asks many questions | All classes |
| **Repeat Customer** | Expects improvements, -10% satisfaction if same as last time | Borgare, Adel |

### Negative Traits (Difficult to Satisfy)

| Trait | Effect | Appears In |
|-------|--------|------------|
| **Demanding** | -20% base satisfaction, higher expectations | Adel, VIP |
| **Impatient** | -10% satisfaction per minute of delay | Borgare, Adel |
| **Easily Seasick** | -30% satisfaction in rough weather | All classes |
| **Snob** | -20% satisfaction if mixed with lower classes | Adel |
| **Critic** | Notices and comments on all flaws, -10% satisfaction | Borgare, Adel |
| **Penny Pincher** | -20% satisfaction if price is high, won't tip | Knegare, Borgare |

### Special Traits (Rare)

| Trait | Effect | Appears In |
|-------|--------|------------|
| **Journalist** | Satisfaction impacts newspaper coverage (double reputation effect) | Borgare, Adel |
| **Rival Company** | Secretly scouting competition, reports to rivals | Borgare |
| **Inspector** | Secretly rating service, major reputation impact | Adel, VIP |
| **Influencer** | High satisfaction = word-of-mouth boost (unlock new passengers) | Adel |

---

## Passenger Preferences

### POI Type Preferences

Different classes prefer different types of Points of Interest:

| Class | Nature POIs | Historical POIs | Military POIs | Preference Notes |
|-------|-------------|-----------------|---------------|------------------|
| **Knegare** | +30% | +10% | +20% | Love nature, some interest in military, less in formal history |
| **Borgare** | +20% | +30% | +10% | Educated, value historical and cultural sites |
| **Adel** | +25% | +25% | +15% | Appreciate all quality sites, prefer exclusivity |
| **VIP** | Variable | Variable | Variable | Depends on individual (Nobel loves science sites, etc.) |

*Full POI system in [Interest Points](GDD_Interest_Points.md)*

### Service Preferences

| Class | Required Amenities | Preferred Amenities | Won't Use |
|-------|-------------------|---------------------|-----------|
| **Knegare** | None | Basic seating | Luxury dining, bar |
| **Borgare** | Seating, basic galley | Dining area, restroom | Top-tier luxury |
| **Adel** | Dining room, observation deck | Bar, premium seating, luxury cabins | Basic amenities |
| **VIP** | All top-tier amenities | Private areas, perfect service | Anything "common" |

*Amenity details in [Ship Amenities](GDD_Ship_Amenities.md)*

### Weather Preferences

| Class | Fair Weather | Light Wind | Moderate Waves | Storm |
|-------|--------------|------------|----------------|-------|
| **Knegare** | 100% comfort | 90% | 70% | 40% |
| **Borgare** | 100% comfort | 85% | 60% | 30% |
| **Adel** | 100% comfort | 80% | 50% | 20% |
| **VIP** | 100% comfort | 75% | 40% | 10% |

*Modifiers: Adventurous trait increases storm tolerance, Seasick decreases it*

---

## Satisfaction Calculation

Each passenger group has a **satisfaction score** (0-100%) calculated in real-time during the voyage.

### Base Satisfaction Formula

```
BaseSatisfaction = 50% (neutral start point)

// Positive Factors
+ POI_Quality (0-30%)
+ Route_Efficiency (0-10%)
+ Service_Quality (0-20%)
+ Weather_Comfort (0-10%)
+ Special_Moments (0-10%)

// Negative Factors
- Delays (-5% per minute late)
- Rough_Conditions (-10% to -30%)
- Poor_Service (-10% to -30%)
- Grounding_Incidents (-20% per grounding)
- Missing_Expected_Amenities (-15%)

// Trait Modifiers
* Trait adjustments (vary by trait)

// Class Expectations
* Class modifier (Knegare: +10%, Borgare: 0%, Adel: -10%, VIP: -20%)

FinalSatisfaction = Clamp(Sum of all factors, 0%, 100%)
```

### Satisfaction Tiers

| Tier | Score Range | Passenger Reaction | Outcome |
|------|-------------|-------------------|---------|
| **Delighted** | 85-100% | Enthusiastic praise, excellent tips | +3 reputation, tip bonus (120-150%) |
| **Satisfied** | 65-84% | Positive comments, decent tips | +2 reputation, tip bonus (100-110%) |
| **Neutral** | 45-64% | Quiet, minimal feedback | 0 reputation, minimal tips (50-80%) |
| **Disappointed** | 25-44% | Complaints, poor tips | -1 reputation, very low tips (0-30%) |
| **Angry** | 0-24% | Public complaints, refunds demanded | -3 reputation, refund demanded |

### Real-Time Feedback

During voyage, passengers provide feedback via:
- **Speech Bubbles**: Short comments appear above passenger groups
  - "What a beautiful view!"
  - "I'm feeling a bit queasy..."
  - "Is this all there is?"
  - "This is taking too long."
  
- **Satisfaction Meter**: UI element shows current average satisfaction (color-coded)
  - Green (80%+): Doing great
  - Yellow (50-79%): Acceptable
  - Orange (30-49%): Problems
  - Red (<30%): Major issues

- **Passenger Animations**: Visual body language
  - High satisfaction: Looking around, pointing, smiling animations
  - Low satisfaction: Arms crossed, looking bored, annoyed gestures

*Full details in [Satisfaction Mechanics](GDD_Satisfaction_Mechanics.md)*

---

## Passenger Generation

### Spawning System

When a tour is booked/starts:

1. **Determine Passenger Count**: Based on ship capacity and route pricing
   ```
   PassengerCount = ShipCapacity * BookingRate
   BookingRate = Function(Price, Reputation, Season, Route Quality)
   ```

2. **Generate Passenger Groups**: Create 1-5 groups (not individual passengers for performance)
   ```
   Each group:
   - Class: Weighted random based on price tier and reputation
   - Size: 1-8 passengers (Knegare larger groups, VIP solo/small)
   - Traits: 1-3 random traits based on class
   - Names: Procedurally selected from pool
   ```

3. **Calculate Expectations**: Based on class, price paid, and route promises
   ```
   Expectations = {
       POI_Quality: ClassExpectation,
       Service_Level: AmenityExpectations,
       Weather_Tolerance: ClassTolerance
   }
   ```

### Example Passenger Groups

**Budget Family Tour**:
- Group 1: Knegare family of 6 (Erik, Karin, +4 children)
  - Traits: "Family Group," "Easily Pleased"
  - Expectations: Low, just wants fun day out
  
- Group 2: Knegare workers (4 men, Johan, Anders, Lars, Per)
  - Traits: "Adventurous," "Nature Lover"
  - Expectations: Low, wants to relax and see nature

**Premium Historical Tour**:
- Group 1: Borgare couple (Fredrik & Emma)
  - Traits: "History Buff," "Romantic Couple"
  - Expectations: Medium, wants historical sites and good service
  
- Group 2: Adel lady with servant (Lady Astrid + 1)
  - Traits: "Demanding," "Cultured"
  - Expectations: High, wants luxury and exclusivity

**VIP Royal Tour**:
- Group 1: King Oscar II + entourage (4 total)
  - Traits: "Influential," "Discerning"
  - Expectations: Very high, wants perfection
  - Special: Success = major prestige boost, failure = major setback

---

## Passenger Behavior

### During Voyage

Passengers don't have complex AI; they react to voyage conditions:

**Passive Behavior**:
- Stand/sit in designated passenger areas
- Animated idle behaviors (talking, looking around)
- Move to observation points at POIs

**Reactive Behavior**:
- React to rough seas (stagger, look ill if seasick)
- Comment on POIs as ship approaches
- Show excitement/disappointment based on satisfaction

**No Direct Interaction**: Players don't talk to passengers directly during voyage (performance reasons). Interaction is indirect through:
- Reading comments/reactions
- Monitoring satisfaction meter
- Adjusting piloting/route based on feedback

### After Voyage

At voyage end, passengers:
1. **Disembark** (animation)
2. **Provide Final Rating** (satisfaction score)
3. **Leave Tips** (if satisfied)
4. **Affect Reputation** (positive or negative)
5. **May Book Again** (if highly satisfied)

---

## Passenger Data (Technical)

### Data-Oriented Design

Passengers are stored in struct-of-arrays for efficiency:

```cpp
struct PassengerGroupData {
    uint32_t[] group_ids;           // Unique ID per group
    uint8_t[] class_types;          // 0=Knegare, 1=Borgare, 2=Adel, 3=VIP
    uint8_t[] group_sizes;          // Number in group
    float[] satisfaction_scores;    // Current satisfaction (0-100)
    uint32_t[] trait_flags;         // Bitflags for traits
    uint32_t[] ship_ids;            // Which ship they're on
    
    // ... other arrays as needed
};
```

### Performance Considerations

- **Max Passengers Per Ship**: 50 (small), 150 (medium), 300 (large)
- **Groups, Not Individuals**: Process groups (5-20 groups per ship), not individual passengers
- **Batch Processing**: Update all passenger groups on all ships in one pass
- **LOD**: Distant AI ships use simplified passenger satisfaction (no real-time updates)

---

## Passenger Progression (Player Experience)

### Act 1: Simple Passengers
- **Classes Available**: Knegare, Borgare
- **Traits**: Simple, mostly positive or neutral
- **Satisfaction**: Easy to please, forgiving
- **Player Learning**: Understand basic preferences, service levels

### Act 2: Sophisticated Passengers
- **Classes Available**: All classes including Adel
- **Traits**: More variety, including negative traits
- **Satisfaction**: Higher expectations, more demanding
- **Player Challenge**: Optimize for specific passenger types, premium service

### Act 3: VIP and Prestige
- **Classes Available**: All classes + VIP contracts
- **Traits**: Complex combinations, rare special traits
- **Satisfaction**: Discerning, unforgiving, high stakes
- **Player Mastery**: Curate perfect experiences for specific audiences

---

## Strategic Implications

### Player Decisions

The passenger system creates strategic choices:

**Route Pricing**:
- Low price → Knegare → High volume, low margin, simple needs
- Medium price → Borgare → Balanced volume/margin, moderate needs
- High price → Adel → Low volume, high margin, luxury needs
- Very high price → VIP → Contract-based, perfect execution required

**Ship Configuration**:
- Basic amenities → Target Knegare (cost-effective)
- Moderate amenities → Target Borgare (versatile)
- Luxury amenities → Target Adel (high margin)
- Top-tier amenities → Target VIP (prestige)

**Route Design**:
- Nature-heavy → Knegare preference
- Historical-heavy → Borgare preference
- Exclusive/scenic → Adel preference
- Curated experience → VIP requirement

**Fleet Specialization**:
- Some ships for volume (Knegare routes)
- Some ships for premium (Adel routes)
- Flagship for VIP (Oscar II tours)

### Optimization Strategies

**Volume Strategy**:
- Target Knegare with budget tours
- Maximize passengers per voyage (fill ships)
- Simple routes, fast turnaround
- Profit through scale

**Premium Strategy**:
- Target Adel with luxury tours
- Smaller passenger counts, higher prices
- Complex routes, exceptional service
- Profit through margin

**Balanced Strategy**:
- Target Borgare as core business
- Mix of Knegare (volume) and Adel (margin)
- Flexible routes, good service
- Profit through versatility

---

## Design Notes

> **On Humanization**: We want players to think "that family loved the lighthouse" not "satisfaction increased by 15%." The passenger system uses names, traits, and reactions to create this feeling, even though underneath it's still numbers.

> **On Class Sensitivity**: The class system reflects historical 1887 Sweden. We present it as historical fact, not value judgment. The game doesn't endorse class hierarchy; it simulates a historical tourism business within that context.

> **On Complexity**: While the system has depth, it should not overwhelm players. Act 1 introduces concepts simply (two classes, simple traits). Complexity increases gradually. By Act 3, players who want deep optimization can master passenger preferences, but casual players can still succeed with intuitive choices.

> **On VIP Integration**: VIPs are not just "rich passengers"—they're historical figures with stories. Each VIP should feel like an event, not a transaction. Their satisfaction is tied to reputation and narrative, not just revenue.

---

## Success Metrics

### System Health
- **Class Distribution**: Healthy mix of all classes being served (not 100% Knegare)
- **Satisfaction Average**: 65-75% average satisfaction across all tours (challenging but achievable)
- **Trait Variety**: 80%+ of traits appear in regular gameplay (none too rare/common)

### Player Understanding
- **Preference Recognition**: 70%+ of players can name 2-3 class preferences (survey)
- **Strategic Adaptation**: 60%+ adjust routes/ships for passenger types (telemetry)
- **Trait Awareness**: 50%+ notice and respond to passenger traits

### Engagement
- **Passenger Comments Read**: 70%+ of players report reading passenger feedback
- **Satisfaction Monitoring**: 80%+ regularly check satisfaction meter during voyages
- **Repeat Customers**: 40%+ notice when passengers book again (if implemented)

---

## Future Enhancements (Post-Launch)

- **Passenger Stories**: Multi-voyage narratives for repeat passengers
- **Passenger Requests**: Mid-voyage special requests (photo stops, detours)
- **Loyalty System**: Repeat customers become "regulars" with bonuses
- **Passenger Interactions**: Direct dialogue options (stretch goal)
- **Expanded Traits**: 50+ traits for even more variety

---

**Document End**

*For related systems, see: [Tours Design](../Tourism/GDD_Tours_Design.md), [Satisfaction Mechanics](../Tourism/GDD_Satisfaction_Mechanics.md), [Interest Points](../Tourism/GDD_Interest_Points.md), [Ship Amenities](../ShipBuilding/GDD_Ship_Amenities.md), [VIP System](../Historical/GDD_VIP_System.md)*
