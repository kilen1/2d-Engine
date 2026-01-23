#pragma once
#include "Types.h"
#include <string>
#include <memory>

namespace CmakeProject1 {

    class AudioClip {
    public:
        AudioClip() = default;
        ~AudioClip() = default;

        std::string path;
        bool loaded = false;
        bool isMusic = false;
        // void* data; // например, Mix_Chunk* или Mix_Music* — можно добавить позже
    };

} // namespace CmakeProject1