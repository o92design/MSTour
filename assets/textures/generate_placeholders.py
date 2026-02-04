from PIL import Image, ImageDraw

# Lighthouse (32x64) - red/white striped
lighthouse = Image.new('RGBA', (32, 64), (0, 0, 0, 0))
draw = ImageDraw.Draw(lighthouse)
draw.rectangle([6, 10, 26, 54], fill=(200, 50, 50, 255))  # Red tower
draw.rectangle([6, 18, 26, 26], fill=(255, 255, 255, 255))  # White stripe
draw.rectangle([6, 34, 26, 42], fill=(255, 255, 255, 255))  # White stripe
draw.rectangle([4, 8, 28, 12], fill=(100, 100, 100, 255))  # Top
lighthouse.save('lighthouse.png')

# Island (64x64) - green/brown
island = Image.new('RGBA', (64, 64), (0, 0, 0, 0))
draw = ImageDraw.Draw(island)
draw.ellipse([10, 25, 54, 55], fill=(120, 90, 60, 255))  # Brown land
draw.ellipse([15, 20, 30, 40], fill=(60, 120, 60, 255))  # Green tree
draw.ellipse([35, 22, 48, 38], fill=(60, 120, 60, 255))  # Green tree
island.save('island.png')

# Water background (256x256) - blue tileable
water = Image.new('RGB', (256, 256), (40, 80, 150))
water.save('water_bg.png')

# Waypoint arrow (16x16)
arrow = Image.new('RGBA', (16, 16), (0, 0, 0, 0))
draw = ImageDraw.Draw(arrow)
draw.polygon([(8, 2), (14, 8), (10, 8), (10, 14), (6, 14), (6, 8), (2, 8)], fill=(255, 200, 0, 255))
arrow.save('waypoint_arrow.png')

# Satisfaction star (16x16)
star = Image.new('RGBA', (16, 16), (0, 0, 0, 0))
draw = ImageDraw.Draw(star)
points = [(8, 2), (10, 6), (14, 6), (11, 9), (13, 13), (8, 11), (3, 13), (5, 9), (2, 6), (6, 6)]
draw.polygon(points, fill=(255, 220, 0, 255))
star.save('satisfaction_star.png')

print('Created 5 placeholder textures:')
print('  - lighthouse.png (32x64)')
print('  - island.png (64x64)')
print('  - water_bg.png (256x256)')
print('  - waypoint_arrow.png (16x16)')
print('  - satisfaction_star.png (16x16)')
