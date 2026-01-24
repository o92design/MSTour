# GitHub Copilot Instructions for MS Tour

## Project Overview
MS Tour is a shipping company management game set in the Gothenburg archipelago, built with a custom C++ game engine following data-oriented design principles.

## Architecture Principles

### Code Style
- **C-style programming** with C++ features where beneficial
- **Data-oriented design**: Prefer structs over classes
- **Functional patterns**: Pure functions operating on data
- **Struct-of-Arrays (SoA)**: For collections and performance-critical code
- **Const correctness**: Use const extensively
- **Minimal OOP**: Avoid inheritance, prefer composition

### Naming Conventions
- **Functions**: `snake_case` (e.g., `engine_init`, `renderer_clear`)
- **Types**: `PascalCase` for structs (e.g., `EngineConfig`, `EngineState`)
- **Variables**: `snake_case` (e.g., `window_width`, `delta_time`)
- **Constants**: `UPPER_SNAKE_CASE` (e.g., `MAX_ENTITIES`)
- **Files**: `snake_case` (e.g., `engine_core.c`, `engine_renderer.h`)

### Project Structure
```
engine/          # Game engine (C library, data-oriented)
  include/       # Public engine API headers
  src/           # Engine implementation
game/            # Game-specific code (uses engine)
  include/       # Game headers
  src/           # Game implementation
tests/           # Unit tests (Google Test)
assets/          # Game assets (textures, models, sounds)
docs/            # Documentation
```

## Technical Stack
- **Language**: C17 with C++17 for tests
- **Graphics**: Raylib 5.0
- **Build**: CMake 3.20+
- **Testing**: Google Test 1.14.0
- **Platforms**: Windows + Linux

## Code Generation Guidelines

### When writing engine code:
- Use extern "C" for C++ compatibility
- Keep headers minimal and focused
- Separate interface from implementation
- Document public APIs with comments
- Prefer data structures over object hierarchies

### When writing game code:
- Build on engine abstractions
- Keep game logic separate from engine
- Use Raylib types directly (e.g., `Color`, `Vector2`)
- Focus on gameplay, not low-level details

### When writing tests:
- Test one thing at a time
- Use descriptive test names
- Test data structures and pure functions
- Mock external dependencies if needed

## Dependencies
- **Raylib**: Use directly for graphics, input, audio
- **No custom Color types**: Use Raylib's `Color`
- **Fetch via CMake**: All dependencies managed by FetchContent

## Performance Considerations
- Cache-friendly data layout (SoA pattern)
- Minimize pointer chasing
- Batch operations when possible
- Profile before optimizing

## Common Tasks

### Adding a new engine system:
1. Create header in `engine/include/`
2. Create implementation in `engine/src/`
3. Update engine CMakeLists.txt if needed
4. Add tests in `tests/`

### Adding game features:
1. Create files in `game/src/` or `game/include/`
2. Use engine APIs
3. Test manually by running the game

### Building:
```bash
cd build
cmake ..
cmake --build .
```

## Jira Integration
- **Project Key**: MST
- **Task Format**: MST-{number}
- Track: Purpose, Description, Definition of Done, Test Plan, Changes

## DO NOT
- Don't create custom types that duplicate Raylib
- Don't use C++ OOP patterns in engine code
- Don't add dependencies without discussion
- Don't optimize prematurely
- Don't break the build

## DO
- Write clear, self-documenting code
- Test your changes
- Keep it simple
- Follow data-oriented principles
- Use Raylib's types and functions
