#include "DebugDraw.h"
#include "Renderer.h"
#include <algorithm>

namespace CmakeProject1 {

    DebugDraw::DebugDraw() {}

    DebugDraw::~DebugDraw() {
        shutdown();
    }

    void DebugDraw::initialize(Renderer* renderer) {
        m_renderer = renderer;
    }

    void DebugDraw::shutdown() {
        clear();
    }

    void DebugDraw::drawLine(const Vector2& start, const Vector2& end, const Color& color, float lifetime) {
        m_lines.push_back({ start, end, color, lifetime });
    }

    void DebugDraw::drawRect(const Rect& rect, const Color& color, float lifetime) {
        m_rects.push_back({ rect, color, lifetime });
    }

    void DebugDraw::drawCircle(const Vector2& center, float radius, const Color& color, float lifetime) {
        m_circles.push_back({ center, radius, color, lifetime });
    }

    void DebugDraw::clear() {
        m_lines.clear();
        m_rects.clear();
        m_circles.clear();
    }

    void DebugDraw::update(float deltaTime) {
        // Remove expired lines
        if (deltaTime > 0) {
            auto removeExpired = [deltaTime](auto& container) {
                for (auto& item : container) {
                    if (item.lifetime > 0) {
                        item.lifetime -= deltaTime;
                    }
                }
                container.erase(
                    std::remove_if(container.begin(), container.end(),
                        [](const auto& item) { return item.lifetime <= 0 && item.lifetime != 0; }),
                    container.end()
                );
                };

            removeExpired(m_lines);
            removeExpired(m_rects);
            removeExpired(m_circles);
        }
    }

    void DebugDraw::render() {
        if (!m_renderer) return;

        // Draw lines
        for (const auto& line : m_lines) {
            m_renderer->drawLine(line.start, line.end, line.color);
        }

        // Draw rects
        for (const auto& rect : m_rects) {
            m_renderer->drawRect(rect.rect, rect.color);
        }

        // Draw circles
        for (const auto& circle : m_circles) {
            m_renderer->drawCircle(circle.center, circle.radius, circle.color);
        }
    }

} // namespace CmakeProject1