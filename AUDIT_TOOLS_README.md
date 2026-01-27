# Confluence DRY Audit Tools

This directory contains tools for auditing Confluence documentation and ensuring DRY (Don't Repeat Yourself) principles are followed.

## Quick Start

### Compare Two Pages for Redundancy

**Windows:**
```bash
audit_pages.bat "Page Title 1" "Page Title 2"
```

**Cross-platform (Python):**
```bash
python confluence_compare.py "Page Title 1" "Page Title 2"
```

### Example

```bash
audit_pages.bat "GDD Master" "Core Gameplay Loop"
```

This will:
1. ✅ Fetch both Confluence pages
2. ✅ Analyze content overlap
3. ✅ Generate detailed comparison report
4. ✅ Save outputs to `audit_output/`

## Tools Overview

### `audit_pages.bat` (Windows)
- User-friendly batch script for Windows users
- Handles full audit workflow
- Saves timestamped reports to `audit_output/`
- Automatically opens report when complete

### `confluence_compare.py` (Cross-platform)
- Python script for direct page comparison
- Calculates similarity percentages
- Identifies duplicate sections
- Provides actionable recommendations

### `tools/confluence_audit.py`
- Low-level audit implementation
- Called by higher-level scripts
- Detailed section-by-section analysis

### `tools/confluence_fetch.py`
- Confluence API wrapper
- Commands: get, find, list, children
- Used by all audit tools

### `tools/confluence_push.py`
- Push markdown content to Confluence
- Batch operations for multiple pages
- Test mode and live mode

## Output Files

After running an audit, you'll find:

```
audit_output/
  ├── Page_Title_1.md          # Fetched content from first page
  ├── Page_Title_2.md          # Fetched content from second page
  └── audit_comparison_[timestamp].txt  # Detailed comparison report
```

## Interpreting Results

### Similarity Percentages
- **>70%**: High redundancy - consolidate immediately
- **40-70%**: Moderate redundancy - review and reduce
- **<40%**: Low redundancy - content is well-separated

### Common Recommendations

**High Overlap (>70%):**
1. Choose one page as "single source of truth"
2. Move ALL details to that page
3. Reduce other page to overview + link
4. Target: <10% overlap

**Moderate Overlap (40-70%):**
1. Identify truly duplicated sections
2. Decide ownership for each concept
3. Use cross-references, not duplication
4. Keep master docs as overviews only

## Common Use Cases

### 1. Audit Master Document vs Child Page
```bash
audit_pages.bat "GDD Master" "Ship Management System"
```

### 2. Check Related System Documents
```bash
audit_pages.bat "Fleet Management" "Route Planning System"
```

### 3. Verify Consolidation Success
```bash
# After consolidating, re-run audit to verify <10% overlap
audit_pages.bat "GDD Master" "Core Gameplay Loop"
```

## Best Practices

### When to Run Audits
- ✅ After creating new GDD sections
- ✅ Before major documentation updates
- ✅ Monthly review of master documents
- ✅ When pages feel "too similar"

### DRY Principles for GDD
1. **Master documents** = Overview + links only
2. **Detail pages** = Complete specifications
3. **Cross-reference** instead of duplicate
4. **Single source of truth** for each concept
5. **Target <10%** overlap between related pages

### Confluence Hierarchy
```
GDD Master (Master)
├── Core Gameplay Loop (Child - single source)
├── Ship Management System (Child - single source)
├── Route Planning System (Child - single source)
└── ...
```

## Troubleshooting

### "Could not fetch page"
- Check page title spelling (case-sensitive)
- Verify Confluence credentials in `.env`
- Ensure page exists in MS space

### Unicode/Encoding Errors
- Use `audit_pages.bat` on Windows (handles encoding)
- Or run Python with: `python -X utf8 confluence_compare.py ...`

### Empty Output Files
- Check Confluence API credentials
- Verify network connectivity
- Try: `python tools/confluence_fetch.py test`

## Configuration

Confluence settings in `.env` file:
```
ATLASSIAN_EMAIL=your.email@example.com
ATLASSIAN_API_TOKEN=your_api_token_here
ATLASSIAN_BASE_URL=https://yoursite.atlassian.net
CONFLUENCE_SPACE_KEY=MS
```

## Future Enhancements

Potential additions:
- [ ] GUI for selecting pages to compare
- [ ] Bulk audit of all pages in space
- [ ] Historical tracking of overlap reduction
- [ ] Automated DRY suggestions
- [ ] Integration with CI/CD for doc validation

---

**Questions?** See individual tool help:
```bash
audit_pages.bat
python confluence_compare.py
python tools/confluence_audit.py
```
