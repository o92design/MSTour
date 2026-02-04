#!/usr/bin/env python3
"""
Push POI & Discovery System documentation to Confluence
Creates 5 pages in the correct hierarchy
"""

import sys
import os
from pathlib import Path

# Add tools directory to path
sys.path.insert(0, str(Path(__file__).parent))

import confluence_fetch
import markdown2

# Parent page IDs
SYSTEMS_PAGE_ID = 950288    # "Systems" folder
TOURISM_PAGE_ID = 1802241   # "Tourism" folder

# Markdown files to push
DOCS_DIR = Path(__file__).parent.parent / 'docs' / 'GDD'

PAGES_TO_CREATE = [
    {
        'file': 'POI_Discovery_Parent.md',
        'title': 'POI & Discovery System',
        'parent_id': SYSTEMS_PAGE_ID,
        'description': 'Parent page for POI and Discovery system documentation'
    },
    # Child pages (will be created after parent)
    {
        'file': 'POI_Technical_Implementation.md',
        'title': 'POI System - Technical Implementation',
        'parent_id': None,  # Will be set after parent is created
        'description': 'Technical details of POI ECS system',
        'child_of': 'POI & Discovery System'
    },
    {
        'file': 'Fog_of_War_Technical.md',
        'title': 'Fog of War System - Technical Implementation',
        'parent_id': None,
        'description': 'Technical details of Fog of War chunk system',
        'child_of': 'POI & Discovery System'
    },
    {
        'file': 'Spatial_Hash_Chunk_Systems.md',
        'title': 'Spatial Hash & Chunk Systems',
        'parent_id': None,
        'description': 'Engine-level spatial data structures',
        'child_of': 'POI & Discovery System'
    },
    {
        'file': 'POI_Modding_Guide.md',
        'title': 'POI Modding Guide',
        'parent_id': TOURISM_PAGE_ID,
        'description': 'Guide for modders to add custom POIs'
    }
]

def markdown_to_confluence_storage(markdown_text):
    """Convert markdown to Confluence storage format (HTML)"""
    html = markdown2.markdown(markdown_text, extras=['tables', 'fenced-code-blocks', 'header-ids'])
    return html

def create_or_update_page(title, content, parent_id):
    """Create or update a Confluence page"""
    # Check if page exists
    existing_page = confluence_fetch.find_page_by_title(title)
    
    if existing_page:
        # Update existing page
        print(f"📝 Updating existing page: '{title}' (ID: {existing_page['id']})")
        result = confluence_fetch.update_page(
            existing_page['id'],
            title,
            content,
            existing_page['version']['number']
        )
        if result:
            print(f"✅ Updated to v{result['version']['number']}")
            return existing_page['id']
        else:
            print(f"❌ Failed to update page: {title}")
            return None
    else:
        # Create new page
        print(f"➕ Creating new page: '{title}' under parent {parent_id}")
        result = confluence_fetch.create_page(title, content, parent_id)
        if result:
            print(f"✅ Created page ID: {result['id']}")
            return result['id']
        else:
            print(f"❌ Failed to create page: {title}")
            return None

def main():
    print("=" * 60)
    print("POI & Discovery System Documentation Push")
    print("=" * 60)
    print()
    
    # Track created page IDs
    page_ids = {}
    
    # Process pages in order
    for page_info in PAGES_TO_CREATE:
        file_path = DOCS_DIR / page_info['file']
        title = page_info['title']
        
        if not file_path.exists():
            print(f"❌ File not found: {file_path}")
            continue
        
        # Read markdown
        print(f"\n📖 Reading: {page_info['file']}")
        with open(file_path, 'r', encoding='utf-8') as f:
            markdown_content = f.read()
        
        print(f"   Size: {len(markdown_content)} characters")
        
        # Convert to HTML
        html_content = markdown_to_confluence_storage(markdown_content)
        print(f"   Converted to HTML: {len(html_content)} characters")
        
        # Determine parent ID
        if 'child_of' in page_info:
            # This is a child page - look up parent ID
            parent_title = page_info['child_of']
            if parent_title in page_ids:
                parent_id = page_ids[parent_title]
            else:
                # Parent might exist already, try to find it
                parent_page = confluence_fetch.find_page_by_title(parent_title)
                if parent_page:
                    parent_id = parent_page['id']
                    page_ids[parent_title] = parent_id
                else:
                    print(f"❌ Parent page '{parent_title}' not found!")
                    continue
        else:
            parent_id = page_info['parent_id']
        
        # Create or update page
        page_id = create_or_update_page(title, html_content, parent_id)
        
        if page_id:
            page_ids[title] = page_id
            print(f"   🔗 URL: https://o92design.atlassian.net/wiki/spaces/MS/pages/{page_id}")
        else:
            print(f"   ⚠️  Failed to create/update: {title}")
    
    print("\n" + "=" * 60)
    print("Summary")
    print("=" * 60)
    print(f"✅ Successfully processed: {len(page_ids)} pages")
    print("\nPage IDs (add these to confluence_pages.py):")
    for title, page_id in page_ids.items():
        print(f"    '{title}': {page_id},")
    print()
    print("🎉 Documentation push complete!")
    print()

if __name__ == '__main__':
    main()
