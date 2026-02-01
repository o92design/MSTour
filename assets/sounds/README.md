# Audio Assets for MS Tour

This directory contains sound effects for the ship control system.

## Required Audio Files

### telegraph_bell..mp3
- **Purpose**: Played when telegraph order changes (W/S keys)
- **Description**: Ship's engine telegraph bell sound
- **Duration**: ~1 second
- **Format**: .mp3, 44.1kHz or 48kHz, mono or stereo
- **Historical Reference**: Brass bell sound from 1887-1950s ships

### engine_loop..mp3
- **Purpose**: Continuous engine sound, pitch varies with ship speed
- **Description**: Steam/diesel engine rumble (looping)
- **Duration**: 2-5 seconds (must loop seamlessly)
- **Format**: .mp3, 44.1kHz or 48kHz, mono preferred
- **Pitch Range**: 0.8x to 1.4x (code adjusts based on speed)
- **Note**: Should loop without clicks or pops

### water_ambient..mp3
- **Purpose**: Ambient water/.mp3e sounds
- **Description**: Ocean .mp3es, water lapping against hull
- **Duration**: 5-10 seconds (must loop seamlessly)
- **Format**: .mp3, 44.1kHz or 48kHz, stereo preferred
- **Note**: Should be subtle background ambiance

## Volume Levels (set in code)
- **Telegraph Bell**: 100% (default)
- **Engine Sound**: 30% (continuous, not overpowering)
- **Water Ambient**: 20% (background ambiance)

## Implementation Notes
- All sounds gracefully degrade if files are missing (warnings logged)
- Engine pitch adjusts from 0.8 (stopped) to 1.4 (max speed)
- Telegraph bell plays on every order change
- Audio system initializes on game startup

## Sources
For prototyping, consider:
- **Freesound.org**: Search "ship bell", "steam engine", "ocean .mp3s"
- **OpenGameArt.org**: Maritime sounds
- **Zapsplat.com**: Free sound effects (attribution required)

Ensure you have appropriate licenses for all audio assets used.
