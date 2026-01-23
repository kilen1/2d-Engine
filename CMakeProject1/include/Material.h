#pragma once
#include "Types.h"
#include <memory>
#include <string>
#include <unordered_map>

namespace CmakeProject1 {

    class Shader;
    class Texture;

    class Material {
    public:
        Material() = default;
        ~Material() = default;

        std::string name;
        std::shared_ptr<Shader> shader;
        std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
        std::unordered_map<std::string, Color> colors;
        std::unordered_map<std::string, float> floats;
        bool transparent = false;
    };

} // namespace CmakeProject1