#!/usr/bin/env python3
"""
Confluence Push Tool
Pushes markdown content to Confluence pages
"""

import os
import sys
import json
import requests
import markdown2
from pathlib import Path
from base64 import b64encode
from fnmatch import fnmatch

import load_env
import confluence_fetch

# Load environment variables
env = load_env.load_env()

# Confluence configuration
CONFLUENCE_EMAIL = env.get('ATLASSIAN_EMAIL')  # Same credentials as Jira
CONFLUENCE_TOKEN = env.get('ATLASSIAN_API_TOKEN')
CONFLUENCE_BASE_URL = env.get('ATLASSIAN_BASE_URL', 'https://o92design.atlassian.net')
CONFLUENCE_SPACE_KEY = env.get('CONFLUENCE_SPACE_KEY', 'MS')

def get_auth_header():
    """Create Basic Auth header"""
    auth_string = f"{CONFLUENCE_EMAIL}:{CONFLUENCE_TOKEN}"
    auth_bytes = auth_string.encode('ascii')
    base64_bytes = b64encode(auth_bytes)
    base64_string = base64_bytes.decode('ascii')
    return f"Basic {base64_string}"

def markdown_to_confluence_storage(markdown_text):
    """Convert markdown to Confluence storage format (HTML)"""
    # Use markdown2 for better conversion
    html = markdown2.markdown(markdown_text, extras=['tables', 'fenced-code-blocks', 'header-ids'])
    return html

def update_page(page_id, title, content, current_version):
    """Update a Confluence page"""
    url = f"{CONFLUENCE_BASE_URL}/wiki/rest/api/content/{page_id}"
    
    headers = {
        'Authorization': get_auth_header(),
        'Content-Type': 'application/json',
        'Accept': 'application/json'
    }
    
    payload = {
        'id': str(page_id),
        'type': 'page',
        'title': title,
        'body': {
            'storage': {
                'value': content,
                'representation': 'storage'
            }
        },
        'version': {
            'number': current_version + 1
        }
    }
    
    response = requests.put(url, json=payload, headers=headers)
    response.raise_for_status()
    return response.json()

def test_connection():
    """Test if we can connect to Confluence"""
    url = f"{CONFLUENCE_BASE_URL}/wiki/rest/api/space/{CONFLUENCE_SPACE_KEY}"
    headers = {
        'Authorization': get_auth_header(),
        'Accept': 'application/json'
    }
    
    try:
        response = requests.get(url, headers=headers)
        response.raise_for_status()
        space_data = response.json()
        print(f"✅ Connected to Confluence space: {space_data.get('name', 'MS')}")
        return True
    except requests.exceptions.RequestException as e:
        print(f"❌ Connection failed: {e}")
        return False

def push_markdown_to_confluence(markdown_file_path, page_id, dry_run=True):
    """
    Push markdown file content to Confluence page
    
    Args:
        markdown_file_path: Path to markdown file
        page_id: Confluence page ID
        dry_run: If True, preview only (don't actually update)
    """
    # Read markdown file
    md_path = Path(markdown_file_path)
    if not md_path.exists():
        print(f"❌ File not found: {markdown_file_path}")
        return False
    
    with open(md_path, 'r', encoding='utf-8') as f:
        markdown_content = f.read()
    
    print(f"📖 Read {len(markdown_content)} characters from {md_path.name}")
    
    # Convert to Confluence storage format
    html_content = markdown_to_confluence_storage(markdown_content)
    print(f"🔄 Converted to HTML ({len(html_content)} characters)")
    
    # Get current page details
    try:
        page = confluence_fetch.get_page(page_id)
        print(f"📄 Current page: '{page['title']}' (v{page['version']['number']})")
    except Exception as e:
        print(f"❌ Failed to fetch page: {e}")
        return False
    
    if dry_run:
        print(f"\n🔍 DRY RUN - Preview only (not updating)")
        print(f"   Would update: {page['title']}")
        print(f"   New version: v{page['version']['number'] + 1}")
        print(f"   Content length: {len(html_content)} chars")
        print(f"\n   Preview (first 500 chars of HTML):")
        print(f"   {html_content[:500]}...")
        return True
    else:
        # Actually update the page
        try:
            result = confluence_fetch.update_page(page_id, page['title'], html_content, page['version']['number'])
            print(f"\n✅ Successfully updated page!")
            print(f"   New version: v{result['version']['number']}")
            print(f"   URL: {CONFLUENCE_BASE_URL}/wiki/spaces/{CONFLUENCE_SPACE_KEY}/pages/{page_id}")
            return True
        except Exception as e:
            print(f"\n❌ Failed to update page: {e}")
            return False

def main():
    print("=== Confluence Push Tool ===\n")
    
    # Parse command line arguments
    if len(sys.argv) < 2:
        print("Usage:")
        print("  Test connection:")
        print("    py confluence_push.py test")
        print("  Push single markdown file:")
        print("    py confluence_push.py push <markdown_file> [--page-id <id>] [--confirm]")
        print("  Batch push all GDD subfolder markdown files:")
        print("    py confluence_push.py batch [--confirm]")
        print("\nExamples:")
        print("  # Preview batch push")
        print("  py confluence_push.py batch")
        print("  # Actually push all files")
        print("  py confluence_push.py batch --confirm")
        return 1
    
    command = sys.argv[1]
    
    if command == 'test':
        # Test connection
        if not test_connection():
            print("\n❌ Cannot connect to Confluence. Check credentials in .env file.")
            return 1
        
        # Try to find the GDD Master page
        print("\n🔍 Searching for 'GDD Master' page...")
        page = confluence_fetch.find_page_by_title("GDD Master")
        
        if page:
            print(f"\n📄 Page Details:")
            print(f"   Title: {page['title']}")
            print(f"   ID: {page['id']}")
            print(f"   Version: {page['version']['number']}")
            print(f"   URL: {CONFLUENCE_BASE_URL}/wiki/spaces/{CONFLUENCE_SPACE_KEY}/pages/{page['id']}")
            
            print("\n✅ Successfully connected and found your GDD page!")
            print("   Ready to push markdown content.")
            return 0
        else:
            return 1
    
    elif command == 'batch':
        # Batch push markdown files from subfolders
        confirm = '--confirm' in sys.argv
        parent_id = None
        folder_path = None
        pattern = None
        specific_files = []
        
        # Parse arguments
        i = 2
        while i < len(sys.argv):
            arg = sys.argv[i]
            if arg == '--parent-id' and i + 1 < len(sys.argv):
                parent_id = int(sys.argv[i + 1])
                i += 2
            elif arg == '--folder' and i + 1 < len(sys.argv):
                folder_path = Path(sys.argv[i + 1])
                i += 2
            elif arg == '--pattern' and i + 1 < len(sys.argv):
                pattern = sys.argv[i + 1]
                i += 2
            elif arg == '--files':
                # Collect all files until next flag or end
                i += 1
                while i < len(sys.argv) and not sys.argv[i].startswith('--'):
                    specific_files.append(sys.argv[i])
                    i += 1
            elif arg == '--confirm':
                i += 1
            else:
                i += 1
        
        # Default to GDD folder if not specified
        if folder_path is None:
            folder_path = Path(__file__).parent.parent / 'docs' / 'GDD'
        
        if not folder_path.exists():
            print(f"❌ Folder not found: {folder_path}")
            return 1
        
        dry_run = not confirm
        
        if dry_run:
            print("🔍 DRY RUN MODE (add --confirm to actually push)\n")
        else:
            print("⚠️  LIVE MODE - Will create/update pages in Confluence\n")
        
        print(f"📁 Scanning folder: {folder_path}")
        if parent_id:
            print(f"👪 Parent page ID: {parent_id}\n")
        else:
            print(f"⚠️  No parent page specified - pages will be created at root level\n")
        
        # Test connection
        if not test_connection():
            print("\n❌ Cannot connect to Confluence.")
            return 1
        
        # Find markdown files based on filters
        markdown_files = []
        
        if specific_files:
            # Use specific files provided by --files flag
            print(f"🎯 Using specific files: {len(specific_files)} file(s)\n")
            for file_path_str in specific_files:
                md_file = folder_path / file_path_str
                if not md_file.exists():
                    print(f"⚠️  File not found: {file_path_str}")
                    continue
                if md_file.suffix != '.md':
                    print(f"⚠️  Not a markdown file: {file_path_str}")
                    continue
                    
                title = md_file.stem.replace('GDD_', '').replace('_', ' ')
                markdown_files.append({
                    'title': title,
                    'file_path': md_file,
                    'relative_path': md_file.relative_to(folder_path)
                })
        else:
            # Scan all .md files in subfolders (not root-level files)
            all_md_files = []
            for subfolder in folder_path.iterdir():
                if subfolder.is_dir():
                    for md_file in subfolder.glob('*.md'):
                        all_md_files.append(md_file)
            
            # Apply pattern filter if specified
            if pattern:
                print(f"🔍 Filtering with pattern: {pattern}\n")
                for md_file in all_md_files:
                    # Check if filename matches pattern
                    if fnmatch(md_file.name, pattern):
                        title = md_file.stem.replace('GDD_', '').replace('_', ' ')
                        markdown_files.append({
                            'title': title,
                            'file_path': md_file,
                            'relative_path': md_file.relative_to(folder_path)
                        })
            else:
                # No filter - use all files
                for md_file in all_md_files:
                    title = md_file.stem.replace('GDD_', '').replace('_', ' ')
                    markdown_files.append({
                        'title': title,
                        'file_path': md_file,
                        'relative_path': md_file.relative_to(folder_path)
                    })
        
        if not markdown_files:
            print(f"❌ No markdown files found in subfolders of {folder_path}")
            return 1
        
        print(f"📚 Found {len(markdown_files)} markdown files to process:\n")
        for item in markdown_files:
            print(f"   • {item['title']} ({item['relative_path']})")
        print()
        
        results = []
        
        for item in markdown_files:
            print(f"📖 Processing: {item['title']}")
            print(f"   File: {item['relative_path']}")
            
            # Read markdown
            with open(item['file_path'], 'r', encoding='utf-8') as f:
                markdown_content = f.read()
            
            print(f"   📏 Size: {len(markdown_content)} chars")
            
            # Convert to HTML
            html_content = markdown_to_confluence_storage(markdown_content)
            
            # Check if page exists
            existing_page = confluence_fetch.find_page_by_title(item['title'])
            
            try:
                if existing_page:
                    # Update existing page
                    print(f"   📄 Page exists (ID: {existing_page['id']}, v{existing_page['version']['number']})")
                    
                    if dry_run:
                        print(f"   🔍 Would update to v{existing_page['version']['number'] + 1}\n")
                        results.append(True)
                    else:
                        result = confluence_fetch.update_page(
                            existing_page['id'],
                            item['title'],
                            html_content,
                            existing_page['version']['number']
                        )
                        if result:
                            print(f"   ✅ Updated to v{result['version']['number']}\n")
                            results.append(True)
                        else:
                            results.append(False)
                else:
                    # Create new page
                    if parent_id:
                        print(f"   ➕ Page doesn't exist - will create as child of page {parent_id}")
                    else:
                        print(f"   ➕ Page doesn't exist - will create at root level")
                    
                    if dry_run:
                        print(f"   🔍 Would create new page\n")
                        results.append(True)
                    else:
                        result = confluence_fetch.create_page(
                            item['title'],
                            html_content,
                            parent_id
                        )
                        if result:
                            url = f"{CONFLUENCE_BASE_URL}/wiki/spaces/{CONFLUENCE_SPACE_KEY}/pages/{result['id']}"
                            print(f"   🔗 {url}\n")
                            results.append(True)
                        else:
                            results.append(False)
            except Exception as e:
                print(f"   ❌ Error: {e}\n")
                results.append(False)
        
        # Summary
        print("=" * 60)
        success_count = sum(results)
        total_count = len(results)
        print(f"✅ Successfully processed: {success_count}/{total_count} pages")
        
        if dry_run:
            print(f"\n💡 To actually push to Confluence, run:")
            cmd = f"   py tools\\confluence_push.py batch"
            if parent_id:
                cmd += f" --parent-id {parent_id}"
            if folder_path != Path(__file__).parent.parent / 'docs' / 'GDD':
                cmd += f" --folder \"{folder_path}\""
            cmd += " --confirm"
            print(cmd)
        else:
            print(f"\n🎉 Pages are now live in Confluence!")
            if parent_id:
                print(f"   Parent: {CONFLUENCE_BASE_URL}/wiki/spaces/{CONFLUENCE_SPACE_KEY}/pages/{parent_id}")
        
        return 0 if all(results) else 1
    
    else:
        print(f"❌ Unknown command: {command}")
        print("   Use 'test' or 'push'")
        return 1

if __name__ == '__main__':
    sys.exit(main())
