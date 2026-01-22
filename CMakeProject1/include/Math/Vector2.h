#pragma once
#include <cmath>

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
        
        Vector2& operator+=(const Vector2& other) {
            x += other.x;
            y += other.y;
            return *this;
        }
        
        Vector2& operator-=(const Vector2& other) {
            x -= other.x;
            y -= other.y;
            return *this;
        }
        
        Vector2& operator*=(float scalar) {
            x *= scalar;
            y *= scalar;
            return *this;
        }
        
        float magnitude() const {
            return sqrt(x*x + y*y);
        }
        
        float magnitudeSquared() const {
            return x*x + y*y;
        }
        
        Vector2 normalized() const {
            float mag = magnitude();
            if (mag == 0) return Vector2(0, 0);
            return Vector2(x/mag, y/mag);
        }
    };

} // namespace CmakeProject1