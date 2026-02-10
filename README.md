# MS Tour

A shipping company management game set in the Gothenburg archipelago.

## Overview

MS Tour is a game where you manage a fleet of ships navigating between islands in the beautiful Gothenburg archipelago. Handle cargo, optimize routes, and grow your shipping empire.

## Key Features

- **Advanced Tourism System**: Visit Points of Interest (POIs) throughout the archipelago
- **Realistic Sightseeing Mechanics**: Slow down and turn broadside to give passengers the best views
- **Strategic Route Planning**: Optimize routes to maximize POI visits and passenger satisfaction
- **Data-Oriented Architecture**: High-performance engine built with cache-friendly design patterns
- **Modular Systems**: ECS architecture with clear separation between engine and game logic

For detailed information about the POI system, see [docs/POI_SYSTEM.md](docs/POI_SYSTEM.md).

## Technical Details

- **Engine**: Custom C++ game engine (data-oriented design)
- **Graphics**: Raylib with orthographic rendering
- **Platforms**: Windows and Linux
- **Language**: C++ (C-style, functional programming patterns)
- **Build System**: CMake

## Building

### Prerequisites

- CMake 3.20 or higher
- C++17 compatible compiler (MSVC, GCC, or Clang)
- Git

### Build Instructions

```bash
# Clone the repository
git clone https://github.com/yourusername/MSTour.git
cd MSTour

# Create build directory
mkdir build
cd build

# Configure and build
cmake ..
cmake --build .

# Run the game
./game/MSTour  # On Linux
.\game\MSTour.exe  # On Windows
```

## Project Structure

```
MSTour/
├── engine/          # Game engine source code
│   ├── include/    # Engine headers
│   └── src/        # Engine implementation
├── game/           # Game-specific code
│   ├── include/    # Game headers
│   └── src/        # Game implementation
├── tests/          # Unit tests
├── assets/         # Game assets
│   ├── textures/   # 2D sprites and textures
│   ├── models/     # 3D ship models
│   └── sounds/     # Audio files
├── docs/           # Documentation
└── third_party/    # External dependencies
```

## Development

### Architecture

The engine follows data-oriented design principles:
- Plain C structs for data
- Pure functions operating on data
- Struct-of-arrays pattern for collections
- Minimal use of OOP constructs

### Code Style

- C-style programming with C++ features where beneficial
- Functional programming patterns
- Const correctness
- Cache-friendly data layout

## License

TBD

## Contributors

TBD
