# MS Tour - Quick Reference for Copilot & Agents

## 📋 Quick Links
- **Jira**: https://o92design.atlassian.net/browse/MST-
- **Docs**: `docs/` folder
- **Agents**: `docs/AGENTS.md`
- **Context**: `docs/PROJECT_CONTEXT.md`
- **Copilot Rules**: `.github/copilot-instructions.md`

## 🚀 Common Commands

### Build
```bash
cd build
cmake ..                    # Configure (first time)
cmake --build .             # Build project
cmake --build . --clean-first  # Clean build
```

### Run
```bash
.\bin\Debug\MSTour.exe      # Run game
.\tests\Debug\MSTour_tests.exe  # Run tests
```

### Test
```bash
cd build
ctest                       # Run all tests
ctest --verbose            # Verbose output
```

## 🤖 Agent Quick Reference

### When to Use Each Agent

| Task | Agent | Command |
|------|-------|---------|
| Explore code | explore | `task("explore", "Find all input handling code")` |
| Build/test | task | `task("task", "Build and run tests")` |
| Complex work | general-purpose | `task("general-purpose", "Implement ECS")` |
| Code review | code-review | `task("code-review", "Review staged changes")` |

### Agent Delegation Template
```
Use task agent for: [what needs to be done]

Context:
- Current state: [describe current situation]
- Goal: [what we want to achieve]
- Constraints: [any limitations]

Success criteria:
- [ ] Item 1
- [ ] Item 2
```

## 📝 Code Patterns

### Engine Function
```c
// Header: engine/include/engine_system.h
#ifndef ENGINE_SYSTEM_H
#define ENGINE_SYSTEM_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int some_data;
} SystemData;

void system_init(SystemData* data);
void system_update(SystemData* data, double delta_time);
void system_shutdown(SystemData* data);

#ifdef __cplusplus
}
#endif
#endif
```

### Game Code
```c
// game/src/feature.c
#include "engine_core.h"
#include "engine_renderer.h"

void feature_update(double delta_time) {
    // Use engine APIs
    renderer_clear(SKYBLUE);
    renderer_draw_text("Hello", 10, 10, 20, WHITE);
}
```

### Test
```cpp
// tests/system_tests.cpp
TEST(SystemTests, Initialization) {
    SystemData data;
    system_init(&data);
    
    EXPECT_EQ(data.some_data, 0);
    
    system_shutdown(&data);
}
```

## 🎯 Jira Quick Actions

### Update Ticket (MST-X)
```markdown
**Changes**:
- Created: file.c - purpose
- Modified: other.c - what changed

**Tests**: ✅ 3/3 passed
**Build**: ✅ Success
**Status**: Complete
```

### Link Commit
```bash
git commit -m "[MST-X] Brief description

Detailed explanation.

Jira: MST-X"
```

## 🏗️ Project Structure

```
MSTour/
├── engine/          # Engine code (C)
│   ├── include/    # Public headers
│   └── src/        # Implementation
├── game/           # Game code
│   ├── include/    # Game headers
│   └── src/        # Game implementation
├── tests/          # Unit tests (C++)
├── assets/         # Game assets
├── docs/           # Documentation
├── .github/        # Copilot config
└── build/          # Build output (gitignored)
```

## 🔍 Finding Things

### Find Function Definitions
```bash
# Use grep tool
pattern: "function_name"
glob: "*.c"
```

### Find Files
```bash
# Use glob tool
pattern: "**/*input*.h"
```

### Explore Feature
```bash
# Use explore agent
"How does the renderer work?"
"Where is input handling implemented?"
```

## ⚡ Performance Tips

### Data-Oriented Design
```c
// ❌ Bad: Array of Structs (AOS)
typedef struct {
    float x, y, z;
    float vx, vy, vz;
} Entity;
Entity entities[1000];

// ✅ Good: Struct of Arrays (SOA)
typedef struct {
    float x[1000];
    float y[1000];
    float z[1000];
    float vx[1000];
    float vy[1000];
    float vz[1000];
} EntitySystem;
```

### Cache-Friendly
- Process data sequentially
- Keep hot data together
- Avoid pointer chasing
- Batch operations

## 🎨 Style Guide Quick

### Naming
- Functions: `snake_case`
- Types: `PascalCase`
- Variables: `snake_case`
- Constants: `UPPER_SNAKE_CASE`
- Files: `snake_case`

### Formatting
- Indent: 4 spaces
- Braces: K&R style
- Max line: ~100 characters
- Comments: `//` for single line, `/* */` for blocks

## 🐛 Debugging

### Build Errors
1. Check CMake configuration
2. Verify dependencies
3. Clean and rebuild
4. Check error logs

### Runtime Errors
1. Run in Debug mode
2. Check console output
3. Use debugger (VS, gdb)
4. Add logging

### Test Failures
1. Run specific test: `--gtest_filter=TestName.*`
2. Enable verbose: `--verbose`
3. Check test logs
4. Debug in IDE

## 📚 Documentation Hierarchy

1. **README.md** - Start here, project overview
2. **QUICKSTART.md** - Get running fast
3. **BUILD.md** - Detailed build instructions
4. **PROJECT_CONTEXT.md** - Deep dive into architecture
5. **AGENTS.md** - Agent definitions and workflows
6. **JIRA_INTEGRATION.md** - Jira ticket management
7. **.github/copilot-instructions.md** - Copilot guidelines

## 🔗 External Resources

- [Raylib Docs](https://www.raylib.com/)
- [CMake Docs](https://cmake.org/documentation/)
- [Google Test](https://google.github.io/googletest/)
- [Data-Oriented Design](https://www.dataorienteddesign.com/dodbook/)

## ⚠️ Common Pitfalls

### DON'T
- ❌ Create types that duplicate Raylib (Color, Vector2, etc.)
- ❌ Use C++ classes in engine code
- ❌ Optimize without profiling
- ❌ Break the build
- ❌ Skip tests

### DO
- ✅ Use Raylib types directly
- ✅ Write data-oriented C code
- ✅ Profile before optimizing
- ✅ Keep builds green
- ✅ Write tests

## 🎓 Learning Path

### Day 1: Setup
1. Read README.md
2. Build project
3. Run game and tests
4. Explore code structure

### Day 2: Engine
1. Read engine headers
2. Understand core systems
3. Study renderer implementation
4. Try modifying something simple

### Day 3: Game
1. Examine game loop
2. Understand game/engine separation
3. Add a simple feature
4. Write a test

### Day 4: Advanced
1. Study data-oriented patterns
2. Optimize something
3. Add a complex feature
4. Document your work

## 💡 Pro Tips

### For Copilot
- Reference docs in comments: "See docs/AGENTS.md"
- Be specific in comments: "TODO MST-X: Implement input handling"
- Use clear variable names
- Follow established patterns

### For Agents
- Give clear context
- Specify success criteria
- Provide examples
- Ask for verification

### For Jira
- Update as you work
- Be specific and measurable
- Link commits
- Document decisions

## 🚨 Emergency Procedures

### Build Broken
```bash
# Nuclear option
rm -rf build
mkdir build
cd build
cmake ..
cmake --build .
```

### Git Messed Up
```bash
# Stash changes
git stash

# Check status
git status

# Restore if needed
git stash pop
```

### Everything on Fire
1. Don't panic
2. Check recent commits: `git log`
3. Consider reverting: `git revert <commit>`
4. Ask for help (use explore agent)
5. Document what happened

---

**Remember**: When in doubt, check the docs folder or ask an agent!
