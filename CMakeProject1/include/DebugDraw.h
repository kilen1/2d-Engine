#pragma once
#include "Types.h"
#include <vector>
#include <memory>


namespace CmakeProject1 {

    class Renderer; // Forward declaration

    struct DebugLine {
        Vector2 start;
        Vector2 end;
        Color color;
        float lifetime; // in seconds, 0 = forever
    };

    struct DebugRect {
        Rect rect;
        Color color;
        float lifetime;
    };

    struct DebugCircle {
        Vector2 center;
        float radius;
        Color color;
        float lifetime;
    };

    class DebugDraw {
    public:
        DebugDraw();
        ~DebugDraw();

        void initialize(Renderer* renderer);
        void shutdown();

        void drawLine(const Vector2& start, const Vector2& end, const Color& color, float lifetime = 0.0f);
        void drawRect(const Rect& rect, const Color& color, float lifetime = 0.0f);
        void drawCircle(const Vector2& center, float radius, const Color& color, float lifetime = 0.0f);

        void clear();
        void update(float deltaTime);
        void render();

    private:
        Renderer* m_renderer = nullptr;
        std::vector<DebugLine> m_lines;
        std::vector<DebugRect> m_rects;
        std::vector<DebugCircle> m_circles;
    };

} // namespace CmakeProject1