# Jira Integration Guide for MS Tour

## Project Information
- **Project Key**: MST
- **Base URL**: https://o92design.atlassian.net/browse/
- **Current Sprint**: Phase 1 (Project Setup)

## Ticket Structure

### Required Fields
1. **Purpose**: High-level reason for this task
2. **Description**: Detailed what needs to be done
3. **Definition of Done**: Specific, measurable completion criteria
4. **Test Plan**: Steps to verify the work
5. **Changes**: Files and code modified (filled after completion)

## Ticket Update Template

```markdown
## Changes Made

### Files Created
- `path/to/file.c` - Description of file purpose
- `path/to/file.h` - Description of header

### Files Modified
- `path/to/existing.c`
  - Added: `function_name()` - what it does
  - Modified: `existing_function()` - what changed
  - Fixed: bug description

### Summary
Brief overview of all changes made.

## Test Plan Execution

### Build Test
```bash
cd C:\Dev\projects\MSTour\build
cmake ..
cmake --build .
```
**Result**: ✅ Build successful / ❌ Build failed

### Unit Tests
```bash
cd C:\Dev\projects\MSTour\build\tests\Debug
.\MSTour_tests.exe
```
**Result**: ✅ All X tests passed / ❌ Y tests failed

### Manual Testing
1. Step-by-step test procedure
2. Expected results
3. Actual results

**Result**: ✅ Passed / ❌ Failed

## Definition of Done Checklist
- [ ] Code compiles without errors
- [ ] All tests pass
- [ ] No new warnings introduced
- [ ] Code follows project style guide
- [ ] Public APIs documented
- [ ] Changes committed to git
- [ ] Jira ticket updated

## Related Commits
- Commit SHA: Brief description
- Link to commit in repository

## Notes
Any additional context, issues encountered, or decisions made.
```

## Completed Tickets

### MST-2: Project Setup and Build System

**Purpose**: Establish foundational project structure and build configuration

**Description**:
Set up the MS Tour project with a complete C++ game engine foundation using CMake, Raylib, and Google Test. Create directory structure, configure build system, and implement basic engine and game code.

**Definition of Done**:
- [x] Project directory structure created
- [x] CMake build system configured
- [x] Raylib 5.0 integrated
- [x] Google Test 1.14.0 integrated
- [x] Basic engine code (core, renderer) implemented
- [x] Basic game code with game loop
- [x] Unit tests created and passing
- [x] Project builds successfully on Windows
- [x] Game runs and displays window
- [x] Documentation created (README, BUILD, QUICKSTART)

**Changes**:
```
Created:
- CMakeLists.txt - Root build configuration
- engine/CMakeLists.txt - Engine library build
- engine/include/engine_core.h - Core engine API
- engine/include/engine_renderer.h - Renderer API
- engine/src/engine_core.c - Core engine implementation
- engine/src/engine_renderer.c - Renderer implementation
- game/CMakeLists.txt - Game executable build
- game/src/main.c - Game entry point and main loop
- tests/CMakeLists.txt - Test suite build
- tests/engine_tests.cpp - Engine unit tests
- .gitignore - Git ignore rules
- .gitattributes - Line ending configuration
- README.md - Project documentation
- docs/BUILD.md - Build instructions
- docs/QUICKSTART.md - Quick start guide
- docs/SETUP_SUMMARY.md - Setup summary
```

**Test Plan**:
1. Build: `cd build && cmake .. && cmake --build .`
   - Result: ✅ Build successful
2. Tests: `.\build\tests\Debug\MSTour_tests.exe`
   - Result: ✅ 3/3 tests passed
3. Game: `.\build\bin\Debug\MSTour.exe`
   - Result: ✅ Window opens, displays title and ship placeholder

**Status**: ✅ COMPLETE

---

### MST-3: Engine Foundation - Core Systems

**Purpose**: Implement core game engine systems following data-oriented design principles

**Description**:
Build foundational engine systems including input handling, camera, Entity Component System, and resource management. These systems will form the backbone of the game engine.

**Definition of Done**:
- [ ] Input handling system (keyboard, mouse)
- [ ] Camera system with orthographic projection
- [ ] Entity Component System (ECS) framework
- [ ] Resource management (textures, models, audio)
- [ ] Enhanced renderer (3D models, sprites, layering)
- [ ] Unit tests for all systems
- [ ] Documentation updated
- [ ] Performance: 60 FPS maintained

**Test Plan**:
```
1. Build System
   - Run: cmake --build build
   - Expected: Clean build, no warnings
   
2. Unit Tests
   - Run: .\build\tests\Debug\MSTour_tests.exe
   - Expected: All tests pass
   
3. Input Testing
   - Run game
   - Test keyboard input
   - Test mouse input
   - Expected: Responsive input handling
   
4. Camera Testing
   - Run game
   - Test camera movement
   - Test camera zoom
   - Expected: Smooth camera control
   
5. ECS Testing
   - Create entities
   - Add components
   - Run systems
   - Expected: Entities behave correctly
   
6. Resource Loading
   - Load textures
   - Load 3D models
   - Expected: Assets load without errors
   
7. Performance
   - Run game for 5 minutes
   - Monitor FPS
   - Expected: Consistent 60 FPS
```

**Changes**: (To be filled upon completion)

**Status**: 🟡 PLANNED

---

## Jira Update Workflow

### When Starting a Task
1. Review ticket purpose and description
2. Understand definition of done
3. Note test plan requirements
4. Comment: "Starting work on this ticket"

### During Development
1. Track files being modified
2. Document decisions made
3. Note any blockers or issues
4. Update ticket if scope changes

### Upon Completion
1. Fill in "Changes" section with all modifications
2. Execute test plan and document results
3. Check off definition of done items
4. Add commit SHAs and links
5. Move ticket to "Done" status
6. Add final comment summarizing work

## Agent Prompt for Jira Updates

When asking an agent to update Jira:

```
Update Jira ticket MST-{number} with the following information:

**Task Summary**: [Brief description of what was done]

**Changes Made**:
[List of files created/modified with descriptions]

**Test Results**:
[Build status, test execution results, manual testing outcomes]

**Definition of Done Status**:
[Checklist of completed items]

Please format this as a Jira comment using the template in docs/JIRA_INTEGRATION.md
```

## Tips for Effective Jira Usage

### Be Specific
❌ "Fixed the bug"
✅ "Fixed renderer crash when drawing null texture by adding null check in renderer_draw_texture()"

### Be Measurable
❌ "Improved performance"
✅ "Reduced frame time from 20ms to 16ms (60 FPS → 60 FPS stable)"

### Link Everything
- Reference file paths
- Link commits
- Cross-reference related tickets
- Include error messages if fixing bugs

### Keep it Updated
- Update as you work, not just at the end
- Document blockers immediately
- Communicate scope changes early

## Example Jira Comment

```markdown
### Update - 2026-01-24

Completed initial project setup. All required infrastructure is in place.

**Build Status**: ✅ Success
- CMake configuration successful
- All dependencies fetched correctly (Raylib 5.0, Google Test 1.14.0)
- Zero compilation errors
- Zero warnings

**Test Status**: ✅ 3/3 Passed
- EngineTests.StateStructSize - PASS
- EngineTests.ConfigStruct - PASS
- RendererTests.RaylibColors - PASS

**Game Status**: ✅ Running
- Window opens correctly (1280x720)
- Displays title and ship placeholder
- FPS counter shows 60 FPS
- ESC key closes application

**Definition of Done**: All items completed ✅

**Commits**:
- [abc1234] Initial project structure and CMake setup
- [def5678] Add engine core and renderer implementation
- [ghi9012] Add game main loop and basic rendering
- [jkl3456] Add unit tests and fix compilation issues

**Next Steps**: Ready to begin MST-3 (Engine Foundation)
```

## Automation Opportunities

Future enhancements could include:
1. Automated Jira updates via API
2. Git hooks to link commits to tickets
3. CI/CD integration to update test status
4. Performance benchmarks auto-reported

For now, manual updates using the template above.
