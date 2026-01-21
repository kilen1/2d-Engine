#pragma once

#include "../Core/Types.h"
#include <glm/glm.hpp>
#include <string>

namespace CmakeProject1 {
    class LightSystem {
    public:
        virtual ~LightSystem() = default;
        virtual bool initialize() = 0;
        virtual void update() = 0;
        virtual void shutdown() = 0;

        virtual void addLight(const Vector2& position, float radius, const Color& color) = 0;
        virtual void removeLight(const std::string& lightId) = 0;
        virtual void updateLight(const std::string& lightId, const Vector2& position, float radius, const Color& color) = 0;
    };
}