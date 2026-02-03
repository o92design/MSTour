#include "engine_audio.h"
#include "engine_math.h"
#include <raylib.h>
#include <string.h>
#include <stdio.h>

bool audio_init(AudioManager* audio) {
    if (!audio) return false;
    
    memset(audio, 0, sizeof(AudioManager));
    
    InitAudioDevice();
    
    if (!IsAudioDeviceReady()) {
        printf("Audio: Failed to initialize audio device\n");
        return false;
    }
    
    audio->master_volume = 1.0f;
    audio->sfx_volume = 1.0f;
    audio->music_volume = 1.0f;
    audio->initialized = true;
    
    printf("Audio: Initialized successfully\n");
    return true;
}

void audio_shutdown(AudioManager* audio) {
    if (!audio || !audio->initialized) return;
    
    // Unload all sounds
    for (int i = 0; i < audio->sound_count; i++) {
        if (audio->sounds[i].loaded) {
            UnloadSound(audio->sounds[i].sound);
            audio->sounds[i].loaded = false;
        }
    }
    
    CloseAudioDevice();
    audio->initialized = false;
    
    printf("Audio: Shutdown complete\n");
}

SoundHandle audio_load_sound(AudioManager* audio, const char* filepath, const char* name) {
    if (!audio || !audio->initialized || !filepath) return INVALID_SOUND_HANDLE;
    if (audio->sound_count >= MAX_AUDIO_SOUNDS) return INVALID_SOUND_HANDLE;
    
    // Check if file exists
    if (!FileExists(filepath)) {
        printf("Audio: File not found: %s\n", filepath);
        return INVALID_SOUND_HANDLE;
    }
    
    // Load the sound
    Sound sound = LoadSound(filepath);
    
    if (sound.frameCount == 0) {
        printf("Audio: Failed to load: %s\n", filepath);
        return INVALID_SOUND_HANDLE;
    }
    
    // Store in manager
    int handle = audio->sound_count;
    audio->sounds[handle].sound = sound;
    audio->sounds[handle].name = name;
    audio->sounds[handle].volume = 1.0f;
    audio->sounds[handle].pitch = 1.0f;
    audio->sounds[handle].loaded = true;
    audio->sounds[handle].looping = false;
    audio->sound_count++;
    
    printf("Audio: Loaded '%s' from %s\n", name, filepath);
    return handle;
}

void audio_unload_sound(AudioManager* audio, SoundHandle handle) {
    if (!audio || handle < 0 || handle >= audio->sound_count) return;
    if (!audio->sounds[handle].loaded) return;
    
    UnloadSound(audio->sounds[handle].sound);
    audio->sounds[handle].loaded = false;
}

void audio_play(AudioManager* audio, SoundHandle handle) {
    if (!audio || !audio->initialized) return;
    if (handle < 0 || handle >= audio->sound_count) return;
    if (!audio->sounds[handle].loaded) return;
    
    AudioSound* snd = &audio->sounds[handle];
    SetSoundVolume(snd->sound, snd->volume * audio->master_volume);
    SetSoundPitch(snd->sound, snd->pitch);
    PlaySound(snd->sound);
}

void audio_play_ex(AudioManager* audio, SoundHandle handle, float volume, float pitch) {
    if (!audio || !audio->initialized) return;
    if (handle < 0 || handle >= audio->sound_count) return;
    if (!audio->sounds[handle].loaded) return;
    
    AudioSound* snd = &audio->sounds[handle];
    SetSoundVolume(snd->sound, volume * audio->master_volume);
    SetSoundPitch(snd->sound, pitch);
    PlaySound(snd->sound);
}

void audio_stop(AudioManager* audio, SoundHandle handle) {
    if (!audio || handle < 0 || handle >= audio->sound_count) return;
    if (!audio->sounds[handle].loaded) return;
    
    StopSound(audio->sounds[handle].sound);
}

void audio_pause(AudioManager* audio, SoundHandle handle) {
    if (!audio || handle < 0 || handle >= audio->sound_count) return;
    if (!audio->sounds[handle].loaded) return;
    
    PauseSound(audio->sounds[handle].sound);
}

void audio_resume(AudioManager* audio, SoundHandle handle) {
    if (!audio || handle < 0 || handle >= audio->sound_count) return;
    if (!audio->sounds[handle].loaded) return;
    
    ResumeSound(audio->sounds[handle].sound);
}

bool audio_is_playing(const AudioManager* audio, SoundHandle handle) {
    if (!audio || handle < 0 || handle >= audio->sound_count) return false;
    if (!audio->sounds[handle].loaded) return false;
    
    return IsSoundPlaying(audio->sounds[handle].sound);
}

void audio_set_volume(AudioManager* audio, SoundHandle handle, float volume) {
    if (!audio || handle < 0 || handle >= audio->sound_count) return;
    
    audio->sounds[handle].volume = math_clamp(volume, 0.0f, 1.0f);
    
    // Update sound if loaded
    if (audio->sounds[handle].loaded) {
        float actual = audio->sounds[handle].volume * audio->master_volume;
        SetSoundVolume(audio->sounds[handle].sound, actual);
    }
}

void audio_set_pitch(AudioManager* audio, SoundHandle handle, float pitch) {
    if (!audio || handle < 0 || handle >= audio->sound_count) return;
    
    audio->sounds[handle].pitch = math_clamp(pitch, 0.1f, 4.0f);
    
    if (audio->sounds[handle].loaded) {
        SetSoundPitch(audio->sounds[handle].sound, audio->sounds[handle].pitch);
    }
}

void audio_set_looping(AudioManager* audio, SoundHandle handle, bool looping) {
    if (!audio || handle < 0 || handle >= audio->sound_count) return;
    audio->sounds[handle].looping = looping;
}

void audio_update(AudioManager* audio) {
    if (!audio || !audio->initialized) return;
    
    // Restart looping sounds that have stopped
    for (int i = 0; i < audio->sound_count; i++) {
        if (audio->sounds[i].loaded && audio->sounds[i].looping) {
            if (!IsSoundPlaying(audio->sounds[i].sound)) {
                PlaySound(audio->sounds[i].sound);
            }
        }
    }
}

void audio_set_master_volume(AudioManager* audio, float volume) {
    if (!audio) return;
    audio->master_volume = math_clamp(volume, 0.0f, 1.0f);
    
    // Update all loaded sounds
    for (int i = 0; i < audio->sound_count; i++) {
        if (audio->sounds[i].loaded) {
            float actual = audio->sounds[i].volume * audio->master_volume;
            SetSoundVolume(audio->sounds[i].sound, actual);
        }
    }
}

float audio_get_master_volume(const AudioManager* audio) {
    if (!audio) return 0.0f;
    return audio->master_volume;
}

SoundHandle audio_find_sound(const AudioManager* audio, const char* name) {
    if (!audio || !name) return INVALID_SOUND_HANDLE;
    
    for (int i = 0; i < audio->sound_count; i++) {
        if (audio->sounds[i].name && strcmp(audio->sounds[i].name, name) == 0) {
            return i;
        }
    }
    return INVALID_SOUND_HANDLE;
}

float audio_get_actual_volume(const AudioManager* audio, SoundHandle handle) {
    if (!audio || handle < 0 || handle >= audio->sound_count) return 0.0f;
    return audio->sounds[handle].volume * audio->master_volume;
}
