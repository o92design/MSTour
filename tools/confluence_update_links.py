#!/usr/bin/env python3
"""
Confluence Link Updater
Automatically adds Confluence internal page links to existing pages
Uses the centralized confluence_pages.py registry
"""

import sys
import re
from pathlib import Path

# Add tools directory to path
sys.path.insert(0, str(Path(__file__).parent))

import confluence_fetch
import confluence_pages

def create_confluence_link(page_title):
    """Create Confluence storage format internal link"""
    return f'<ac:link><ri:page ri:content-title="{page_title}"/></ac:link>'

def add_links_to_html(html_content):
    """
    Add Confluence internal page links to HTML content
    Uses the centralized page registry from confluence_pages.py
    
    Args:
        html_content: HTML content in Confluence storage format
    
    Returns:
        Updated HTML content with Confluence links
    """
    # Get all known pages (including aliases)
    all_pages = confluence_pages.get_all_pages()
    
    # Build patterns for each known page
    patterns = []
    
    for title in all_pages.keys():
        # Get canonical title (resolve aliases)
        canonical_title = confluence_pages.get_canonical_title(title)
        
        # Pattern for bold text: <strong>Title</strong>
        patterns.append((
            rf'<strong>({re.escape(title)})</strong>',
            lambda m, t=canonical_title: f'<strong>{create_confluence_link(t)}</strong>'
        ))
        
        # Pattern for plain text in element content: >Title<
        # Be careful not to replace within attributes or existing links
        patterns.append((
            rf'>({re.escape(title)})<',
            lambda m, t=canonical_title: f'>{create_confluence_link(t)}<'
        ))
    
    # Apply replacements
    for pattern, replacement in patterns:
        html_content = re.sub(pattern, replacement, html_content)
    
    return html_content

def update_page_links(page_id, page_title=None, dry_run=False):
    """
    Update a Confluence page to add internal links
    
    Args:
        page_id: Confluence page ID
        page_title: Optional page title (fetched if not provided)
        dry_run: If True, show changes without updating
    
    Returns:
        True if successful, False otherwise
    """
    # Get current page
    page = confluence_fetch.get_page(page_id)
    if not page:
        print(f"[ERROR] Failed to fetch page with ID: {page_id}")
        return False
    
    title = page_title or page['title']
    current_version = page['version']['number']
    current_content = page['body']['storage']['value']
    
    print(f"\n{'='*60}")
    print(f"Processing: {title}")
    print(f"{'='*60}")
    print(f"[INFO] Current version: v{current_version}")
    print(f"[INFO] Current content: {len(current_content)} characters")
    
    # Add Confluence links
    updated_content = add_links_to_html(current_content)
    
    if updated_content == current_content:
        print(f"[OK] No changes needed (already has links or no references found)")
        return True
    
    changes = len(updated_content) - len(current_content)
    print(f"[INFO] Updated content: {len(updated_content)} characters")
    print(f"[INFO] Changes: {changes:+d} characters")
    
    if dry_run:
        print(f"\n[DRY RUN] Would update page but not applying changes")
        print(f"[DRY RUN] Run without --dry-run to apply")
        return True
    
    # Update the page
    result = confluence_fetch.update_page(page_id, title, updated_content, current_version)
    
    if result:
        print(f"\n[OK] Successfully updated page!")
        print(f"   New version: v{result['version']['number']}")
        print(f"   URL: {confluence_pages.get_page_url(page_id)}")
        return True
    else:
        print(f"\n[ERROR] Failed to update page")
        return False

def update_multiple_pages(page_ids, dry_run=False):
    """
    Update multiple pages with links
    
    Args:
        page_ids: List of page IDs or tuples of (page_id, title)
        dry_run: If True, preview changes without updating
    
    Returns:
        Number of successfully updated pages
    """
    success_count = 0
    
    for item in page_ids:
        # Support both plain page_id and (page_id, title) tuples
        if isinstance(item, tuple):
            page_id, title = item
        else:
            page_id, title = item, None
        
        if update_page_links(page_id, title, dry_run):
            success_count += 1
    
    return success_count

def update_all_known_pages(dry_run=False):
    """
    Update all pages in the registry with internal links
    
    Args:
        dry_run: If True, preview changes without updating
    
    Returns:
        Number of successfully updated pages
    """
    print("[INFO] Updating all pages in the GDD registry...")
    print(f"[INFO] Found {len(confluence_pages.GDD_PAGES)} pages\n")
    
    pages_to_update = [
        (page_id, title) 
        for title, page_id in confluence_pages.GDD_PAGES.items()
    ]
    
    return update_multiple_pages(pages_to_update, dry_run)

def main():
    """Command-line interface"""
    if len(sys.argv) < 2:
        print("Confluence Link Updater")
        print("\nUsage:")
        print("  Update single page:")
        print("    py confluence_update_links.py <page_id> [--dry-run]")
        print("  Update multiple pages:")
        print("    py confluence_update_links.py <page_id1> <page_id2> ... [--dry-run]")
        print("  Update page by title:")
        print("    py confluence_update_links.py --title \"Page Title\" [--dry-run]")
        print("  Update all known pages:")
        print("    py confluence_update_links.py --all [--dry-run]")
        print("\nExamples:")
        print("  py confluence_update_links.py 1703949 --dry-run")
        print("  py confluence_update_links.py 1703949 1605636 1671171")
        print("  py confluence_update_links.py --title \"GDD Index\"")
        print("  py confluence_update_links.py --all --dry-run")
        print("\nNote: Page registry is defined in confluence_pages.py")
        return 1
    
    # Parse arguments
    dry_run = '--dry-run' in sys.argv
    update_all = '--all' in sys.argv
    page_ids = []
    title_arg = None
    
    i = 1
    while i < len(sys.argv):
        arg = sys.argv[i]
        
        if arg in ('--dry-run', '--all'):
            i += 1
            continue
        elif arg == '--title' and i + 1 < len(sys.argv):
            title_arg = sys.argv[i + 1]
            i += 2
            continue
        elif arg.isdigit():
            page_ids.append(int(arg))
            i += 1
        else:
            print(f"[ERROR] Unknown argument: {arg}")
            return 1
    
    print("=== Confluence Link Updater ===\n")
    
    if dry_run:
        print("[INFO] DRY RUN MODE - No changes will be made\n")
    
    # Update all pages
    if update_all:
        success_count = update_all_known_pages(dry_run)
        total = len(confluence_pages.GDD_PAGES)
        print(f"\n{'='*60}")
        print(f"Summary: {success_count}/{total} pages processed")
        print(f"{'='*60}")
        return 0 if success_count > 0 else 1
    
    # Update by title
    if title_arg:
        page_id = confluence_pages.get_page_id(title_arg)
        if not page_id:
            print(f"[ERROR] Page not found in registry: {title_arg}")
            print(f"[INFO] Check confluence_pages.py or use page ID directly")
            return 1
        success = update_page_links(page_id, title_arg, dry_run)
        return 0 if success else 1
    
    # Update specific pages
    if not page_ids:
        print("[ERROR] No page IDs provided")
        print("[INFO] Use --all to update all known pages")
        return 1
    
    success_count = update_multiple_pages(page_ids, dry_run)
    print(f"\n{'='*60}")
    print(f"Summary: {success_count}/{len(page_ids)} pages updated")
    print(f"{'='*60}")
    return 0 if success_count == len(page_ids) else 1

if __name__ == "__main__":
    sys.exit(main())
