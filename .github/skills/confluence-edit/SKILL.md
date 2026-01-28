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
- **`confluence_pages.py`**: Centralized registry of GDD page IDs and titles
- **`confluence_update_links.py`**: Automatically add internal page links to existing pages

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

### 6. Update Internal Page Links

Automatically add Confluence internal links to pages that reference other GDD pages:

```bash
cd tools
# Update a single page (dry run first)
py confluence_update_links.py <page_id> --dry-run

# Apply the update
py confluence_update_links.py <page_id>

# Update by page title
py confluence_update_links.py --title "GDD Index"

# Update all known pages in the registry
py confluence_update_links.py --all
```

**How it works:**
- Scans page content for references to other GDD pages (e.g., "GDD Master", "Core Gameplay Loop")
- Converts plain text references to Confluence internal page links
- Uses the centralized page registry in `confluence_pages.py`
- Preserves existing formatting (bold, lists, etc.)

**When to use:**
- After creating new pages that reference existing pages
- After migrating markdown files with cross-references
- When page references are plain text instead of links
- To maintain consistent cross-referencing across all GDD pages

## Python Script Reference

### confluence_pages.py - Page Registry

**Purpose**: Centralized mapping of GDD page titles to Confluence page IDs

**get_page_id(page_title)**: Get page ID by title
```py
page_id = get_page_id("GDD Master")
# Returns: 393217
```

**get_page_url(page_title_or_id)**: Get full Confluence URL
```py
url = get_page_url("GDD Master")
# Returns: https://o92design.atlassian.net/wiki/spaces/MS/pages/393217
```

**get_canonical_title(page_title)**: Resolve aliases to canonical titles
```py
canonical = get_canonical_title("Ship Direct Control")
# Returns: "Ship Control"
```

**get_all_pages()**: Get all known pages including aliases
```py
all_pages = get_all_pages()
# Returns: {"GDD Master": 393217, "Ship Control": 1343506, ...}
```

**Updating the registry**: When you create new GDD pages, add them to `GDD_PAGES` dict in `confluence_pages.py`:
```py
GDD_PAGES = {
    'GDD Master': 393217,
    'Ship Fuel System': 1234567,  # <- Add new pages here
    # ...
}
```

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

### confluence_update_links.py Functions

**add_links_to_html(html_content)**: Add Confluence links to HTML content
```py
updated_html = add_links_to_html(html_content)
# Converts text references to Confluence internal links
```

**update_page_links(page_id, page_title, dry_run)**: Update a single page with links
```py
success = update_page_links(1703949, "GDD Index", dry_run=True)
# Preview changes without applying
```

**update_multiple_pages(page_ids, dry_run)**: Update multiple pages
```py
success_count = update_multiple_pages([1703949, 1605636, 1671171])
# Returns number of successfully updated pages
```

**update_all_known_pages(dry_run)**: Update all pages in registry
```py
success_count = update_all_known_pages(dry_run=False)
# Updates all pages listed in confluence_pages.py
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

6. **Add Internal Links** (recommended after creation)
   ```bash
   py confluence_update_links.py <new_page_id>
   ```
   
   This automatically converts any references to other GDD pages into Confluence internal links.

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

6. **Update Links** (if new page references were added)
   ```bash
   py confluence_update_links.py <page_id>
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

### Page Registry (`confluence_pages.py`)

- Centralized mapping of page titles to IDs
- **Must be updated** when new pages are created
- Used by link updater and other tools
- Edit `GDD_PAGES` dict to add new pages
- Add aliases in `PAGE_ALIASES` for alternative names

**Example update:**
```py
# In confluence_pages.py
GDD_PAGES = {
    'GDD Master': 393217,
    'Ship Fuel System': 1234567,  # <- Add your new page
}
```

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

# View page registry
py confluence_pages.py list

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

# Add internal links to page (dry run)
py confluence_update_links.py <page_id> --dry-run

# Add internal links to page
py confluence_update_links.py <page_id>

# Add links to page by title
py confluence_update_links.py --title "Page Title"

# Add links to all known pages
py confluence_update_links.py --all
```

## Integration with game-design-creator Agent

When the game-design-creator agent uses this skill:

1. **Instead of creating local files in `docs/GDD/`**, create Confluence pages
2. **Instead of editing local markdown**, update Confluence pages
3. **Maintain same structure**, just in Confluence hierarchy
4. **Use temporary files** as intermediary for content transfer
5. **Always verify** page creation/update succeeded before reporting completion
6. **Update page registry** in `confluence_pages.py` after creating new pages
7. **Run link updater** after creating/updating pages with cross-references

The agent should:
- Ask for parent page confirmation before creating new pages
- Show Confluence URLs after page creation
- Handle errors gracefully (auth, network, page not found)
- Clean up temporary files automatically
- Add newly created pages to `confluence_pages.py`
- Run `confluence_update_links.py` to add internal links automatically

### Typical Workflow

1. Create new GDD page in Confluence
2. Add page mapping to `confluence_pages.py`
3. Commit the registry update: `git add tools/confluence_pages.py && git commit -m "Add <Page Name> to registry"`
4. Run link updater on new page: `py confluence_update_links.py --title "<Page Name>"`
5. Optionally update all pages: `py confluence_update_links.py --all`

This ensures all GDD pages have proper cross-references and the registry stays up-to-date.
