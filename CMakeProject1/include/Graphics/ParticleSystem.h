#pragma once

#include "../Core/Types.h"
#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace CmakeProject1 {
    struct Particle {
        Vector2 position;
        Vector2 velocity;
        Color color;
        float life = 0.0f;
        float maxLife = 1.0f;
    };

    class ParticleSystem {
    public:
        virtual ~ParticleSystem() = default;
        virtual bool initialize() = 0;
        virtual void update() = 0;
        virtual void shutdown() = 0;

        virtual void emit(const std::string& systemName, const Vector2& position, int count = 100) = 0;
        virtual void stop(const std::string& systemName) = 0;
        virtual void render() = 0;
    };
}