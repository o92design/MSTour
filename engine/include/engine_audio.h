#ifndef ENGINE_AUDIO_H
#define ENGINE_AUDIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <raylib.h>
#include <stdbool.h>

// =============================================================================
// Audio Manager
// Provides audio abstraction with sound management and volume control
// =============================================================================

// Maximum sounds that can be managed
#define MAX_AUDIO_SOUNDS 64

// Sound handle (returned by audio_load_sound)
typedef int SoundHandle;
#define INVALID_SOUND_HANDLE -1

// Sound entry in the manager
typedef struct AudioSound {
    Sound sound;
    const char* name;
    float volume;
    float pitch;
    bool loaded;
    bool looping;
} AudioSound;

// Audio manager state
typedef struct AudioManager {
    AudioSound sounds[MAX_AUDIO_SOUNDS];
    int sound_count;
    float master_volume;
    float sfx_volume;
    float music_volume;
    bool initialized;
} AudioManager;

// Initialize audio manager (call after engine_init)
bool audio_init(AudioManager* audio);

// Shutdown audio manager
void audio_shutdown(AudioManager* audio);

// Load a sound from file
// Returns INVALID_SOUND_HANDLE on failure
SoundHandle audio_load_sound(AudioManager* audio, const char* filepath, const char* name);

// Unload a sound
void audio_unload_sound(AudioManager* audio, SoundHandle handle);

// Play a sound
void audio_play(AudioManager* audio, SoundHandle handle);

// Play a sound with specific volume and pitch
void audio_play_ex(AudioManager* audio, SoundHandle handle, float volume, float pitch);

// Stop a sound
void audio_stop(AudioManager* audio, SoundHandle handle);

// Pause a sound
void audio_pause(AudioManager* audio, SoundHandle handle);

// Resume a paused sound
void audio_resume(AudioManager* audio, SoundHandle handle);

// Check if a sound is playing
bool audio_is_playing(const AudioManager* audio, SoundHandle handle);

// Set sound volume (0.0 to 1.0)
void audio_set_volume(AudioManager* audio, SoundHandle handle, float volume);

// Set sound pitch (1.0 = normal)
void audio_set_pitch(AudioManager* audio, SoundHandle handle, float pitch);

// Enable/disable looping for a sound
void audio_set_looping(AudioManager* audio, SoundHandle handle, bool looping);

// Update looping sounds (call each frame)
void audio_update(AudioManager* audio);

// Set master volume (affects all sounds)
void audio_set_master_volume(AudioManager* audio, float volume);

// Get master volume
float audio_get_master_volume(const AudioManager* audio);

// Find sound by name
SoundHandle audio_find_sound(const AudioManager* audio, const char* name);

// Get actual volume (combines sound volume with master)
float audio_get_actual_volume(const AudioManager* audio, SoundHandle handle);

#ifdef __cplusplus
}
#endif

#endif // ENGINE_AUDIO_H
