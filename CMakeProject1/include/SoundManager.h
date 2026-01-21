#pragma once
#include <string>
#include <unordered_map>
#include <memory>

namespace CmakeProject1 {

class SoundManager {
public:
    static SoundManager& getInstance();
    
    struct SoundData {
        std::string name;
        void* soundBuffer; // Pointer to actual sound implementation
    };
    
    bool loadSound(const std::string& name, const std::string& filepath);
    bool playSound(const std::string& name);
    bool playMusic(const std::string& name, bool loop = true);
    void stopSound(const std::string& name);
    void setVolume(float volume);
    void clearSounds();

private:
    SoundManager() = default;
    ~SoundManager() = default;
    SoundManager(const SoundManager&) = delete;
    SoundManager& operator=(const SoundManager&) = delete;
    
    std::unordered_map<std::string, std::shared_ptr<SoundData>> sounds;
    float masterVolume;
};

} // namespace CmakeProject1