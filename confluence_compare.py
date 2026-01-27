#!/usr/bin/env python3
"""
General-purpose Confluence page comparison and DRY audit tool
Usage: python confluence_compare.py "Page 1 Title" "Page 2 Title"
"""

import sys
from pathlib import Path
from difflib import SequenceMatcher

# Add tools directory to path
sys.path.insert(0, str(Path(__file__).parent / 'tools'))

import confluence_fetch

def calculate_similarity(text1, text2):
    """Calculate similarity ratio between two texts"""
    return SequenceMatcher(None, text1, text2).ratio()

def find_common_sections(lines1, lines2, min_length=5):
    """Find common sections between two documents"""
    common = []
    
    for i in range(len(lines1) - min_length):
        section1 = '\n'.join(lines1[i:i+min_length])
        for j in range(len(lines2) - min_length):
            section2 = '\n'.join(lines2[j:j+min_length])
            similarity = calculate_similarity(section1, section2)
            
            if similarity > 0.8:  # 80% similar
                common.append({
                    'page1_line': i + 1,
                    'page2_line': j + 1,
                    'similarity': similarity,
                    'content': lines1[i][:100]  # First 100 chars
                })
    
    return common

def main():
    if len(sys.argv) < 3:
        print("=" * 70)
        print("Confluence Page Comparison Tool")
        print("=" * 70)
        print("\nUsage: python confluence_compare.py \"Page 1\" \"Page 2\"")
        print("\nExamples:")
        print("  python confluence_compare.py \"GDD Master\" \"Core Gameplay Loop\"")
        print("  python confluence_compare.py \"Ship System\" \"Fleet Management\"")
        print("\nThis tool analyzes two Confluence pages and identifies:")
        print("  - Overall content similarity")
        print("  - Common sections and duplicates")
        print("  - Recommendations for DRY improvements")
        print()
        return 1
    
    page1_title = sys.argv[1]
    page2_title = sys.argv[2]
    
    print("=" * 70)
    print("Confluence DRY Audit")
    print("=" * 70)
    print(f"\nPage 1: {page1_title}")
    print(f"Page 2: {page2_title}\n")
    
    # Fetch pages
    print("[1/4] Fetching pages...")
    content1 = confluence_fetch.get_page_content(page1_title)
    content2 = confluence_fetch.get_page_content(page2_title)
    
    if not content1:
        print(f"ERROR: Could not fetch '{page1_title}'")
        return 1
    if not content2:
        print(f"ERROR: Could not fetch '{page2_title}'")
        return 1
    
    print(f"  {page1_title}: {len(content1)} chars")
    print(f"  {page2_title}: {len(content2)} chars\n")
    
    # Calculate overall similarity
    print("[2/4] Calculating similarity...")
    similarity = calculate_similarity(content1, content2)
    print(f"  Overall similarity: {similarity*100:.1f}%\n")
    
    # Find common sections
    print("[3/4] Finding common sections...")
    lines1 = content1.split('\n')
    lines2 = content2.split('\n')
    common = find_common_sections(lines1, lines2)
    print(f"  Found {len(common)} similar sections\n")
    
    # Generate report
    print("[4/4] Generating report...")
    print("\n" + "=" * 70)
    print("AUDIT RESULTS")
    print("=" * 70 + "\n")
    
    print(f"Overall Content Overlap: {similarity*100:.1f}%\n")
    
    if similarity > 0.7:
        print("⚠️  HIGH REDUNDANCY DETECTED (>70% overlap)")
        print("   Recommendation: Consider consolidating content\n")
    elif similarity > 0.4:
        print("⚠️  MODERATE REDUNDANCY (40-70% overlap)")
        print("   Recommendation: Review for potential consolidation\n")
    else:
        print("✅ LOW REDUNDANCY (<40% overlap)")
        print("   Pages appear to have distinct content\n")
    
    if common:
        print(f"Common Sections Found: {len(common)}\n")
        print("First 5 duplicates:")
        for idx, item in enumerate(common[:5], 1):
            print(f"\n{idx}. Similarity: {item['similarity']*100:.1f}%")
            print(f"   {page1_title} line {item['page1_line']}")
            print(f"   {page2_title} line {item['page2_line']}")
            print(f"   Content: {item['content']}...")
    
    print("\n" + "=" * 70)
    print("RECOMMENDATIONS")
    print("=" * 70 + "\n")
    
    if similarity > 0.7:
        print("1. Designate ONE page as 'single source of truth'")
        print("2. Move ALL detailed content to that page")
        print("3. Reduce the other page to overview + link")
        print("4. Aim for <10% content overlap")
    elif similarity > 0.4:
        print("1. Identify which sections are truly duplicated")
        print("2. Decide which page should own each concept")
        print("3. Add cross-references instead of duplicating")
        print("4. Keep master document as overview only")
    else:
        print("✅ Content distribution looks good!")
        print("   Continue maintaining clear separation of concerns")
    
    print("\n" + "=" * 70 + "\n")
    
    return 0

if __name__ == '__main__':
    sys.exit(main())
