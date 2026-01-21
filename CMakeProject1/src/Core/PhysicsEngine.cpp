#include "../../include/Core/PhysicsEngine.h"
#include <algorithm>
#include <cmath>

namespace CmakeProject1 {

    // PhysicsBody Implementation
    PhysicsBody::PhysicsBody() {}

    PhysicsBody::~PhysicsBody() {}

    void PhysicsBody::setTransform(Vector2 position, Vector2 scale) {
        this->position = position;
        this->scale = scale;
    }

    void PhysicsBody::update(float deltaTime) {
        if (!isStatic) {
            // Simple Euler integration
            position.x += velocity.x * deltaTime;
            position.y += velocity.y * deltaTime;
        }
    }

    // PhysicsEngine Implementation
    PhysicsEngine::PhysicsEngine() {}

    PhysicsEngine::~PhysicsEngine() {}

    void PhysicsEngine::update(float deltaTime) {
        // Update all physics bodies
        for (auto& body : bodies) {
            body->update(deltaTime);
        }

        // Simple collision detection
        for (size_t i = 0; i < bodies.size(); ++i) {
            for (size_t j = i + 1; j < bodies.size(); ++j) {
                auto collision = checkCollision(*bodies[i], *bodies[j]);
                if (collision.collided) {
                    // Handle collision response here
                    // For now, just prevent overlap
                    if (!bodies[i]->getStatic() && !bodies[j]->getStatic()) {
                        // Both dynamic: move both apart
                        float separation = collision.penetrationDepth * 0.5f;
                        Vector2 correction = collision.collisionNormal * separation;
                        
                        bodies[i]->setTransform(
                            bodies[i]->getPosition() + correction,
                            bodies[i]->getScale()
                        );
                        bodies[j]->setTransform(
                            bodies[j]->getPosition() - correction,
                            bodies[j]->getScale()
                        );
                    } else if (bodies[i]->getStatic()) {
                        // Move only the dynamic body
                        Vector2 correction = collision.collisionNormal * collision.penetrationDepth;
                        bodies[j]->setTransform(
                            bodies[j]->getPosition() - correction,
                            bodies[j]->getScale()
                        );
                    } else {
                        // Move only the dynamic body
                        Vector2 correction = collision.collisionNormal * collision.penetrationDepth;
                        bodies[i]->setTransform(
                            bodies[i]->getPosition() + correction,
                            bodies[i]->getScale()
                        );
                    }
                }
            }
        }
    }

    void PhysicsEngine::addBody(std::shared_ptr<PhysicsBody> body) {
        bodies.push_back(body);
    }

    void PhysicsEngine::removeBody(std::shared_ptr<PhysicsBody> body) {
        bodies.erase(
            std::remove(bodies.begin(), bodies.end(), body),
            bodies.end()
        );
    }

    CollisionInfo PhysicsEngine::checkCollision(const PhysicsBody& bodyA, const PhysicsBody& bodyB) const {
        CollisionInfo info;
        
        // AABB collision detection
        float aLeft = bodyA.getPosition().x;
        float aRight = bodyA.getPosition().x + bodyA.getScale().x;
        float aTop = bodyA.getPosition().y;
        float aBottom = bodyA.getPosition().y + bodyA.getScale().y;
        
        float bLeft = bodyB.getPosition().x;
        float bRight = bodyB.getPosition().x + bodyB.getScale().x;
        float bTop = bodyB.getPosition().y;
        float bBottom = bodyB.getPosition().y + bodyB.getScale().y;
        
        // Check if rectangles overlap
        if (aLeft < bRight && aRight > bLeft && aTop < bBottom && aBottom > bTop) {
            info.collided = true;
            
            // Calculate overlap in each direction
            float overlapX = std::min(aRight, bRight) - std::max(aLeft, bLeft);
            float overlapY = std::min(aBottom, bBottom) - std::max(aTop, bTop);
            
            // Determine collision normal and penetration depth
            if (overlapX < overlapY) {
                // Horizontal collision
                info.penetrationDepth = overlapX;
                if (bodyA.getPosition().x < bodyB.getPosition().x) {
                    info.collisionNormal = Vector2(-1.0f, 0.0f);
                } else {
                    info.collisionNormal = Vector2(1.0f, 0.0f);
                }
            } else {
                // Vertical collision
                info.penetrationDepth = overlapY;
                if (bodyA.getPosition().y < bodyB.getPosition().y) {
                    info.collisionNormal = Vector2(0.0f, -1.0f);
                } else {
                    info.collisionNormal = Vector2(0.0f, 1.0f);
                }
            }
        }
        
        return info;
    }

} // namespace CmakeProject1