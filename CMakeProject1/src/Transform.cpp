#include "../include/Transform.h"

namespace CmakeProject1 {

    Transform::Transform() : position(0.0f, 0.0f), scale(1.0f, 1.0f), rotation(0.0f) {}

    Transform::Transform(const Vector2& position, const Vector2& scale, float rotation)
        : position(position), scale(scale), rotation(rotation) {
    }

    void Transform::translate(const Vector2& offset) {
        position.x += offset.x;
        position.y += offset.y;
    }

    void Transform::rotate(float angle) {
        rotation += angle;
    }

    SDL_Rect Transform::getBounds() const {
        SDL_Rect bounds;
        bounds.x = static_cast<int>(position.x);
        bounds.y = static_cast<int>(position.y);
        bounds.w = static_cast<int>(scale.x * 32); // Assuming default size of 32x32
        bounds.h = static_cast<int>(scale.y * 32);
        return bounds;
    }

} // namespace CmakeProject1