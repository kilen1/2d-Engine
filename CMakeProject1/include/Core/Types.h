#pragma once

#include <glm/glm.hpp>

namespace CmakeProject1 {
    struct Vector2 {
        float x, y;
        Vector2(float x = 0, float y = 0) : x(x), y(y) {}
    };

    struct Color {
        float r, g, b, a;
        Color(float r = 1, float g = 1, float b = 1, float a = 1) : r(r), g(g), b(b), a(a) {}
    };

    enum class QualityLevel {
        Low,
        Medium,
        High,
        Ultra
    };
}