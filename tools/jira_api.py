"""Jira API Helper for MS Tour"""
import json, base64, sys
from pathlib import Path

def load_env():
    env_file = Path(__file__).parent.parent / '.env'
    if not env_file.exists():
        print("ERROR: .env file not found! See docs/JIRA_API_SETUP.md")
        sys.exit(1)
    env = {}
    for line in open(env_file):
        line = line.strip()
        if line and not line.startswith('#'):
            k, v = line.split('=', 1)
            env[k] = v
    return env

def make_request(method, issue_key, action='get', data=None):
    env = load_env()
    creds = base64.b64encode(f"{env['JIRA_EMAIL']}:{env['JIRA_API_TOKEN']}".encode()).decode()
    base = env['JIRA_BASE_URL']
    
    endpoints = {
        'get': f"/rest/api/3/issue/{issue_key}",
        'comment': f"/rest/api/3/issue/{issue_key}/comment",
        'update': f"/rest/api/3/issue/{issue_key}",
    }
    
    print(f"curl -X {method} \"{base}{endpoints[action]}\" -H \"Authorization: Basic {creds}\" -H \"Content-Type: application/json\"", end="")
    if data:
        print(f" -d '{json.dumps(data)}'")
    else:
        print()

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python jira_api.py <action> <issue-key> [data]")
        sys.exit(1)
    action, issue = sys.argv[1], sys.argv[2]
    make_request('GET' if action == 'get' else 'POST', issue, action)
