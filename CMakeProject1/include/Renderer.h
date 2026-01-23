#pragma once

#include "Types.h"
#include <memory>
#include <vector>

namespace CmakeProject1 {

    class Renderer {
    public:
        Renderer();
        ~Renderer();

        bool initialize();
        void shutdown();

        void clear(const Color& color);
        void beginFrame();
        void endFrame();
        void drawRect(const Rect& rect, const Color& color);
        void drawSprite(std::shared_ptr<Texture> texture, const Rect& src, const Rect& dest, float rotation = 0.0f);
        void drawLine(const Vector2& start, const Vector2& end, const Color& color);
        void drawCircle(const Vector2& center, float radius, const Color& color);

        void setViewport(int x, int y, int w, int h);
        void resetViewport();

        // Опционально: поддержка шейдеров, буферов и т.д.
    };

} // namespace CmakeProject1