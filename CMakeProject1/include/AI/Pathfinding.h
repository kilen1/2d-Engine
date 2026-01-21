#pragma once

#include "../Core/Types.h"
#include <glm/glm.hpp>
#include <vector>

namespace CmakeProject1 {
    class Pathfinding {
    public:
        virtual ~Pathfinding() = default;
        virtual bool initialize() = 0;
        virtual void update() = 0;
        virtual void shutdown() = 0;

        virtual std::vector<Vector2> findPath(const Vector2& start, const Vector2& end) = 0;
        virtual void updateObstacles() = 0;
    };
}