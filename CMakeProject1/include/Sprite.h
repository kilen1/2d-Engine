#pragma once
#include "Types.h"
#include <memory>
#include <string>

namespace CmakeProject1 {

    class Texture;

    class Sprite {
    public:
        Sprite() = default;
        ~Sprite() = default;

        std::string name;
        std::shared_ptr<Texture> texture;
        Rect sourceRect; // часть текстуры
        Vector2 pivot{ 0.5f, 0.5f }; // центр по умолчанию
    };

} // namespace CmakeProject1