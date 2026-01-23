#pragma once
#include "Types.h"
#include <memory>
#include <vector>

namespace CmakeProject1 {

    class PhysicsEngine {
    public:
        PhysicsEngine();
        ~PhysicsEngine();

        bool initialize();
        void shutdown();

        void update(float deltaTime);
        void simulate();

        // Collision detection
        void checkCollisions();
        void resolveCollisions();

        // Rigid body management
        void addRigidBody(std::shared_ptr<GameObject> go);
        void removeRigidBody(std::shared_ptr<GameObject> go);

        // Utility
        Vector2 getGravity() const { return m_gravity; }
        void setGravity(const Vector2& gravity) { m_gravity = gravity; }

    private:
        bool m_initialized = false;
        Vector2 m_gravity{ 0, 9.8f };
        std::vector<std::shared_ptr<GameObject>> m_rigidBodies;
    };

} // namespace CmakeProject1