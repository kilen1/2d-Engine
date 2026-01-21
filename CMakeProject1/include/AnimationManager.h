#pragma once
#include <string>
#include <vector>
#include <memory>

namespace CmakeProject1 {

class AnimationManager {
public:
    static AnimationManager& getInstance();
    
    struct AnimationClip {
        std::string name;
        std::vector<std::string> frames;
        float frameRate;
        bool loop;
    };
    
    bool loadAnimation(const std::string& name, const std::string& filepath);
    AnimationClip* getAnimation(const std::string& name);
    void playAnimation(const std::string& name);
    void stopAnimation(const std::string& name);
    void clearAnimations();

private:
    AnimationManager() = default;
    ~AnimationManager() = default;
    AnimationManager(const AnimationManager&) = delete;
    AnimationManager& operator=(const AnimationManager&) = delete;
    
    std::unordered_map<std::string, std::shared_ptr<AnimationClip>> animations;
};

} // namespace CmakeProject1