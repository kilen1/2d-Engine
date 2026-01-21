#pragma once

#include <SDL2/SDL.h>
#include <memory>

namespace CmakeProject1 {

    struct Vector2 {
        float x = 0.0f;
        float y = 0.0f;

        Vector2() = default;
        Vector2(float x, float y) : x(x), y(y) {}

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

    class Transform {
    public:
        Transform();
        Transform(const Vector2& position, const Vector2& scale = Vector2(1.0f, 1.0f), float rotation = 0.0f);

        void setPosition(const Vector2& pos) { position = pos; }
        void setScale(const Vector2& scl) { scale = scl; }
        void setRotation(float rot) { rotation = rot; }

        Vector2 getPosition() const { return position; }
        Vector2 getScale() const { return scale; }
        float getRotation() const { return rotation; }

        void translate(const Vector2& offset);
        void rotate(float angle);

        SDL_Rect getBounds() const;

    private:
        Vector2 position;
        Vector2 scale;
        float rotation;
    };

} // namespace CmakeProject1