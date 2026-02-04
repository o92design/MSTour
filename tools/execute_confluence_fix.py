#!/usr/bin/env python3
"""
Execute Confluence hierarchy fix for POI documentation
Creates pages in correct structure without user interaction
"""

import sys
import os
from pathlib import Path

# Add tools directory to path
sys.path.insert(0, str(Path(__file__).parent))

import load_env
import confluence_fetch
import markdown2

# Load environment
env = load_env.load_env()

def markdown_to_html(md_path):
    """Convert markdown file to Confluence HTML"""
    with open(md_path, 'r', encoding='utf-8') as f:
        content = f.read()
    return markdown2.markdown(content, extras=['tables', 'fenced-code-blocks', 'header-ids'])

def create_or_get_page(title, content_html, parent_id=None):
    """Create page or return existing page ID"""
    # Check if page exists
    existing = confluence_fetch.find_page_by_title(title)
    if existing:
        print(f"  [EXISTS] '{title}' (ID: {existing['id']})")
        return int(existing['id'])
    
    # Create new page
    result = confluence_fetch.create_page(title, content_html, parent_id)
    page_id = int(result['id'])
    print(f"  [CREATED] '{title}' (ID: {page_id})")
    return page_id

def main():
    print("=" * 60)
    print("  Confluence POI Documentation Hierarchy Fix")
    print("=" * 60)
    print()
    
    # Test connection first
    print("[1/6] Testing Confluence connection...")
    try:
        # Test by getting GDD Master page
        gdd = confluence_fetch.find_page_by_title("GDD Master")
        if gdd:
            print(f"  Connected! Found GDD Master (ID: {gdd['id']})")
        else:
            print(f"  WARNING: Connected but GDD Master not found")
    except Exception as e:
        print(f"  ERROR: Cannot connect to Confluence: {e}")
        return 1
    
    docs_path = Path(__file__).parent.parent / "docs" / "GDD"
    page_ids = {}
    
    # Step 1: Create Technical Documentation (top-level)
    print()
    print("[2/6] Creating Technical Documentation (top-level)...")
    tech_doc_html = markdown_to_html(docs_path / "Technical_Documentation.md")
    page_ids['Technical Documentation'] = create_or_get_page(
        "Technical Documentation", 
        tech_doc_html, 
        parent_id=None  # Top-level
    )
    
    # Step 2: Create Modding Documentation (top-level)
    print()
    print("[3/6] Creating Modding Documentation (top-level)...")
    mod_doc_html = markdown_to_html(docs_path / "Modding_Documentation.md")
    page_ids['Modding Documentation'] = create_or_get_page(
        "Modding Documentation",
        mod_doc_html,
        parent_id=None  # Top-level
    )
    
    # Step 3: Create POI & Discovery System (under Technical)
    print()
    print("[4/6] Creating POI & Discovery System (under Technical Documentation)...")
    poi_parent_html = markdown_to_html(docs_path / "POI_Discovery_Parent.md")
    page_ids['POI & Discovery System'] = create_or_get_page(
        "POI & Discovery System",
        poi_parent_html,
        parent_id=page_ids['Technical Documentation']
    )
    
    # Step 4: Create technical implementation pages (under POI & Discovery)
    print()
    print("[5/6] Creating technical implementation pages...")
    
    poi_tech_html = markdown_to_html(docs_path / "POI_Technical_Implementation.md")
    page_ids['POI System - Technical Implementation'] = create_or_get_page(
        "POI System - Technical Implementation",
        poi_tech_html,
        parent_id=page_ids['POI & Discovery System']
    )
    
    fog_tech_html = markdown_to_html(docs_path / "Fog_of_War_Technical.md")
    page_ids['Fog of War System - Technical Implementation'] = create_or_get_page(
        "Fog of War System - Technical Implementation",
        fog_tech_html,
        parent_id=page_ids['POI & Discovery System']
    )
    
    spatial_html = markdown_to_html(docs_path / "Spatial_Hash_Chunk_Systems.md")
    page_ids['Spatial Hash & Chunk Systems'] = create_or_get_page(
        "Spatial Hash & Chunk Systems",
        spatial_html,
        parent_id=page_ids['POI & Discovery System']
    )
    
    # Step 5: Create modding guide (under Modding Documentation)
    print()
    print("[6/6] Creating POI Modding Guide (under Modding Documentation)...")
    modding_html = markdown_to_html(docs_path / "POI_Modding_Guide.md")
    page_ids['POI Modding Guide'] = create_or_get_page(
        "POI Modding Guide",
        modding_html,
        parent_id=page_ids['Modding Documentation']
    )
    
    # Summary
    print()
    print("=" * 60)
    print("  SUCCESS! All pages created.")
    print("=" * 60)
    print()
    print("Page IDs for confluence_pages.py:")
    print("-" * 40)
    for title, page_id in page_ids.items():
        print(f"    '{title}': {page_id},")
    print()
    
    # Output Confluence URLs
    base_url = env.get('ATLASSIAN_BASE_URL', 'https://o92design.atlassian.net')
    print("Page URLs:")
    print("-" * 40)
    for title, page_id in page_ids.items():
        print(f"  {title}:")
        print(f"    {base_url}/wiki/spaces/MS/pages/{page_id}")
    
    return 0

if __name__ == "__main__":
    sys.exit(main())
