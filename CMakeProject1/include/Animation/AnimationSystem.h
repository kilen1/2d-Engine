#pragma once

#include <string>
#include <vector>

namespace CmakeProject1 {
    class AnimationSystem {
    public:
        virtual ~AnimationSystem() = default;
        virtual bool initialize() = 0;
        virtual void update() = 0;
        virtual void shutdown() = 0;

        virtual void createAnimation(const std::string& name, const std::vector<std::string>& frames) = 0;
        virtual void playAnimation(const std::string& gameObject, const std::string& animation) = 0;
        virtual void stopAnimation(const std::string& gameObject, const std::string& animation) = 0;
        virtual void updateAnimations() = 0;
    };
}