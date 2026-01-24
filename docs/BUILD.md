# Build Instructions

## Prerequisites

### Required Tools
- **CMake** 3.20 or higher ([Download](https://cmake.org/download/))
- **C++ Compiler**:
  - Windows: Visual Studio 2019+ or MinGW-w64
  - Linux: GCC 9+ or Clang 10+
- **Git** (for cloning and version control)

### Optional but Recommended
- **PowerShell Core** 7+ ([Download](https://aka.ms/powershell)) - for better scripting support
- **Ninja** build system ([Download](https://ninja-build.org/)) - faster builds

## Building on Windows

### Option 1: Visual Studio Generator (Recommended)

1. **Open Command Prompt or PowerShell** in the project root:
   ```cmd
   cd C:\Dev\projects\MSTour
   ```

2. **Configure the project**:
   ```cmd
   cd build
   cmake ..
   ```
   
   Or specify Visual Studio version:
   ```cmd
   cmake .. -G "Visual Studio 17 2022"
   ```

3. **Build the project**:
   ```cmd
   cmake --build .
   ```
   
   Or build in Release mode:
   ```cmd
   cmake --build . --config Release
   ```

4. **Run the game**:
   ```cmd
   .\bin\Debug\MSTour.exe
   ```

5. **Run tests**:
   ```cmd
   .\Debug\MSTour_tests.exe
   ```

### Option 2: Ninja Generator (Faster)

1. **Configure with Ninja**:
   ```cmd
   cd build
   cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Debug
   ```

2. **Build**:
   ```cmd
   cmake --build .
   ```

## Building on Linux

1. **Install dependencies**:
   ```bash
   # Ubuntu/Debian
   sudo apt-get install build-essential cmake git

   # Fedora
   sudo dnf install gcc-c++ cmake git

   # Arch
   sudo pacman -S base-devel cmake git
   ```

2. **Build the project**:
   ```bash
   cd build
   cmake ..
   cmake --build .
   ```

3. **Run the game**:
   ```bash
   ./bin/MSTour
   ```

4. **Run tests**:
   ```bash
   ./MSTour_tests
   ```

## Build Configurations

### Debug Build (Default)
- Includes debugging symbols
- No optimizations
- Assertions enabled
- Console window visible on Windows

```cmd
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build .
```

### Release Build
- Optimizations enabled
- No debugging symbols
- Faster execution
- No console window on Windows

```cmd
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

### RelWithDebInfo Build
- Optimizations enabled
- Debugging symbols included
- Good for profiling

```cmd
cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build .
```

## Cleaning the Build

To clean and rebuild from scratch:

```cmd
# Windows
cd C:\Dev\projects\MSTour
rmdir /s /q build
mkdir build
cd build
cmake ..
cmake --build .

# Linux
cd /path/to/MSTour
rm -rf build
mkdir build
cd build
cmake ..
cmake --build .
```

## Common Build Issues

### Issue: CMake not found
**Solution**: Install CMake and add it to your PATH, or specify full path to cmake.exe

### Issue: No C++ compiler found
**Solution**: 
- Windows: Install Visual Studio with C++ workload or MinGW
- Linux: Install build-essential (Ubuntu) or equivalent

### Issue: Raylib or Google Test download fails
**Solution**: 
- Check internet connection
- Try building again (CMake will resume download)
- Manually clone dependencies if needed

### Issue: "LINK : fatal error LNK1104: cannot open file 'raylib.lib'"
**Solution**: Clean and rebuild. Raylib might not have compiled properly.

## Running Tests

### All Tests
```cmd
# Windows
.\Debug\MSTour_tests.exe

# Linux
./MSTour_tests
```

### With CTest
```cmd
cd build
ctest
```

### Verbose Test Output
```cmd
ctest --verbose
```

## IDE Integration

### Visual Studio
1. Open Visual Studio
2. File → Open → CMake...
3. Select `CMakeLists.txt` in project root
4. Visual Studio will configure automatically

### Visual Studio Code
1. Install "CMake Tools" extension
2. Open project folder
3. Press Ctrl+Shift+P → "CMake: Configure"
4. Press F7 to build

### CLion
1. Open project folder
2. CLion will detect CMakeLists.txt automatically
3. Build → Build Project

## Next Steps

After successful build:
1. Run the game to see the basic window
2. Run tests to verify engine components
3. Start implementing game features (see Phase 2 in plan.md)
