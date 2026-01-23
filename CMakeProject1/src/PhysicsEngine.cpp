#include "PhysicsEngine.h"
#include <iostream>

namespace CmakeProject1 {

    PhysicsEngine::PhysicsEngine() {}

    PhysicsEngine::~PhysicsEngine() {
        shutdown();
    }

    bool PhysicsEngine::initialize() {
        m_initialized = true;
        std::cout << "PhysicsEngine initialized." << std::endl;
        return true;
    }

    void PhysicsEngine::shutdown() {
        m_rigidBodies.clear();
        m_initialized = false;
    }

    void PhysicsEngine::update(float deltaTime) {
        simulate();
    }

    void PhysicsEngine::simulate() {
        for (auto& rb : m_rigidBodies) {
            // Apply gravity
            // Update velocity and position
            // Placeholder
        }
        checkCollisions();
        resolveCollisions();
    }

    void PhysicsEngine::checkCollisions() {
        // Simple AABB collision detection
        for (size_t i = 0; i < m_rigidBodies.size(); ++i) {
            for (size_t j = i + 1; j < m_rigidBodies.size(); ++j) {
                // Check collision between bodies
            }
        }
    }

    void PhysicsEngine::resolveCollisions() {
        // Resolve penetration and apply impulses
    }

    void PhysicsEngine::addRigidBody(std::shared_ptr<GameObject> go) {
        m_rigidBodies.push_back(go);
    }

    void PhysicsEngine::removeRigidBody(std::shared_ptr<GameObject> go) {
        m_rigidBodies.erase(
            std::remove(m_rigidBodies.begin(), m_rigidBodies.end(), go),
            m_rigidBodies.end()
        );
    }

} // namespace CmakeProject1