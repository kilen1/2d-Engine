#pragma once

#include "../Core/Types.h"
#include <glm/glm.hpp>

namespace CmakeProject1 {
    class SpriteBatch {
    public:
        virtual ~SpriteBatch() = default;
        virtual bool initialize() = 0;
        virtual void update() = 0;
        virtual void shutdown() = 0;

        virtual void begin() = 0;
        virtual void draw(const std::string& texture, const Vector2& position, float rotation = 0.0f, const Vector2& scale = Vector2(1, 1)) = 0;
        virtual void end() = 0;
    };
}