"""
Confluence Fetch Tool
Fetches markdown content from Confluence pages
"""

import os
import sys
import json
from pathlib import Path

try:
    import requests
    from bs4 import BeautifulSoup
    import html2text
except ImportError as e:
    print(f"❌ Missing required package: {e}")
    print("Install with: pip install beautifulsoup4 html2text requests")
    sys.exit(1)

import load_env

# Load environment variables
env = load_env.load_env()
# Confluence configuration
CONFLUENCE_EMAIL = env.get('ATLASSIAN_EMAIL') 
CONFLUENCE_TOKEN = env.get('ATLASSIAN_API_TOKEN')
CONFLUENCE_BASE_URL = env.get('ATLASSIAN_BASE_URL', 'https://o92design.atlassian.net')
CONFLUENCE_SPACE_KEY = env.get('CONFLUENCE_SPACE_KEY', 'MS')

def get_auth_header():
    """Create Basic Auth header"""
    from base64 import b64encode
    
    if not CONFLUENCE_EMAIL or not CONFLUENCE_TOKEN:
        print("[ERROR] Missing credentials. Set ATLASSIAN_EMAIL and ATLASSIAN_API_TOKEN in .env")
        sys.exit(1)
    
    auth_string = f"{CONFLUENCE_EMAIL}:{CONFLUENCE_TOKEN}"
    auth_bytes = auth_string.encode('ascii')
    base64_bytes = b64encode(auth_bytes)
    base64_string = base64_bytes.decode('ascii')
    return f"Basic {base64_string}"

def get_page(page_id):
    """Fetch existing page details"""
    url = f"{CONFLUENCE_BASE_URL}/wiki/rest/api/content/{page_id}"
    params = {
        'expand': 'body.storage,version'
    }
    headers = {
        'Authorization': get_auth_header(),
        'Accept': 'application/json'
    }
    
    try:
        response = requests.get(url, params=params, headers=headers)
        response.raise_for_status()
        return response.json()
    except requests.exceptions.RequestException as e:
        print(f"[ERROR] Failed to fetch page: {e}")
        return None

def find_page_by_title(title):
    """Search for page by title in the MS space"""
    url = f"{CONFLUENCE_BASE_URL}/wiki/rest/api/content"
    headers = {
        'Authorization': get_auth_header(),
        'Accept': 'application/json'
    }
    params = {
        'spaceKey': CONFLUENCE_SPACE_KEY,
        'title': title,
        'expand': 'version'
    }
    
    try:
        response = requests.get(url, params=params, headers=headers)
        response.raise_for_status()
        data = response.json()
        
        if data['results']:
            page = data['results'][0]
            print(f"[OK] Found page: '{page['title']}' (ID: {page['id']})")
            return page
        else:
            print(f"[ERROR] No page found with title: '{title}'")
            return None
    except requests.exceptions.RequestException as e:
        print(f"[ERROR] Search failed: {e}")
        return None

def confluence_storage_to_markdown(storage_text):
    """Convert Confluence storage format (HTML) to markdown"""
    # Parse the HTML content
    soup = BeautifulSoup(storage_text, 'html.parser')
    html_content = str(soup)

    # Convert HTML to Markdown
    h = html2text.HTML2Text()
    h.ignore_links = False
    markdown = h.handle(html_content)
    return markdown

def get_page_content(page_identifier):
    """Fetch page content from Confluence by ID or title"""
    # Check if it's a page ID (numeric) or title
    if page_identifier.isdigit():
        page_id = page_identifier
    else:
        # Search by title
        page = find_page_by_title(page_identifier)
        if not page:
            return None
        page_id = page['id']
    
    url = f"{CONFLUENCE_BASE_URL}/wiki/rest/api/content/{page_id}"
    params = {
        'expand': 'body.storage'
    }
    headers = {
        'Authorization': get_auth_header(),
        'Accept': 'application/json'
    }
    
    try:
        response = requests.get(url, params=params, headers=headers)
        response.raise_for_status()
        data = response.json()
        storage_content = data['body']['storage']['value']
        markdown_content = confluence_storage_to_markdown(storage_content)
        return markdown_content
    except requests.exceptions.RequestException as e:
        print(f"[ERROR] Failed to fetch page content: {e}")
        return None

def create_page(title, content, parent_id=None):
    """
    Create a new Confluence page
    
    Args:
        title: Page title
        content: HTML content in Confluence storage format
        parent_id: Parent page ID (if None, creates top-level page)
    """
    url = f"{CONFLUENCE_BASE_URL}/wiki/rest/api/content"
    
    headers = {
        'Authorization': get_auth_header(),
        'Content-Type': 'application/json',
        'Accept': 'application/json'
    }
    
    payload = {
        'type': 'page',
        'title': title,
        'space': {'key': CONFLUENCE_SPACE_KEY},
        'body': {
            'storage': {
                'value': content,
                'representation': 'storage'
            }
        }
    }
    
    # Only add ancestors if parent_id is specified
    if parent_id is not None:
        payload['ancestors'] = [{'id': str(parent_id)}]
    
    try:
        response = requests.post(url, json=payload, headers=headers)
        response.raise_for_status()
        result = response.json()
        parent_info = f" as child of page {parent_id}" if parent_id else " at top level"
        print(f"[OK] Created page: '{title}' (ID: {result['id']}){parent_info}")
        return result
    except requests.exceptions.RequestException as e:
        print(f"[ERROR] Failed to create page: {e}")
        if hasattr(e, 'response') and e.response:
            print(f"   Response: {e.response.text}")
        return None

def update_page(page_id, title, content, current_version):
    """Update an existing Confluence page"""
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
    
    try:
        response = requests.put(url, json=payload, headers=headers)
        response.raise_for_status()
        return response.json()
    except requests.exceptions.RequestException as e:
        print(f"[ERROR] Failed to update page: {e}")
        if hasattr(e, 'response') and e.response:
            print(f"   Response: {e.response.text}")
        return None

def move_page(page_id, new_parent_id):
    """Move a page to a new parent"""
    # First get current page info
    page = get_page(page_id)
    if not page:
        print(f"[ERROR] Cannot find page {page_id}")
        return None
    
    url = f"{CONFLUENCE_BASE_URL}/wiki/rest/api/content/{page_id}"
    
    headers = {
        'Authorization': get_auth_header(),
        'Content-Type': 'application/json',
        'Accept': 'application/json'
    }
    
    payload = {
        'id': str(page_id),
        'type': 'page',
        'title': page['title'],
        'ancestors': [{'id': str(new_parent_id)}],
        'version': {
            'number': page['version']['number'] + 1
        }
    }
    
    try:
        response = requests.put(url, json=payload, headers=headers)
        response.raise_for_status()
        result = response.json()
        print(f"[OK] Moved '{page['title']}' to parent {new_parent_id}")
        return result
    except requests.exceptions.RequestException as e:
        print(f"[ERROR] Failed to move page: {e}")
        if hasattr(e, 'response') and e.response:
            print(f"   Response: {e.response.text}")
        return None

def list_pages_in_space():
    """List all pages in the MS space"""
    url = f"{CONFLUENCE_BASE_URL}/wiki/rest/api/content"
    headers = {
        'Authorization': get_auth_header(),
        'Accept': 'application/json'
    }
    params = {
        'spaceKey': CONFLUENCE_SPACE_KEY,
        'expand': 'version,ancestors',
        'limit': 100
    }
    
    try:
        response = requests.get(url, params=params, headers=headers)
        response.raise_for_status()
        data = response.json()
        return data['results']
    except requests.exceptions.RequestException as e:
        print(f"[ERROR] Failed to list pages: {e}")
        return []

def get_child_pages(parent_id):
    """Get child pages of a parent page"""
    url = f"{CONFLUENCE_BASE_URL}/wiki/rest/api/content/{parent_id}/child/page"
    headers = {
        'Authorization': get_auth_header(),
        'Accept': 'application/json'
    }
    params = {
        'expand': 'version',
        'limit': 100
    }
    
    try:
        response = requests.get(url, params=params, headers=headers)
        response.raise_for_status()
        data = response.json()
        return data['results']
    except requests.exceptions.RequestException as e:
        print(f"[ERROR] Failed to get child pages: {e}")
        return []

if __name__ == "__main__":
    import sys
    import io
    
    # Force UTF-8 output on Windows
    if sys.platform == 'win32':
        sys.stdout = io.TextIOWrapper(sys.stdout.buffer, encoding='utf-8', errors='replace')
    
    if len(sys.argv) < 2:
        print("Usage:")
        print("  py confluence_fetch.py <command> [args]")
        print("\nCommands:")
        print("  get <page-id-or-title>  - Fetch page content as markdown")
        print("  find <title>             - Find page by title and show ID")
        print("  list                     - List all pages in space")
        print("  children <page-id>       - List child pages of a parent")
        print("\nExamples:")
        print("  py confluence_fetch.py get 'Game Design Document'")
        print("  py confluence_fetch.py find 'Ship Fuel System'")
        print("  py confluence_fetch.py list")
        print("  py confluence_fetch.py children 123456")
        sys.exit(1)
    
    command = sys.argv[1].lower()
    
    if command == "get":
        if len(sys.argv) < 3:
            print("[ERROR] Usage: py confluence_fetch.py get <page-id-or-title>")
            sys.exit(1)
        page_identifier = sys.argv[2]
        markdown = get_page_content(page_identifier)
        if markdown:
            print(markdown)
        else:
            print("[ERROR] Failed to fetch page content")
            sys.exit(1)
    
    elif command == "find":
        if len(sys.argv) < 3:
            print("[ERROR] Usage: py confluence_fetch.py find <title>")
            sys.exit(1)
        title = sys.argv[2]
        page = find_page_by_title(title)
        if page:
            print(f"\nPage ID: {page['id']}")
            print(f"Title: {page['title']}")
            print(f"Version: {page['version']['number']}")
        sys.exit(0 if page else 1)
    
    elif command == "list":
        pages = list_pages_in_space()
        if pages:
            print(f"\n[OK] Found {len(pages)} pages in {CONFLUENCE_SPACE_KEY} space:\n")
            for page in pages:
                ancestors = page.get('ancestors', [])
                indent = "  " * len(ancestors)
                print(f"{indent}* {page['title']} (ID: {page['id']})")
        else:
            print("[ERROR] No pages found or failed to fetch")
        sys.exit(0)
    
    elif command == "children":
        if len(sys.argv) < 3:
            print("[ERROR] Usage: py confluence_fetch.py children <page-id>")
            sys.exit(1)
        parent_id = sys.argv[2]
        children = get_child_pages(parent_id)
        if children:
            print(f"\n[OK] Found {len(children)} child pages:\n")
            for child in children:
                print(f"  * {child['title']} (ID: {child['id']})")
        else:
            print("[ERROR] No child pages found")
        sys.exit(0)
    
    else:
        print(f"[ERROR] Unknown command: {command}")
        print("Use: get, find, list, or children")
        sys.exit(1)