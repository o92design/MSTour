# MS Tour Documentation Index

Welcome to the MS Tour documentation! This guide will help you navigate all available documentation.

## 📚 Quick Navigation

### Getting Started
1. **[QUICKSTART.md](QUICKSTART.md)** - Get up and running in 5 minutes
2. **[BUILD.md](BUILD.md)** - Detailed build instructions for all platforms
3. **[../README.md](../README.md)** - Project overview and introduction

### Development
4. **[PROJECT_CONTEXT.md](PROJECT_CONTEXT.md)** - Deep dive into architecture, philosophy, and design decisions
5. **[COPILOT_QUICKREF.md](COPILOT_QUICKREF.md)** - Quick reference for daily development
6. **[AGENTS.md](AGENTS.md)** - Custom agent definitions for task delegation

### Game Design Documentation
7. **Confluence Space "MS"** - Active GDD location (primary)
8. **[GDD/](GDD/)** - Archived GDD documentation (historical reference only)

### Workflow & Process
9. **[JIRA_INTEGRATION.md](JIRA_INTEGRATION.md)** - Jira ticket management and workflows
10. **[../.github/copilot-instructions.md](../.github/copilot-instructions.md)** - GitHub Copilot configuration

### Project History
11. **[SETUP_SUMMARY.md](SETUP_SUMMARY.md)** - Phase 1 complete setup summary
12. **[jira_updates/](jira_updates/)** - Jira ticket update archives

---

## 📖 Documentation Overview

### QUICKSTART.md
**Purpose**: Get the project built and running immediately  
**Audience**: New developers, quick reference  
**Contents**: Build commands, run instructions, troubleshooting

### BUILD.md
**Purpose**: Comprehensive build guide  
**Audience**: All developers, build system maintainers  
**Contents**: Prerequisites, detailed build steps for Windows/Linux, IDE integration, common issues

### PROJECT_CONTEXT.md
**Purpose**: Understand the "why" behind MS Tour  
**Audience**: All team members, new developers  
**Contents**: Game vision, architecture principles, data-oriented design, development workflow

### COPILOT_QUICKREF.md
**Purpose**: Daily development reference  
**Audience**: All developers using Copilot  
**Contents**: Common commands, code patterns, agent usage, style guide, tips and tricks

### AGENTS.md
**Purpose**: Define and use custom agents for task delegation  
**Audience**: All developers, team leads  
**Contents**: 6 agent definitions (engine, game, test, docs, jira, build), workflows, examples

### JIRA_INTEGRATION.md
**Purpose**: Manage Jira tickets effectively  
**Audience**: All team members  
**Contents**: Ticket structure, update templates, workflows, MST-2 and MST-3 details

### GDD (Archived)
**Purpose**: Historical game design documentation  
**Audience**: Reference only - see Confluence for active docs  
**Contents**: Legacy GDD content (pre-January 2026), see [GDD/GDD_README.md](GDD/GDD_README.md)  
**Active Location**: Confluence Space "MS" → "GDD Master"

### SETUP_SUMMARY.md
**Purpose**: Document Phase 1 completion  
**Audience**: Team leads, project managers, new developers  
**Contents**: Complete setup details, files created, test results, next steps

---

## 🎯 Use Cases

### "I'm new to the project"
1. Read [../README.md](../README.md) - Overview
2. Follow [QUICKSTART.md](QUICKSTART.md) - Get it running
3. Read [PROJECT_CONTEXT.md](PROJECT_CONTEXT.md) - Understand architecture
4. Bookmark [COPILOT_QUICKREF.md](COPILOT_QUICKREF.md) - Daily reference

### "I need to build the project"
1. Check [QUICKSTART.md](QUICKSTART.md) - Quick commands
2. If issues, see [BUILD.md](BUILD.md) - Detailed guide
3. Still stuck? Check [COPILOT_QUICKREF.md](COPILOT_QUICKREF.md) - Troubleshooting

### "I'm implementing a feature"
1. Check [AGENTS.md](AGENTS.md) - Delegate to specialized agents
2. Refer to Confluence GDD - Design specifications
3. Use [COPILOT_QUICKREF.md](COPILOT_QUICKREF.md) - Code patterns
4. Refer to [PROJECT_CONTEXT.md](PROJECT_CONTEXT.md) - Design principles
5. Update [JIRA_INTEGRATION.md](JIRA_INTEGRATION.md) - Track progress

### "I'm updating Jira"
1. Open [JIRA_INTEGRATION.md](JIRA_INTEGRATION.md) - Templates and workflows
2. See [jira_updates/MST-2_update.md](jira_updates/MST-2_update.md) - Example update
3. Follow the template for your ticket

### "I'm setting up Copilot"
1. Read [../.github/copilot-instructions.md](../.github/copilot-instructions.md) - Copilot config
2. Review [PROJECT_CONTEXT.md](PROJECT_CONTEXT.md) - Project context
3. Use [COPILOT_QUICKREF.md](COPILOT_QUICKREF.md) - Quick reference

---

## 📝 Documentation Philosophy

### Principles
- **Layered**: Start simple (QUICKSTART), go deep (PROJECT_CONTEXT)
- **Practical**: Real examples, not just theory
- **Up-to-date**: Update docs when code changes
- **Searchable**: Clear headings, good structure

### Maintenance
- Update relevant docs with each PR
- Keep examples working
- Archive old content in history/
- Link related docs

### Style
- Use clear headings (##, ###)
- Include code examples with syntax highlighting
- Add ✅ ❌ emojis for clarity
- Link to other docs liberally

---

## 🔗 External Resources

### Raylib
- [Official Docs](https://www.raylib.com/)
- [Cheatsheet](https://www.raylib.com/cheatsheet/cheatsheet.html)
- [Examples](https://www.raylib.com/examples.html)

### CMake
- [Documentation](https://cmake.org/documentation/)
- [Tutorial](https://cmake.org/cmake/help/latest/guide/tutorial/index.html)

### Google Test
- [Primer](https://google.github.io/googletest/primer.html)
- [Advanced Guide](https://google.github.io/googletest/advanced.html)

### Data-Oriented Design
- [DOD Book](https://www.dataorienteddesign.com/dodbook/)
- [Mike Acton's Talk](https://www.youtube.com/watch?v=rX0ItVEVjHc)

---

## 🚀 Contributing to Docs

### When to Update Docs
- Adding new features → Update PROJECT_CONTEXT, COPILOT_QUICKREF
- Fixing build issues → Update BUILD.md
- New workflow → Update JIRA_INTEGRATION
- New agent → Update AGENTS.md

### How to Update
1. Edit the relevant .md file
2. Follow existing style/structure
3. Add examples where helpful
4. Link to related docs
5. Update this index if adding new docs

### Documentation Tasks
- [ ] Add Linux build instructions (BUILD.md)
- [ ] Create ARCHITECTURE.md (detailed diagrams)
- [ ] Add CONTRIBUTING.md (contribution guidelines)
- [ ] Create API reference docs (engine API)
- [ ] Add performance benchmarks doc

---

## 📂 File Structure

```
docs/
├── README.md (this file)        # Documentation index
├── QUICKSTART.md                # Quick start guide
├── BUILD.md                     # Build instructions
├── PROJECT_CONTEXT.md           # Architecture and philosophy
├── COPILOT_QUICKREF.md          # Daily reference
├── AGENTS.md                    # Agent definitions
├── JIRA_INTEGRATION.md          # Jira workflows
├── SETUP_SUMMARY.md             # Phase 1 summary
├── GDD/                         # Archived game design docs (see Confluence for active)
└── jira_updates/                # Jira update archives
    └── MST-2_update.md          # MST-2 completion

../.github/
├── copilot-instructions.md      # GitHub Copilot config
├── agents/                      # Custom agent definitions
│   └── game-design-creator.agent.md  # GDD documentation agent
└── skills/                      # Custom skills
    └── confluence-edit/         # Confluence integration skill
        └── SKILL.md
```

---

## ❓ FAQ

**Q: Which doc should I read first?**  
A: Start with [QUICKSTART.md](QUICKSTART.md) to get running, then [PROJECT_CONTEXT.md](PROJECT_CONTEXT.md) to understand the project.

**Q: Where are the code examples?**  
A: [COPILOT_QUICKREF.md](COPILOT_QUICKREF.md) has patterns, [PROJECT_CONTEXT.md](PROJECT_CONTEXT.md) has architecture examples.

**Q: How do I use agents?**  
A: Read [AGENTS.md](AGENTS.md) for complete guide with examples.

**Q: Where's the Game Design Document?**  
A: Active GDD is in Confluence Space "MS" → "GDD Master". The [GDD/](GDD/) folder contains archived historical docs.

**Q: How do I create GDD documentation?**  
A: Use the game-design-creator agent with the confluence-edit skill. See [AGENTS.md](AGENTS.md) for details.

**Q: Where's the API reference?**  
A: Currently in header files (engine/include/*.h). API reference doc is planned.

**Q: How do I update Jira?**  
A: Use templates in [JIRA_INTEGRATION.md](JIRA_INTEGRATION.md), see example in jira_updates/.

**Q: Where are build troubleshooting steps?**  
A: [BUILD.md](BUILD.md) has detailed troubleshooting, [COPILOT_QUICKREF.md](COPILOT_QUICKREF.md) has emergency procedures.

---

## 📈 Documentation Status

| Document | Status | Last Updated | Completeness |
|----------|--------|--------------|--------------|
| README (root) | ✅ Complete | 2026-01-24 | 100% |
| QUICKSTART | ✅ Complete | 2026-01-24 | 100% |
| BUILD | ✅ Complete | 2026-01-24 | 95% (Linux pending) |
| PROJECT_CONTEXT | ✅ Complete | 2026-01-24 | 100% |
| COPILOT_QUICKREF | ✅ Complete | 2026-01-24 | 100% |
| AGENTS | ✅ Complete | 2026-01-24 | 100% |
| JIRA_INTEGRATION | ✅ Complete | 2026-01-24 | 100% |
| SETUP_SUMMARY | ✅ Complete | 2026-01-24 | 100% |
| GDD | 📦 Archived | 2026-01-26 | See Confluence |

---

**Last Updated**: 2026-01-26  
**Maintained By**: MS Tour Development Team  
**Questions?**: Check relevant doc or ask in team chat
