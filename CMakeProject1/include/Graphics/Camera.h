#pragma once

#include "../Core/Types.h"
#include <glm/glm.hpp>

namespace CmakeProject1 {
    class Camera {
    public:
        virtual ~Camera() = default;
        virtual bool initialize() = 0;
        virtual void update() = 0;
        virtual void shutdown() = 0;

        virtual void setPosition(const Vector2& pos) = 0;
        virtual Vector2 getPosition() const = 0;
        virtual void setZoom(float zoom) = 0;
        virtual float getZoom() const = 0;
        virtual glm::mat4 getViewMatrix() const = 0;
    };
}