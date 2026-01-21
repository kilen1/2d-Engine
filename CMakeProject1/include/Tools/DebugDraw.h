#pragma once

#include "../Core/Types.h"
#include <glm/glm.hpp>

struct SDL_Rect;

namespace CmakeProject1 {
    class DebugDraw {
    public:
        virtual ~DebugDraw() = default;
        virtual bool initialize() = 0;
        virtual void update() = 0;
        virtual void shutdown() = 0;

        virtual void drawLine(const Vector2& start, const Vector2& end, const Color& color) = 0;
        virtual void drawCircle(const Vector2& center, float radius, const Color& color) = 0;
        virtual void drawRect(const SDL_Rect& rect, const Color& color) = 0;
        virtual void render() = 0;
    };
}