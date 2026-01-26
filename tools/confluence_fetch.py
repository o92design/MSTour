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
        print("❌ Missing credentials. Set ATLASSIAN_EMAIL and ATLASSIAN_API_TOKEN in .env")
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
        print(f"❌ Failed to fetch page: {e}")
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
            print(f"✅ Found page: '{page['title']}' (ID: {page['id']})")
            return page
        else:
            print(f"❌ No page found with title: '{title}'")
            return None
    except requests.exceptions.RequestException as e:
        print(f"❌ Search failed: {e}")
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
        print(f"❌ Failed to fetch page content: {e}")
        return None

def create_page(title, content, parent_id):
    """Create a new Confluence page as child of parent"""
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
        'ancestors': [{'id': str(parent_id)}],
        'body': {
            'storage': {
                'value': content,
                'representation': 'storage'
            }
        }
    }
    
    try:
        response = requests.post(url, json=payload, headers=headers)
        response.raise_for_status()
        result = response.json()
        print(f"✅ Created page: '{title}' (ID: {result['id']})")
        return result
    except requests.exceptions.RequestException as e:
        print(f"❌ Failed to create page: {e}")
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
        print(f"❌ Failed to update page: {e}")
        if hasattr(e, 'response') and e.response:
            print(f"   Response: {e.response.text}")
        return None

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python confluence_fetch.py <page-id-or-title>")
        print("\nExamples:")
        print("  python confluence_fetch.py 123456789")
        print("  python confluence_fetch.py 'Game Design Document'")
        sys.exit(1)
    
    page_identifier = sys.argv[1]
    markdown = get_page_content(page_identifier)
    
    if markdown:
        print(markdown)
    else:
        print("❌ Failed to fetch page content")
        sys.exit(1)