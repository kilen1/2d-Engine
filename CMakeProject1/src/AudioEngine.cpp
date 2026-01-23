#include "AudioEngine.h"   
#include "AudioClip.h"    
#include <iostream> 

namespace CmakeProject1 {

    AudioEngine::AudioEngine() {}

    AudioEngine::~AudioEngine() {
        shutdown();
    }

    bool AudioEngine::initialize() {
        m_initialized = true;
        std::cout << "AudioEngine initialized." << std::endl;
        return true;
    }

    void AudioEngine::shutdown() {
        m_sounds.clear();
        m_currentMusic = nullptr;
        m_initialized = false;
    }

    void AudioEngine::playSound(std::shared_ptr<AudioClip> clip, float volume, bool loop) {
        if (!clip || !clip->loaded) return;
        std::cout << "Playing sound: " << clip->path << " (volume: " << volume << ", loop: " << loop << ")" << std::endl;
        // In real project: use SDL_mixer or OpenAL
    }

    void AudioEngine::stopSound(std::shared_ptr<AudioClip> clip) {
        std::cout << "Stopping sound: " << clip->path << std::endl;
    }

    void AudioEngine::pauseSound(std::shared_ptr<AudioClip> clip) {
        std::cout << "Pausing sound: " << clip->path << std::endl;
    }

    void AudioEngine::resumeSound(std::shared_ptr<AudioClip> clip) {
        std::cout << "Resuming sound: " << clip->path << std::endl;
    }

    void AudioEngine::setMasterVolume(float volume) {
        m_masterVolume = volume;
        std::cout << "Master volume set to: " << volume << std::endl;
    }

    float AudioEngine::getMasterVolume() const {
        return m_masterVolume;
    }

    void AudioEngine::playMusic(std::shared_ptr<AudioClip> music, bool loop) {
        if (m_currentMusic) stopMusic();
        m_currentMusic = music;
        std::cout << "Playing music: " << music->path << " (loop: " << loop << ")" << std::endl;
    }

    void AudioEngine::stopMusic() {
        if (m_currentMusic) {
            std::cout << "Stopping music: " << m_currentMusic->path << std::endl;
            m_currentMusic = nullptr;
        }
    }

} // namespace CmakeProject1