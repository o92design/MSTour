---
name: confluence-edit
description: Guide for creating and updating game design documentation in Confluence. Use this when asked to create or update GDD pages in Confluence.
---

# Confluence Edit Skill

This skill enables you to create and update Game Design Document (GDD) pages in Confluence instead of local markdown files.

## Prerequisites

Before using this skill, ensure:
1. Authentication credentials are loaded from `.env` file
2. Required environment variables: `ATLASSIAN_EMAIL`, `ATLASSIAN_API_TOKEN`, `CONFLUENCE_SPACE_KEY`
3. Confluence space key defaults to "MS"
4. Base URL defaults to "https://o92design.atlassian.net"

## Available Tools

Located in `tools/` directory:
- **`confluence_fetch.py`**: Read and search Confluence pages
- **`confluence_push.py`**: Create and update Confluence pages

## Confluence Space Structure

Organize GDD pages in the following hierarchy:

```
GDD Master (Root/Parent Page)
├── Systems
│   ├── Ship Management System
│   ├── Route Planning System
│   └── Weather System
├── Features
│   ├── Ship Fuel System
│   ├── Cargo Management
│   └── Fleet Management
├── Content
│   ├── Islands
│   ├── Cargo Types
│   └── Events
├── Balance
│   ├── Economy Tuning
│   ├── Progression Curves
│   └── Difficulty Parameters
└── UX
    ├── Main Menu Flow
    ├── Route Planning Interface
    └── Fleet Overview
```

## Operations

### 1. Check Connection

Test Confluence connection before operations:

```bash
cd tools
py confluence_push.py test
```

### 2. Find Existing Pages

Search for a page by title:

```bash
cd tools
py confluence_fetch.py find "GDD Master"
```

This returns the page ID needed for creating child pages.

### 3. Create a New Page

To create a new GDD page as a child of a parent page:

```bash
cd tools
py confluence_push.py create --title "Ship Fuel System" --parent-id <parent_page_id> --content-file <temp_markdown_file>
```

**Workflow:**
1. Create a temporary markdown file with the GDD content
2. Use `confluence_push.py create` to create the page
3. Delete the temporary file after creation

**Example:**
```bash
# Create temp file
echo "# Ship Fuel System\n\n## Overview\n..." > temp_gdd.md

# Create Confluence page
py confluence_push.py create --title "Ship Fuel System" --parent-id 123456 --content-file temp_gdd.md

# Clean up
del temp_gdd.md
```

### 4. Update an Existing Page

To update content on an existing page:

```bash
cd tools
# First, find the page ID
py confluence_fetch.py find "Ship Fuel System"

# Then update it
py confluence_push.py push temp_update.md --page-id <page_id> --confirm
```

**Workflow:**
1. Create a temporary markdown file with updated content
2. Use `confluence_push.py push` to update the page
3. Delete the temporary file

### 5. Read Page Content

To read existing Confluence page content (useful for updates):

```bash
cd tools
py confluence_fetch.py get <page_id_or_title>
```

## Python Script Reference

### confluence_fetch.py Functions

**get_page(page_id)**: Fetch page details
```py
page = get_page("123456")
# Returns: {title, version, body, etc.}
```

**find_page_by_title(title)**: Search by title
```py
page = find_page_by_title("Ship Fuel System")
# Returns: {id, title, version}
```

**create_page(title, content, parent_id)**: Create new page
```py
result = create_page("New Feature", html_content, "123456")
# content must be in Confluence storage format (HTML)
```

**update_page(page_id, title, content, current_version)**: Update existing page
```py
result = update_page("789", "Feature Name", html_content, 5)
# Increments version automatically
```

### confluence_push.py Functions

**markdown_to_confluence_storage(markdown_text)**: Convert markdown to HTML
```py
html = markdown_to_confluence_storage("# Title\n\nContent...")
# Returns HTML in Confluence storage format
```

## Workflow for Game Design Documentation

### Creating a New GDD Section

1. **Determine Parent Page**
   ```bash
   py confluence_fetch.py find "GDD Master"
   # Note the page ID (e.g., 123456)
   ```

2. **Create Content**
   - Write markdown content following GDD templates
   - Save to temporary file: `temp_gdd_content.md`

3. **Create Page**
   ```bash
   py confluence_push.py create --title "New System Design" --parent-id 123456 --content-file temp_gdd_content.md
   ```

4. **Verify**
   - Check output for page URL
   - Verify page appears in Confluence under correct parent

5. **Clean Up**
   ```bash
   del temp_gdd_content.md
   ```

### Updating Existing GDD Content

1. **Find Page**
   ```bash
   py confluence_fetch.py find "Ship Fuel System"
   # Note the page ID
   ```

2. **Prepare Update**
   - Write updated markdown content
   - Save to temporary file: `temp_update.md`

3. **Preview Changes (Dry Run)**
   ```bash
   py confluence_push.py push temp_update.md --page-id <page_id>
   # Shows preview without updating
   ```

4. **Apply Update**
   ```bash
   py confluence_push.py push temp_update.md --page-id <page_id> --confirm
   ```

5. **Clean Up**
   ```bash
   del temp_update.md
   ```

## Important Notes

### Markdown to Confluence Conversion

- Markdown is automatically converted to Confluence storage format (HTML)
- Supported: headings, lists, tables, code blocks, links
- Complex formatting may need manual adjustment in Confluence UI

### Page Hierarchy

- Always specify parent page when creating new pages
- Root GDD page: "GDD Master"
- Maintain consistent structure matching the file-based organization

### Temporary Files

- Use temporary markdown files for content transfer
- Always clean up temp files after operations
- Naming convention: `temp_*.md`

### Version Control

- Confluence tracks page versions automatically
- Each update increments version number
- Cannot overwrite without current version number

### Error Handling

- Always test connection first
- Verify parent page exists before creating children
- Check page exists before updating
- Handle authentication errors gracefully

## Command Reference Quick Sheet

```bash
# Navigate to tools directory
cd tools

# Test connection
py confluence_push.py test

# Find page by title
py confluence_fetch.py find "Page Title"

# Get page content
py confluence_fetch.py get <page_id_or_title>

# Create new page
py confluence_push.py create --title "Title" --parent-id <id> --content-file <file>

# Update page (dry run)
py confluence_push.py push <file> --page-id <id>

# Update page (confirm)
py confluence_push.py push <file> --page-id <id> --confirm
```

## Integration with game-design-creator Agent

When the game-design-creator agent uses this skill:

1. **Instead of creating local files in `docs/GDD/`**, create Confluence pages
2. **Instead of editing local markdown**, update Confluence pages
3. **Maintain same structure**, just in Confluence hierarchy
4. **Use temporary files** as intermediary for content transfer
5. **Always verify** page creation/update succeeded before reporting completion

The agent should:
- Ask for parent page confirmation before creating new pages
- Show Confluence URLs after page creation
- Handle errors gracefully (auth, network, page not found)
- Clean up temporary files automatically
