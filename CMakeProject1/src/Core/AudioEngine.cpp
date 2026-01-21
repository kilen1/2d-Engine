#include "../../include/Core/AudioEngine.h"
#include <iostream>

namespace CmakeProject1 {

    // Sound implementation
    Sound::~Sound() {
        if (soundChunk) {
            Mix_FreeChunk(soundChunk);
        }
    }

    // Music implementation
    Music::~Music() {
        if (musicTrack) {
            Mix_FreeMusic(musicTrack);
        }
    }

    // AudioEngine implementation
    AudioEngine::AudioEngine() {}

    AudioEngine::~AudioEngine() {
        shutdown();
    }

    bool AudioEngine::initialize() {
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
            return false;
        }

        // Initialize MP3 and OGG support
        int flags = MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_FLAC | MIX_INIT_MOD;
        if ((Mix_Init(flags) & flags) != flags) {
            std::cerr << "Warning: Some audio formats not supported by SDL_mixer! SDL_mixer Error: " << Mix_GetError() << std::endl;
        }

        // Allocate channels
        Mix_AllocateChannels(32);

        return true;
    }

    void AudioEngine::shutdown() {
        sounds.clear();
        musicTracks.clear();

        Mix_CloseAudio();
        Mix_Quit();
    }

    bool AudioEngine::loadSound(const std::string& filename, const std::string& id) {
        Mix_Chunk* chunk = Mix_LoadWAV(filename.c_str());
        if (!chunk) {
            std::cerr << "Failed to load sound: " << filename << " - " << Mix_GetError() << std::endl;
            return false;
        }

        sounds[id] = std::make_shared<Sound>(chunk);
        return true;
    }

    bool AudioEngine::loadMusic(const std::string& filename, const std::string& id) {
        Mix_Music* music = Mix_LoadMUS(filename.c_str());
        if (!music) {
            std::cerr << "Failed to load music: " << filename << " - " << Mix_GetError() << std::endl;
            return false;
        }

        musicTracks[id] = std::make_shared<Music>(music);
        return true;
    }

    void AudioEngine::playSound(const std::string& id, int loops, int channel) {
        auto it = sounds.find(id);
        if (it != sounds.end()) {
            int vol = static_cast<int>(MIX_MAX_VOLUME * soundVolume * masterVolume);
            Mix_VolumeChunk(it->second->getChunk(), vol);
            
            Mix_PlayChannel(channel, it->second->getChunk(), loops);
        }
    }

    void AudioEngine::playMusic(const std::string& id, int loops) {
        auto it = musicTracks.find(id);
        if (it != musicTracks.end()) {
            int vol = static_cast<int>(MIX_MAX_VOLUME * musicVolume * masterVolume);
            Mix_VolumeMusic(vol);
            
            Mix_PlayMusic(it->second->getMusic(), loops);
        }
    }

    void AudioEngine::pauseMusic() {
        Mix_PauseMusic();
    }

    void AudioEngine::resumeMusic() {
        Mix_ResumeMusic();
    }

    void AudioEngine::stopMusic() {
        Mix_HaltMusic();
    }

    void AudioEngine::stopAllSounds() {
        Mix_HaltChannel(-1);
    }

    void AudioEngine::setMasterVolume(float volume) {
        masterVolume = std::max(0.0f, std::min(1.0f, volume));
    }

    void AudioEngine::setMusicVolume(float volume) {
        musicVolume = std::max(0.0f, std::min(1.0f, volume));
    }

    void AudioEngine::setSoundVolume(float volume) {
        soundVolume = std::max(0.0f, std::min(1.0f, volume));
    }

    void AudioEngine::removeSound(const std::string& id) {
        sounds.erase(id);
    }

    void AudioEngine::removeMusic(const std::string& id) {
        musicTracks.erase(id);
    }

} // namespace CmakeProject1