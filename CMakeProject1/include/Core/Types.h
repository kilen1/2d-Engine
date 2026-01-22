#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <SDL2/SDL.h>

namespace CmakeProject1 {
    struct Vector2 {
        float x, y;
        Vector2(float x = 0, float y = 0) : x(x), y(y) {}
        
        Vector2 operator+(const Vector2& other) const {
            return Vector2(x + other.x, y + other.y);
        }

        Vector2 operator-(const Vector2& other) const {
            return Vector2(x - other.x, y - other.y);
        }

        Vector2 operator*(float scalar) const {
            return Vector2(x * scalar, y * scalar);
        }
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
        
        // Constructor for Uint8 values (0-255)
        Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255) : r(r/255.0f), g(g/255.0f), b(b/255.0f), a(a/255.0f) {}
        
        // Static color constants
        static const Color White;
        static const Color Black;
        static const Color Red;
        static const Color Green;
        static const Color Blue;
        static const Color Yellow;
        static const Color Cyan;
        static const Color Magenta;
        static const Color Transparent;
    };
    
    // Define color constants
    inline const Color Color::White(1.0f, 1.0f, 1.0f, 1.0f);
    inline const Color Color::Black(0.0f, 0.0f, 0.0f, 1.0f);
    inline const Color Color::Red(1.0f, 0.0f, 0.0f, 1.0f);
    inline const Color Color::Green(0.0f, 1.0f, 0.0f, 1.0f);
    inline const Color Color::Blue(0.0f, 0.0f, 1.0f, 1.0f);
    inline const Color Color::Yellow(1.0f, 1.0f, 0.0f, 1.0f);
    inline const Color Color::Cyan(0.0f, 1.0f, 1.0f, 1.0f);
    inline const Color Color::Magenta(1.0f, 0.0f, 1.0f, 1.0f);
    inline const Color Color::Transparent(0.0f, 0.0f, 0.0f, 0.0f);

    enum class QualityLevel {
        Low,
        Medium,
        High,
        Ultra
    };
}