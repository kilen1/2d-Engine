#pragma once
#include "Types.h"
#include <string>
#include <memory>

namespace CmakeProject1 {

    class Texture {
    public:
        Texture() = default;
        ~Texture() = default;

        std::string path;
        int width = 0;
        int height = 0;
        bool loaded = false;
        // GLuint handle; // OpenGL handle — можно добавить позже
    };

} // namespace CmakeProject1