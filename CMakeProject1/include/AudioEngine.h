#pragma once
#include "Types.h"
#include <string>
#include <memory>
#include <unordered_map>

namespace CmakeProject1 {

    class AudioEngine {
    public:
        AudioEngine();
        ~AudioEngine();

        bool initialize();
        void shutdown();

        void playSound(std::shared_ptr<AudioClip> clip, float volume = 1.0f, bool loop = false);
        void stopSound(std::shared_ptr<AudioClip> clip);
        void pauseSound(std::shared_ptr<AudioClip> clip);
        void resumeSound(std::shared_ptr<AudioClip> clip);

        void setMasterVolume(float volume);
        float getMasterVolume() const;

        // Music
        void playMusic(std::shared_ptr<AudioClip> music, bool loop = true);
        void stopMusic();

        // Utility
        bool isInitialized() const { return m_initialized; }

    private:
        bool m_initialized = false;
        float m_masterVolume = 1.0f;
        std::shared_ptr<AudioClip> m_currentMusic;
        std::unordered_map<std::string, std::shared_ptr<AudioClip>> m_sounds;
    };

} // namespace CmakeProject1