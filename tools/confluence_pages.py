"""
Confluence Page Registry
Centralized mapping of GDD page titles to Confluence page IDs
"""

# Base URL for the MS Confluence space
CONFLUENCE_BASE_URL = "https://o92design.atlassian.net"
CONFLUENCE_SPACE_KEY = "MS"

# GDD Page Registry
# This mapping is used by all Confluence tools for creating internal links
# and referencing pages programmatically
#
# Format: 'Page Title': page_id
# Update this file when new GDD pages are created

GDD_PAGES = {
    # Root
    'GDD Master': 393217,
    
    # Meta Documentation
    'GDD Index': 1703949,
    'GDD Quick Reference': 1605636,
    'GDD Quick Start': 1671171,
    
    # Core Systems
    'Core Gameplay Loop': 622601,
    'Ship Control': 1343506,
    'Passenger System': 1572869,
    'Ship Propulsion and Engine Systems': 2588673,
    
    # Narrative
    'Three-Act Structure': 851973,
    'Emotional Core & Setting': 2097175,
    
    # Folder pages (if needed)
    'Systems': 950288,
    'Tourism': 1802241,
    'Narrative': 1114113,
    
    # Vertical Slice Prototype (MVP)
    'Vertical Slice Prototype - Ship Control & POI': 3047427,
    'Ship Control System - MVP Specification': 3047444,
    'POI System - MVP Specification': 3047460,
    'Voyage Flow & Integration - MVP': 3047476,
    'MVP Test Scenarios & Playtesting Guide': 3112963,
    'MVP Technical Requirements': 3080194,
    'Vertical Slice Milestones & Deliverables': 3080209,
    
    # Technical Documentation (top-level)
    'Technical Documentation': 4358166,
    'POI & Discovery System': 4390913,
    'POI System - Technical Implementation': 4423681,
    'Fog of War System - Technical Implementation': 4456449,
    'Spatial Hash & Chunk Systems': 4489217,
    
    # Modding Documentation (top-level)
    'Modding Documentation': 4358182,
    'POI Modding Guide': 4358146,
}

# Page aliases (alternative names that map to canonical pages)
PAGE_ALIASES = {
    'Ship Direct Control': 'Ship Control',
    'Three Act Structure': 'Three-Act Structure',
    'GDD Structure': 'GDD Master',  # If GDD Structure becomes separate, update this
}

def get_page_id(page_title):
    """
    Get page ID for a given title, checking aliases
    
    Args:
        page_title: Page title to lookup
        
    Returns:
        Page ID if found, None otherwise
    """
    # Check direct match
    if page_title in GDD_PAGES:
        return GDD_PAGES[page_title]
    
    # Check aliases
    if page_title in PAGE_ALIASES:
        canonical = PAGE_ALIASES[page_title]
        return GDD_PAGES.get(canonical)
    
    return None

def get_page_url(page_title_or_id):
    """
    Get full Confluence URL for a page
    
    Args:
        page_title_or_id: Page title or page ID
        
    Returns:
        Full URL if found, None otherwise
    """
    if isinstance(page_title_or_id, str):
        page_id = get_page_id(page_title_or_id)
    else:
        page_id = page_title_or_id
    
    if page_id:
        return f"{CONFLUENCE_BASE_URL}/wiki/spaces/{CONFLUENCE_SPACE_KEY}/pages/{page_id}"
    
    return None

def get_canonical_title(page_title):
    """
    Get canonical page title, resolving aliases
    
    Args:
        page_title: Page title (may be alias)
        
    Returns:
        Canonical title if found, original title otherwise
    """
    if page_title in PAGE_ALIASES:
        return PAGE_ALIASES[page_title]
    return page_title

def get_all_pages():
    """Get all known page mappings (including aliases)"""
    all_pages = dict(GDD_PAGES)
    
    # Add aliases
    for alias, canonical in PAGE_ALIASES.items():
        if canonical in GDD_PAGES:
            all_pages[alias] = GDD_PAGES[canonical]
    
    return all_pages

def add_page(title, page_id):
    """
    Add a new page to the registry (for programmatic updates)
    Note: This only updates the in-memory registry, not the file
    
    Args:
        title: Page title
        page_id: Confluence page ID
    """
    GDD_PAGES[title] = page_id

def list_pages():
    """Print all registered pages"""
    print("=== GDD Page Registry ===\n")
    print("Core Pages:")
    for title, page_id in sorted(GDD_PAGES.items()):
        url = get_page_url(page_id)
        print(f"  {title:30s} ID: {page_id:7d}  {url}")
    
    print("\nAliases:")
    for alias, canonical in sorted(PAGE_ALIASES.items()):
        page_id = get_page_id(alias)
        print(f"  {alias:30s} -> {canonical:20s} (ID: {page_id})")

if __name__ == "__main__":
    import sys
    
    if len(sys.argv) > 1:
        if sys.argv[1] == 'list':
            list_pages()
        elif sys.argv[1] == 'get' and len(sys.argv) > 2:
            title = sys.argv[2]
            page_id = get_page_id(title)
            if page_id:
                print(f"{title} -> ID: {page_id}")
                print(f"URL: {get_page_url(page_id)}")
            else:
                print(f"Page not found: {title}")
        else:
            print("Usage:")
            print("  py confluence_pages.py list")
            print("  py confluence_pages.py get \"Page Title\"")
    else:
        list_pages()
