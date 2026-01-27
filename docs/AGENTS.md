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

### 5. Game Design Consultant Agent
**Name**: `game-design-consultant`  
**Specialization**: Interactive game design discussion and exploration  
**Use for**:
- Exploring game mechanics and systems ideas
- Evaluating design decisions
- Applying game design principles (Schell, Koster, Adams)
- Discussing player experience and engagement
- Analyzing design trade-offs
- Content design (islands, routes, cargo, events)
- Balance and progression discussions

**Design Philosophy**:
- Inspired by Jesse Schell's "The Art of Game Design: A Book of Lenses"
- Raph Koster's "A Theory of Fun for Game Design"
- Ernest Adams' "Fundamentals of Game Design"

**Example prompts**:
- "Let's explore how ship fuel mechanics could create meaningful choices"
- "Evaluate the progression curve for unlocking new islands"
- "What are the core patterns players should learn in MS Tour?"
- "How can we balance challenge and accessibility in route planning?"

---

### 6. Game Design Creator Agent
**Name**: `game-design-creator`  
**Specialization**: Formal game design documentation and specifications in Confluence  
**Use for**:
- Creating Game Design Documents (GDDs) in Confluence
- Writing feature specifications as Confluence pages
- Documenting game systems formally
- Creating balance and tuning documents
- Writing content specifications
- UI/UX specifications
- Maintaining design documentation in Confluence

**Document Location**: Confluence Space "MS" → "MS Tour - Game Design Document"  
**Legacy Docs**: `docs/GDD/` (archived, read-only)

**Document Types**:
- Feature specifications
- System design documents
- Balance sheets and formulas
- Content specifications (islands, cargo types, events)
- Player experience maps
- UI/UX specifications

**Example prompts**:
- "Create a Confluence page for the cargo trading system specification"
- "Document the ship upgrade progression in Confluence with formulas and values"
- "Write a feature spec page for weather effects on shipping routes"
- "Create a balance document in Confluence for the game economy"

**Workflow with Consultant**:
1. Use `game-design-consultant` to explore and discuss ideas
2. Use `game-design-creator` to formalize and document decisions in Confluence
3. Creator produces implementation-ready specifications as Confluence pages

---

### 7. Jira Integration Agent
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

### 8. Build & CI Agent
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
1. **Game Design**: Use game-design-consultant for exploration, game-design-creator for documentation
2. **Complex subsystems**: Delegate to specialized agents (engine, game)
3. **Testing**: Always use test-agent for comprehensive test coverage
4. **Documentation**: Use docs-agent for technical/code documentation
5. **Jira updates**: Use jira-agent after completing tasks

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

### Designing and Implementing a New Feature
1. **Design Exploration**: Delegate to game-design-consultant (explore mechanics, balance, player experience)
2. **Design Documentation**: Delegate to game-design-creator (formal specifications)
3. **Planning**: Main agent creates implementation plan
4. **Engine Work**: Delegate to engine-agent (if needed)
5. **Game Integration**: Delegate to game-agent
6. **Testing**: Delegate to test-agent
7. **Documentation**: Delegate to docs-agent (technical/code docs)
8. **Jira Update**: Delegate to jira-agent

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

### Game Design Workflow
1. **Explore Ideas**: game-design-consultant discusses mechanics, balance, player experience
2. **Make Decisions**: Collaborate with consultant to evaluate options
3. **Document Design**: game-design-creator formalizes specifications in Confluence
4. **Review & Iterate**: Refine design based on review
5. **Implementation**: Pass Confluence specs to game-agent/engine-agent
6. **Playtest & Balance**: Test and iterate using consultant feedback

**Agent Separation**:
- **game-design-consultant**: Explores "what should we design?"
- **game-design-creator**: Documents "here's what we decided" (in Confluence)
- **game-agent**: Implements "here's how it works in code"
- **docs-agent**: Documents "here's how to use the code"

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
