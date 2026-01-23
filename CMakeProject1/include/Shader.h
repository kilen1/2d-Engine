#pragma once
#include "Types.h"
#include <string>
#include <memory>

namespace CmakeProject1 {

    class Shader {
    public:
        Shader() = default;
        ~Shader() = default;

        std::string vertexPath;
        std::string fragmentPath;
        bool loaded = false;
        // GLuint program; // OpenGL program ID — можно добавить позже
    };

} // namespace CmakeProject1