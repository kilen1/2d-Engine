#pragma once

#include "Component.h"
#include "Collider2D.h"
#include "../Core/Types.h"

namespace CmakeProject1 {

    enum class JointType {
        Revolute,
        Prismatic,
        Distance,
        Pulley,
        Mouse,
        Gear,
        Wheel,
        Weld,
        Friction,
        Rope
    };

    struct JointConfig {
        Vector2 anchorA;
        Vector2 anchorB;
        bool collideConnected = false;
        float frequencyHz = 4.0f;
        float dampingRatio = 0.7f;
    };

    struct ContactPoint {
        Vector2 position;
        Vector2 normal;
        float penetration = 0.0f;
    };

    class Rigidbody2D : public Component {
    public:
        Rigidbody2D();
        ~Rigidbody2D();

        // Movement methods
        void SetVelocity(const Vector2& velocity);
        Vector2 GetVelocity() const;
        void SetAngularVelocity(float omega);
        float GetAngularVelocity() const;
        void SetMass(float mass);
        float GetMass() const;
        void ApplyForce(const Vector2& force, const Vector2& point);
        void ApplyImpulse(const Vector2& impulse, const Vector2& point);
        void ApplyTorque(float torque);

        // Body properties
        void SetBodyType(int type); // Dynamic, Static, Kinematic
        int GetBodyType() const;
        void SetFixedRotation(bool fixed);
        bool IsFixedRotation() const;

        // Position and rotation
        void SetPosition(const Vector2& position);
        Vector2 GetPosition() const;
        void SetRotation(float angle);
        float GetRotation() const;

        // Collision callbacks
        std::function<void(Rigidbody2D* other, const ContactPoint& contact)> onCollisionEnter;
        std::function<void(Rigidbody2D* other, const ContactPoint& contact)> onCollisionStay;
        std::function<void(Rigidbody2D* other)> onCollisionExit;
        std::function<void(Rigidbody2D* other)> onTriggerEnter;
        std::function<void(Rigidbody2D* other)> onTriggerStay;
        std::function<void(Rigidbody2D* other)> onTriggerExit;

        // Joints
        void CreateJoint(JointType type, Rigidbody2D* otherBody, const JointConfig& config);

    private:
        Vector2 velocity;
        float angularVelocity = 0.0f;
        float mass = 1.0f;
        int bodyType = 0; // 0: Static, 1: Kinematic, 2: Dynamic
        bool fixedRotation = false;
    };

} // namespace CmakeProject1