"""
Confluence Audit Script
Gathers all GDD pages for analysis
"""
import sys
import json
from pathlib import Path

# Import the confluence_fetch functions
from confluence_fetch import (
    list_pages_in_space,
    get_child_pages,
    find_page_by_title,
    get_page_content,
    get_page
)

def audit_confluence_space():
    """Run comprehensive audit of Confluence GDD pages"""
    print("=" * 80)
    print("CONFLUENCE GDD AUDIT")
    print("=" * 80)
    
    # 1. List all pages in space
    print("\n[1/4] Listing all pages in MS space...")
    all_pages = list_pages_in_space()
    
    if not all_pages:
        print("[ERROR] Failed to fetch pages")
        return None
    
    print(f"[OK] Found {len(all_pages)} pages\n")
    for page in all_pages:
        ancestors = page.get('ancestors', [])
        indent = "  " * len(ancestors)
        print(f"{indent}* {page['title']} (ID: {page['id']})")
    
    # 2. Find GDD Master (root page)
    print("\n[2/4] Finding GDD Master (root page)...")
    gdd_master_page = find_page_by_title("GDD Master")
    
    if not gdd_master_page:
        print("[ERROR] GDD Master page not found")
        return None
    
    print(f"[OK] Found GDD Master: (ID: {gdd_master_page['id']})")
    
    # 3. Get child pages of GDD Master
    print("\n[3/4] Fetching child pages of GDD Master...")
    children = get_child_pages(gdd_master_page['id'])
    print(f"[OK] Found {len(children)} child pages:")
    for child in children:
        print(f"  * {child['title']} (ID: {child['id']})")
    
    # 4. Fetch GDD Master content
    print("\n[4/4] Fetching GDD Master and Core Gameplay Loop content...")
    master_content = get_page_content(gdd_master_page['id'])
    print(f"[OK] Fetched GDD Master ({len(master_content)} chars)")
    
    # Find and fetch Core Gameplay Loop
    core_loop_page = find_page_by_title("Core Gameplay Loop")
    if core_loop_page:
        loop_content = get_page_content(core_loop_page['id'])
        print(f"[OK] Fetched Core Gameplay Loop ({len(loop_content)} chars)")
    else:
        print("[WARN] Core Gameplay Loop page not found")
        loop_content = None
        core_loop_page = None
    
    # Compile results
    results = {
        'all_pages': all_pages,
        'gdd_master': {
            'page': gdd_master_page,
            'content': master_content
        },
        'children': children,
        'core_loop': {
            'page': core_loop_page,
            'content': loop_content
        }
    }
    
    print("\n" + "=" * 80)
    print("AUDIT COMPLETE")
    print("=" * 80)
    
    return results

def save_audit_results(results, output_dir):
    """Save audit results to files for analysis"""
    output_path = Path(output_dir)
    output_path.mkdir(exist_ok=True)
    
    # Save page list
    with open(output_path / "page_list.json", 'w', encoding='utf-8') as f:
        pages_data = [
            {
                'id': p['id'],
                'title': p['title'],
                'version': p.get('version', {}).get('number', 0),
                'ancestors': len(p.get('ancestors', []))
            }
            for p in results['all_pages']
        ]
        json.dump(pages_data, f, indent=2)
    
    # Save GDD Master content
    if results['gdd_master']['content']:
        with open(output_path / "gdd_master.md", 'w', encoding='utf-8') as f:
            f.write(results['gdd_master']['content'])
    
    # Save Core Gameplay Loop content
    if results['core_loop']['content']:
        with open(output_path / "core_gameplay_loop.md", 'w', encoding='utf-8') as f:
            f.write(results['core_loop']['content'])
    
    # Save children summary
    with open(output_path / "children_pages.json", 'w', encoding='utf-8') as f:
        children_data = [
            {
                'id': p['id'],
                'title': p['title'],
                'version': p.get('version', {}).get('number', 0)
            }
            for p in results['children']
        ]
        json.dump(children_data, f, indent=2)
    
    print(f"\n[OK] Audit results saved to: {output_path}")
    print(f"   - page_list.json: All pages in space")
    print(f"   - gdd_master.md: GDD Master content")
    print(f"   - core_gameplay_loop.md: Core Gameplay Loop content")
    print(f"   - children_pages.json: Child pages of GDD Master")

if __name__ == "__main__":
    # Run audit
    results = audit_confluence_space()
    
    if results:
        # Save to audit output folder
        output_dir = Path(__file__).parent / "audit_output"
        save_audit_results(results, output_dir)
        print("\n[OK] Ready for analysis")
    else:
        print("\n[ERROR] Audit failed")
        sys.exit(1)
