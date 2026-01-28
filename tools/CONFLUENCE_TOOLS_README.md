# Confluence Tools for MS Tour GDD

This directory contains tools for managing Game Design Documents (GDD) in Confluence.

## Overview

Instead of maintaining GDD documents as local markdown files, we use Confluence as the authoritative source. These tools help create, update, and maintain GDD pages in the MS Confluence space.

## Tools

### Core Tools

- **`confluence_fetch.py`** - Read and search Confluence pages
- **`confluence_push.py`** - Create and update Confluence pages  
- **`confluence_pages.py`** - Centralized registry of GDD page IDs and titles
- **`confluence_update_links.py`** - Add internal page links automatically

### Prerequisites

1. **Authentication**: Credentials in `.env` file at project root
   ```
   ATLASSIAN_EMAIL=your.email@example.com
   ATLASSIAN_API_TOKEN=your_api_token
   ATLASSIAN_BASE_URL=https://o92design.atlassian.net
   CONFLUENCE_SPACE_KEY=MS
   ```

2. **Python packages**: Install required dependencies
   ```bash
   pip install requests beautifulsoup4 html2text markdown2
   ```

## Quick Start

### Test Connection

```bash
cd tools
py confluence_push.py test
```

### View Page Registry

```bash
py confluence_pages.py list
```

Shows all known GDD pages with IDs and URLs.

### Create a New GDD Page

1. **Find parent page ID**:
   ```bash
   py confluence_fetch.py find "GDD Master"
   ```

2. **Create markdown content** in a temporary file:
   ```bash
   echo "# Ship Fuel System" > temp_gdd.md
   echo "" >> temp_gdd.md
   echo "## Overview" >> temp_gdd.md
   echo "Ships consume fuel..." >> temp_gdd.md
   ```

3. **Create page in Confluence**:
   ```bash
   py confluence_push.py create --title "Ship Fuel System" --parent-id 393217 --content-file temp_gdd.md
   ```

4. **Add to page registry** (edit `confluence_pages.py`):
   ```py
   GDD_PAGES = {
       'Ship Fuel System': 1234567,  # <- Add your new page
   }
   ```

5. **Update links**:
   ```bash
   py confluence_update_links.py --title "Ship Fuel System"
   ```

6. **Clean up**:
   ```bash
   del temp_gdd.md
   ```

### Update an Existing Page

1. **Find page**:
   ```bash
   py confluence_fetch.py find "Ship Fuel System"
   ```

2. **Create updated content**:
   ```bash
   echo "# Ship Fuel System" > temp_update.md
   echo "" >> temp_update.md
   echo "## Updated content..." >> temp_update.md
   ```

3. **Preview changes** (dry run):
   ```bash
   py confluence_push.py push temp_update.md --page-id 1234567
   ```

4. **Apply update**:
   ```bash
   py confluence_push.py push temp_update.md --page-id 1234567 --confirm
   ```

5. **Update links** (if needed):
   ```bash
   py confluence_update_links.py 1234567
   ```

6. **Clean up**:
   ```bash
   del temp_update.md
   ```

### Add Internal Links to Pages

Convert text references (like "GDD Master") to Confluence internal links:

```bash
# Single page
py confluence_update_links.py --title "GDD Index"

# Multiple pages
py confluence_update_links.py 1703949 1605636 1671171

# All known pages
py confluence_update_links.py --all

# Dry run (preview changes)
py confluence_update_links.py --all --dry-run
```

## Page Registry (`confluence_pages.py`)

The page registry is a centralized mapping of GDD page titles to Confluence page IDs.

**Why it exists:**
- Enables finding pages by title programmatically
- Used by link updater to create internal links
- Provides single source of truth for page IDs
- Supports page aliases (e.g., "Ship Direct Control" → "Ship Control")

**When to update:**
- After creating any new GDD page in Confluence
- When renaming pages (update title, keep ID)
- When adding page aliases

**How to update:**

Edit `confluence_pages.py` and add your page to `GDD_PAGES`:

```python
GDD_PAGES = {
    'GDD Master': 393217,
    'Your New Page': 1234567,  # <- Add here
}
```

Then commit the change:

```bash
git add tools/confluence_pages.py
git commit -m "Add 'Your New Page' to registry"
```

## Confluence Space Structure

```
GDD Master (393217)
├── GDD Index (1703949)
├── GDD Quick Reference (1605636)
├── GDD Quick Start (1671171)
├── Core Gameplay Loop (622601)
├── Systems (950288)
│   ├── Ship Control (1343506)
│   └── Tourism (1802241)
│       └── Passenger System (1572869)
└── Narrative (1114113)
    └── Three-Act Structure (851973)
```

Maintain this hierarchical structure when creating new pages.

## Command Reference

| Command | Purpose |
|---------|---------|
| `py confluence_push.py test` | Test connection to Confluence |
| `py confluence_pages.py list` | View all registered pages |
| `py confluence_pages.py get "Title"` | Get page ID by title |
| `py confluence_fetch.py find "Title"` | Search for page in Confluence |
| `py confluence_fetch.py get <id>` | Get page content |
| `py confluence_fetch.py children <id>` | List child pages |
| `py confluence_push.py create ...` | Create new page |
| `py confluence_push.py push ...` | Update existing page |
| `py confluence_update_links.py <id>` | Add internal links to page |
| `py confluence_update_links.py --all` | Add links to all pages |

## Workflow Tips

### Creating Multiple Pages

Use a script to batch-create pages:

```python
import confluence_fetch
import confluence_pages

pages_to_create = [
    ("Ship Fuel System", "# Ship Fuel System\n\n..."),
    ("Cargo Management", "# Cargo Management\n\n..."),
]

parent_id = confluence_pages.get_page_id("GDD Master")

for title, markdown in pages_to_create:
    html = markdown_to_confluence_storage(markdown)
    result = confluence_fetch.create_page(title, html, parent_id)
    if result:
        print(f"Created: {title} (ID: {result['id']})")
        # Add to registry
        confluence_pages.add_page(title, int(result['id']))
```

### Migrating from Local Markdown

1. Read local markdown files from `docs/GDD/`
2. Create corresponding Confluence pages
3. Add pages to registry
4. Run link updater on all pages
5. Archive or remove local markdown files

### Maintaining Cross-References

After any content changes:

```bash
# Update links on modified pages
py confluence_update_links.py <page_id>

# Or update all pages (safer)
py confluence_update_links.py --all
```

## Troubleshooting

**Authentication errors:**
- Check `.env` file has correct credentials
- Verify API token is valid and not expired
- Ensure email matches Atlassian account

**Page not found:**
- Check page ID is correct
- Verify page exists in MS space
- Update `confluence_pages.py` if page was renamed

**Link updater not working:**
- Ensure page is in `confluence_pages.py` registry
- Check page titles match exactly (case-sensitive)
- Run with `--dry-run` to preview changes

**"Page already exists" error:**
- Page titles must be unique in the space
- Check if page already exists with `confluence_fetch.py find`
- Update existing page instead of creating new one

## Integration with Agents

The **game-design-creator agent** uses these tools via the **confluence-edit skill**:

1. Agent creates GDD content in Confluence instead of local files
2. Agent updates `confluence_pages.py` after creating pages
3. Agent runs link updater to maintain cross-references
4. All GDD documentation lives in Confluence

See `.github/skills/confluence-edit/SKILL.md` for agent-specific documentation.

## Related Documentation

- **Skill Documentation**: `.github/skills/confluence-edit/SKILL.md`
- **Confluence Audit**: `AUDIT_TOOLS_README.md` (page comparison tools)
- **GDD Master Page**: https://o92design.atlassian.net/wiki/spaces/MS/pages/393217

---

**Last Updated**: 2026-01-28  
**Maintained By**: Documentation Lead
