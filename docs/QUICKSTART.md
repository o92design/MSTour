# Quick Start Guide

## Build and Run (First Time)

### Step 1: Build
```cmd
cd C:\Dev\projects\MSTour\build
cmake ..
cmake --build .
```

**Expected**: CMake will download Raylib and Google Test (first time only, ~2-3 minutes), then compile the project.

### Step 2: Run the Game
```cmd
.\bin\Debug\MSTour.exe
```

**Expected**: A window opens showing:
- Title: "MS Tour - Archipelago Shipping Company"
- Blue sky background
- Red ship placeholder
- FPS counter

### Step 3: Run Tests
```cmd
.\Debug\MSTour_tests.exe
```

**Expected**: All 4 tests pass.

---

## After Making Code Changes

```cmd
cd C:\Dev\projects\MSTour\build
cmake --build .
.\bin\Debug\MSTour.exe
```

---

## Common Commands

### Clean and Rebuild
```cmd
cd C:\Dev\projects\MSTour
rmdir /s /q build
mkdir build
cd build
cmake ..
cmake --build .
```

### Build in Release Mode
```cmd
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
.\bin\Release\MSTour.exe
```

### Run Tests with CTest
```cmd
cd build
ctest --verbose
```

---

## Troubleshooting

**Q: "cmake is not recognized"**  
A: Install CMake from https://cmake.org/download/ and add to PATH

**Q: "No CMAKE_CXX_COMPILER found"**  
A: Install Visual Studio with C++ workload

**Q: Build takes forever**  
A: First build downloads dependencies. Subsequent builds are faster.

**Q: "cannot open file 'raylib.lib'"**  
A: Clean and rebuild. Raylib didn't compile properly.

---

## Project Structure Quick Reference

```
Engine Code:     engine/src/*.c, engine/include/*.h
Game Code:       game/src/*.c, game/include/*.h
Tests:           tests/*.cpp
Assets:          assets/textures, assets/models, assets/sounds
Documentation:   docs/*.md
```

---

## Development Workflow

1. **Make changes** to engine or game code
2. **Build**: `cmake --build .` (in build directory)
3. **Run**: `.\bin\Debug\MSTour.exe`
4. **Test**: `.\Debug\MSTour_tests.exe`
5. **Commit**: `git add . && git commit -m "message"`

---

## Next: Phase 2

Once verified working, see `plan.md` for Phase 2: Game Engine Foundation
- Input handling
- Camera system
- Entity Component System
- Resource management
