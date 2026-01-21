#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace CmakeProject1 {
    struct Vector2 {
        float x, y;
        Vector2(float x = 0, float y = 0) : x(x), y(y) {}
    };

    struct Vector3 {
        float x, y, z;
        Vector3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
    };

    using Quaternion = glm::quat;

    struct Rect {
        float x, y, width, height;
        Rect(float x = 0, float y = 0, float width = 0, float height = 0) : x(x), y(y), width(width), height(height) {}
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