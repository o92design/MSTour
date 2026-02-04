# Test Route Assets - Placeholders

## Status: Assets Not Yet Created

The following assets are required for MST-24 but have not been generated yet. The POI system can function without them (using colored rectangles as fallback), or they can be created manually/sourced later.

### Required Assets

#### POI Sprites
- **lighthouse.png** (32x64) - Red/white striped lighthouse for Vinga
- **island.png** (64x64) - Green/brown rocky island for Hönö Klåva

#### Background
- **water_bg.png** (256x256) - Blue tileable water texture

#### UI Icons
- **waypoint_arrow.png** (16x16) - Yellow arrow for waypoint indicator
- **satisfaction_star.png** (16x16) - Yellow star for satisfaction rating

### Temporary Solution

The game can render POIs without these assets by:
1. Using Raylib's `DrawRectangle()` with colored placeholders
2. Using Raylib's `GenImageColor()` to create runtime textures
3. Drawing simple shapes (circles, triangles) as stand-ins

### Asset Generation Options

#### Option 1: Manual Creation
Use any image editor (GIMP, Paint.NET, Aseprite) to create simple pixel art sprites matching the specifications above.

#### Option 2: Python Script (requires Pillow)
```bash
pip install pillow
py generate_placeholders.py
```

#### Option 3: In-Game Generation
Add code to create textures at runtime using Raylib image functions.

### Integration

Once created, POI system will load textures from:
- `./assets/textures/lighthouse.png`
- `./assets/textures/island.png`
- etc.

If files don't exist, fallback rendering should be used.
