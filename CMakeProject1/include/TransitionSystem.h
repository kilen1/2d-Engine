#pragma once
#include <string>

namespace CmakeProject1 {

class TransitionSystem {
public:
    static TransitionSystem& getInstance();
    
    enum class TransitionType {
        FadeIn,
        FadeOut,
        SlideLeft,
        SlideRight,
        SlideUp,
        SlideDown,
        CircleCrop
    };
    
    void startTransition(TransitionType type, float duration);
    void update(float deltaTime);
    bool isTransitioning() const;
    void render();
    void setTargetScene(const std::string& sceneName);

private:
    TransitionSystem() = default;
    ~TransitionSystem() = default;
    TransitionSystem(const TransitionSystem&) = delete;
    TransitionSystem& operator=(const TransitionSystem&) = delete;
    
    TransitionType currentTransition;
    float duration;
    float currentTime;
    bool transitioning;
    std::string targetScene;
};

} // namespace CmakeProject1