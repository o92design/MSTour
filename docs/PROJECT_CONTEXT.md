# MS Tour - Project Context

## What is MS Tour?
A shipping company management simulation game set in the beautiful Gothenburg archipelago in Sweden. Players manage a fleet of ships, handle cargo, optimize routes between islands, and grow their maritime business.

## Project Philosophy

### Data-Oriented Design (DOD)
We prioritize:
- **Data layout over object hierarchies**
- **Transform functions over methods**
- **Struct-of-Arrays over Array-of-Structs**
- **Cache-friendly access patterns**
- **Minimal indirection**

### Why C-style in C++?
- Predictable memory layout
- Better performance control
- Simpler debugging
- Easier to reason about
- No hidden costs (vtables, RTTI, exceptions)

### Functional Programming Patterns
- Pure functions where possible
- Immutable data structures
- Stateless transformations
- Explicit side effects

## Game Design Vision

### Setting: Gothenburg Archipelago
- Multiple islands and ports
- Varied routes and distances
- Weather conditions affecting travel
- Seasonal variations

### Gameplay Core Loop
1. **Manage Fleet**: Purchase, maintain, assign ships
2. **Plan Routes**: Optimize for profit and efficiency
3. **Handle Cargo**: Different types, deadlines, values
4. **Expand Business**: Unlock new routes, upgrade ships
5. **Deal with Events**: Weather, breakdowns, market changes

### Technical Requirements
- **Rendering**: Orthographic view with 3D ship models on 2D pre-rendered backgrounds
- **Performance**: 60 FPS on modest hardware
- **Platforms**: Windows and Linux
- **Scale**: Handle 50+ ships, 20+ islands

## Architecture Overview

### Engine Layer (engine/)
Pure C library providing:
- **Core**: Window, timing, lifecycle
- **Renderer**: 2D/3D rendering, camera
- **Input**: Keyboard, mouse handling
- **ECS**: Entity-Component-System
- **Resources**: Asset loading and caching

### Game Layer (game/)
Uses engine to implement:
- **Ships**: Movement, AI, pathfinding
- **World**: Islands, routes, map
- **Economy**: Cargo, pricing, profit
- **UI**: Menus, HUD, information displays
- **Progression**: Unlocks, achievements

### Separation of Concerns
```
Game Logic → Engine API → Raylib → OS/GPU
   ↑            ↑           ↑
  Pure C     Pure C      C/C++
```

## Development Workflow

### Feature Development
1. **Design**: Write specification in docs/
2. **Implement**: Engine first, then game integration
3. **Test**: Unit tests + manual testing
4. **Document**: Update relevant docs
5. **Jira**: Update ticket with changes
6. **Commit**: Atomic commits with clear messages

### Testing Strategy
- **Unit tests**: Pure functions and data structures
- **Integration tests**: System interactions
- **Manual testing**: Gameplay verification
- **Performance tests**: Frame rate, memory usage

### Build Process
```bash
# Configure (first time or after CMake changes)
cd build && cmake ..

# Build
cmake --build .

# Run tests
./tests/Debug/MSTour_tests.exe

# Run game
./bin/Debug/MSTour.exe
```

## Current Status

### Phase 1: Complete ✅
- Project structure
- Build system
- Basic engine (core, renderer)
- Basic game loop
- Tests passing

### Phase 2: Planned 🟡
- Input handling
- Camera system
- Entity Component System
- Resource management
- Enhanced rendering

### Phase 3: Future 🔵
- Game mechanics (ships, routes)
- UI framework
- Save/load system
- Audio system

## Key Design Decisions

### Why Raylib?
- Simple, beginner-friendly API
- Cross-platform
- Active community
- Good documentation
- Handles both 2D and 3D

### Why CMake?
- Industry standard
- Cross-platform
- FetchContent for dependencies
- Good IDE integration

### Why Google Test?
- Industry standard for C++
- Rich assertion library
- Good tooling support
- Easy to learn

### Why No Custom Types for Raylib?
- **Lesson learned**: Don't duplicate library types
- **Benefits**: Simpler code, fewer conversions
- **Approach**: Wrap functions, not types

## Jira Workflow

### Project: MS Tour (MST)
- **Base URL**: https://o92design.atlassian.net/browse/MST-{number}

### Ticket Structure
Every ticket should have:
1. **Purpose**: Why this task exists
2. **Description**: What needs to be done
3. **Definition of Done**: Clear completion criteria
4. **Test Plan**: How to verify it works
5. **Changes**: Files modified/added (filled at completion)

### Completed Tickets
- **MST-2**: Project setup and structure
- **MST-3**: Engine foundation (planned)

### Updating Tickets
After completing work:
1. Document all changes (files, functions added/modified)
2. Describe test results
3. Confirm definition of done met
4. Link any related commits

## Communication Guidelines

### Code Comments
- **Headers**: Document public API thoroughly
- **Implementation**: Comment complex logic
- **Avoid**: Obvious comments ("increments i")
- **Prefer**: Self-documenting code

### Commit Messages
```
[MST-X] Brief description (50 chars or less)

Detailed explanation of what changed and why.
Include any relevant context or decisions made.

- Bullet points for multiple changes
- Reference Jira ticket: MST-X
```

### Documentation
- Keep docs/ up to date
- Document design decisions
- Include examples
- Update README for major changes

## Resources

### Internal Docs
- `README.md`: Project overview
- `docs/BUILD.md`: Build instructions
- `docs/QUICKSTART.md`: Quick start guide
- `docs/AGENTS.md`: Custom agent definitions
- `.github/copilot-instructions.md`: Copilot guidelines

### External References
- [Raylib Cheatsheet](https://www.raylib.com/cheatsheet/cheatsheet.html)
- [CMake Documentation](https://cmake.org/documentation/)
- [Google Test Primer](https://google.github.io/googletest/primer.html)
- [Data-Oriented Design](https://www.dataorienteddesign.com/dodbook/)

## Common Patterns

### Adding Engine Feature
```c
// Header: engine/include/engine_feature.h
#ifndef ENGINE_FEATURE_H
#define ENGINE_FEATURE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    // Data members
} FeatureData;

void feature_init(FeatureData* data);
void feature_update(FeatureData* data, double delta_time);
void feature_shutdown(FeatureData* data);

#ifdef __cplusplus
}
#endif

#endif // ENGINE_FEATURE_H
```

### Testing Pattern
```cpp
TEST(FeatureTests, SpecificBehavior) {
    FeatureData data;
    feature_init(&data);
    
    // Test specific behavior
    feature_update(&data, 0.016);
    
    EXPECT_EQ(data.some_value, expected_value);
    
    feature_shutdown(&data);
}
```

## Success Metrics
- **Code Quality**: Clean, readable, maintainable
- **Performance**: 60 FPS with many entities
- **Test Coverage**: Critical paths covered
- **Documentation**: Up to date and accurate
- **Jira**: Tasks tracked and updated
