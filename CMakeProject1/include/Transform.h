#pragma once

#include <SDL2/SDL.h>
#include <memory>
#include "Core/Types.h"

namespace CmakeProject1 {

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