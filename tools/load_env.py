"""
Load Environment Variables Tool
Loads environment variables from a .env file
"""

from pathlib import Path

def load_env():
    env_path = Path(__file__).parent.parent / '.env'
    env_vars = {}
    if env_path.exists():
        with open(env_path) as f:
            for line in f:
                line = line.strip()
                if line and not line.startswith('#') and '=' in line:
                    key, value = line.split('=', 1)
                    env_vars[key] = value
    return env_vars