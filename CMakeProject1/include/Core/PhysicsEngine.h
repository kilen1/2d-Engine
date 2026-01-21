#pragma once

#include "../Transform.h"
#include <vector>
#include <memory>

namespace CmakeProject1 {

    struct CollisionInfo {
        bool collided = false;
        Vector2 collisionNormal;
        float penetrationDepth = 0.0f;
    };

    class PhysicsBody {
    public:
        PhysicsBody();
        ~PhysicsBody();

        void setTransform(Vector2 position, Vector2 scale = Vector2(1.0f, 1.0f));
        void setVelocity(Vector2 vel) { velocity = vel; }
        void setMass(float m) { mass = m; }
        void setStatic(bool s) { isStatic = s; }

        Vector2 getPosition() const { return position; }
        Vector2 getVelocity() const { return velocity; }
        Vector2 getScale() const { return scale; }
        float getMass() const { return mass; }
        bool getStatic() const { return isStatic; }

        void update(float deltaTime);

    private:
        Vector2 position;
        Vector2 scale;
        Vector2 velocity;
        float mass = 1.0f;
        bool isStatic = false;
    };

    class PhysicsEngine {
    public:
        PhysicsEngine();
        ~PhysicsEngine();

        void update(float deltaTime);

        void addBody(std::shared_ptr<PhysicsBody> body);
        void removeBody(std::shared_ptr<PhysicsBody> body);

        CollisionInfo checkCollision(const PhysicsBody& bodyA, const PhysicsBody& bodyB) const;

    private:
        std::vector<std::shared_ptr<PhysicsBody>> bodies;
    };

} // namespace CmakeProject1