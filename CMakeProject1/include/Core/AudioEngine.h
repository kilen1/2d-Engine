#pragma once

#include <SDL2/SDL_mixer.h>
#include <unordered_map>
#include <string>
#include <memory>

namespace CmakeProject1 {

    class Sound {
    public:
        Sound(Mix_Chunk* chunk) : soundChunk(chunk) {}
        ~Sound();
        
        Mix_Chunk* getChunk() const { return soundChunk; }

    private:
        Mix_Chunk* soundChunk;
    };

    class Music {
    public:
        Music(Mix_Music* music) : musicTrack(music) {}
        ~Music();
        
        Mix_Music* getMusic() const { return musicTrack; }

    private:
        Mix_Music* musicTrack;
    };

    class AudioEngine {
    public:
        AudioEngine();
        ~AudioEngine();

        bool initialize();
        void shutdown();

        bool loadSound(const std::string& filename, const std::string& id);
        bool loadMusic(const std::string& filename, const std::string& id);

        void playSound(const std::string& id, int loops = 0, int channel = -1);
        void playMusic(const std::string& id, int loops = -1);

        void pauseMusic();
        void resumeMusic();
        void stopMusic();
        void stopAllSounds();

        void setMasterVolume(float volume);
        void setMusicVolume(float volume);
        void setSoundVolume(float volume);

        void removeSound(const std::string& id);
        void removeMusic(const std::string& id);

    private:
        std::unordered_map<std::string, std::shared_ptr<Sound>> sounds;
        std::unordered_map<std::string, std::shared_ptr<Music>> musicTracks;
        float masterVolume = 1.0f;
        float musicVolume = 1.0f;
        float soundVolume = 1.0f;
    };

} // namespace CmakeProject1