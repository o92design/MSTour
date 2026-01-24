# Custom Agents for MS Tour Project

This document defines specialized agents for delegating tasks within the MS Tour project.

## Available Agents

### 1. Engine Agent
**Name**: `engine-agent`  
**Specialization**: Game engine development in C with data-oriented design  
**Use for**:
- Implementing core engine systems (window, input, timing)
- Renderer enhancements (camera, 3D models, sprites)
- ECS (Entity Component System) implementation
- Memory management and performance optimization
- Low-level systems programming

**Example prompts**:
- "Implement an input handling system with keyboard and mouse support"
- "Create a camera system with orthographic projection"
- "Add 3D model loading using Raylib"

---

### 2. Game Logic Agent
**Name**: `game-agent`  
**Specialization**: Game-specific features and gameplay logic  
**Use for**:
- Ship management systems
- Fleet operations and AI
- Map/world navigation
- UI implementation
- Game state management
- Gameplay mechanics

**Example prompts**:
- "Implement ship movement and pathfinding"
- "Create a route planning system for ships"
- "Add UI for fleet management"

---

### 3. Testing Agent
**Name**: `test-agent`  
**Specialization**: Unit testing and test infrastructure  
**Use for**:
- Writing unit tests for engine components
- Testing game logic
- Test utilities and fixtures
- Integration testing
- Performance benchmarking

**Example prompts**:
- "Write tests for the input handling system"
- "Create test fixtures for ship data"
- "Add integration tests for renderer"

---

### 4. Documentation Agent
**Name**: `docs-agent`  
**Specialization**: Technical writing and documentation  
**Use for**:
- API documentation
- Architecture diagrams
- Code style guides
- Build instructions
- User documentation

**Example prompts**:
- "Document the ECS architecture"
- "Create API reference for renderer"
- "Write user guide for building the project"

---

### 5. Jira Integration Agent
**Name**: `jira-agent`  
**Specialization**: Jira ticket management and updates  
**Use for**:
- Updating Jira tickets with progress
- Documenting changes made
- Tracking test plans
- Marking tasks as complete

**Project Details**:
- **Project Key**: MST
- **Base URL**: https://o92design.atlassian.net/browse/
- **Ticket Format**: MST-{number}

**Ticket Fields to Update**:
- **Purpose**: Why this task exists
- **Description**: What will be implemented
- **Definition of Done**: Completion criteria
- **Test Plan**: How to verify it works
- **Changes**: Files modified/created

**Example prompts**:
- "Update MST-2 with project setup completion details"
- "Create summary for MST-3 about engine implementation"
- "Generate test plan for current task"

**Template for Updates**:
```markdown
### Changes
- File: path/to/file.c
  - Added: function_name() - description
  - Modified: existing_function() - changes made

### Test Plan
1. Build the project: `cmake --build build`
2. Run tests: `./build/tests/Debug/MSTour_tests.exe`
3. Expected: All tests pass
4. Run game: `./build/bin/Debug/MSTour.exe`
5. Expected: Window opens and displays correctly

### Definition of Done
- [x] Code compiles without errors
- [x] All tests pass
- [x] Documentation updated
- [x] Changes committed to git
```

---

### 6. Build & CI Agent
**Name**: `build-agent`  
**Specialization**: Build system and continuous integration  
**Use for**:
- CMake configuration
- Fixing build errors
- Dependency management
- Cross-platform build issues
- CI/CD pipeline setup

**Example prompts**:
- "Fix CMake configuration for new module"
- "Add new dependency to build system"
- "Debug compilation errors"

---

## Agent Usage Guidelines

### When to delegate:
1. **Complex subsystems**: Delegate to specialized agents (engine, game)
2. **Testing**: Always use test-agent for comprehensive test coverage
3. **Documentation**: Use docs-agent for formal documentation
4. **Jira updates**: Use jira-agent after completing tasks

### How to delegate:
```
Use the task tool with appropriate agent_type:
- agent_type: "explore" - For code exploration and questions
- agent_type: "task" - For command execution (builds, tests)
- agent_type: "general-purpose" - For complex multi-step tasks
```

### Task Completion Workflow:
1. **Implement** feature with appropriate agent
2. **Test** with test-agent
3. **Document** with docs-agent (if needed)
4. **Update Jira** with jira-agent
5. **Commit** changes to git

---

## Agent Communication Protocol

### Input Format:
```markdown
**Task**: [Clear description of what needs to be done]
**Context**: [Relevant background information]
**Constraints**: [Any limitations or requirements]
**Success Criteria**: [How to verify completion]
```

### Output Format:
```markdown
**Status**: [Complete/In Progress/Blocked]
**Changes**: [List of files modified/created]
**Next Steps**: [What should happen next]
**Jira**: [Ticket number if applicable]
```

---

## Example Delegation Flows

### Implementing a New Feature
1. **Planning**: Main agent creates implementation plan
2. **Engine Work**: Delegate to engine-agent
3. **Game Integration**: Delegate to game-agent
4. **Testing**: Delegate to test-agent
5. **Documentation**: Delegate to docs-agent
6. **Jira Update**: Delegate to jira-agent

### Fixing a Bug
1. **Investigation**: Use explore agent to find issue
2. **Fix**: Appropriate agent (engine/game)
3. **Verification**: test-agent runs tests
4. **Update**: jira-agent updates ticket

### Adding Tests
1. **Identify Coverage Gaps**: test-agent analyzes
2. **Write Tests**: test-agent implements
3. **Verify**: test-agent runs suite
4. **Document**: Add to test plan

---

## Jira Ticket Lifecycle

### MST-2: Project Setup (COMPLETE)
- Created project structure
- Setup build system
- Initial engine and game code
- Status: ✅ Done

### MST-3: Engine Foundation (IN PROGRESS)
- Planned Phase 2 implementation
- Core systems, rendering, ECS, resources
- Status: 🟡 Planned

### Creating New Tickets
When creating work items:
1. Clear purpose statement
2. Detailed description
3. Measurable definition of done
4. Concrete test plan
5. Link dependencies

---

## Best Practices

1. **Single Responsibility**: Each agent focuses on its domain
2. **Clear Handoffs**: Provide complete context when delegating
3. **Verify Work**: Always test before marking complete
4. **Document Changes**: Update Jira with details
5. **Commit Frequently**: Small, atomic commits with good messages
