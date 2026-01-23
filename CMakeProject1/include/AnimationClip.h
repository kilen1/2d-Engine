#pragma once
#include "Types.h"
#include <vector>
#include <string>
#include <memory>

namespace CmakeProject1 {

    struct AnimationFrame {
        std::shared_ptr<Sprite> sprite;
        float duration = 0.1f; // секунды
    };

    class AnimationClip {
    public:
        AnimationClip() = default;
        ~AnimationClip() = default;

        std::string name;
        std::vector<AnimationFrame> frames;
        bool loop = true;
        float totalDuration() const {
            float sum = 0.0f;
            for (const auto& frame : frames) sum += frame.duration;
            return sum;
        }
    };

} // namespace CmakeProject1