#include "Rigidbody2D.h"
#include "Collider2D.h"
#include <iostream>

namespace CmakeProject1 {

    // Rigidbody2D implementations
    Rigidbody2D::Rigidbody2D() : velocity({0, 0}), angularVelocity(0.0f), mass(1.0f), bodyType(2), fixedRotation(false) {
        // Initialize rigidbody properties
    }

    Rigidbody2D::~Rigidbody2D() {
        // Clean up resources
    }

    void Rigidbody2D::SetVelocity(const Vector2& vel) {
        velocity = vel;
    }

    Vector2 Rigidbody2D::GetVelocity() const {
        return velocity;
    }

    void Rigidbody2D::SetAngularVelocity(float omega) {
        angularVelocity = omega;
    }

    float Rigidbody2D::GetAngularVelocity() const {
        return angularVelocity;
    }

    void Rigidbody2D::SetMass(float m) {
        mass = m;
    }

    float Rigidbody2D::GetMass() const {
        return mass;
    }

    void Rigidbody2D::ApplyForce(const Vector2& force, const Vector2& point) {
        // Apply force implementation
    }

    void Rigidbody2D::ApplyImpulse(const Vector2& impulse, const Vector2& point) {
        // Apply impulse implementation
    }

    void Rigidbody2D::ApplyTorque(float torque) {
        // Apply torque implementation
    }

    void Rigidbody2D::SetBodyType(int type) {
        bodyType = type;
    }

    int Rigidbody2D::GetBodyType() const {
        return bodyType;
    }

    void Rigidbody2D::SetFixedRotation(bool fixed) {
        fixedRotation = fixed;
    }

    bool Rigidbody2D::IsFixedRotation() const {
        return fixedRotation;
    }

    void Rigidbody2D::SetPosition(const Vector2& position) {
        // Set position implementation
    }

    Vector2 Rigidbody2D::GetPosition() const {
        return {0, 0}; // Placeholder
    }

    void Rigidbody2D::SetRotation(float angle) {
        // Set rotation implementation
    }

    float Rigidbody2D::GetRotation() const {
        return 0.0f; // Placeholder
    }

    void Rigidbody2D::CreateJoint(JointType type, Rigidbody2D* otherBody, const JointConfig& config) {
        // Create joint implementation
    }

    // BoxCollider2D implementations
    BoxCollider2D::BoxCollider2D() : size({1.0f, 1.0f}) {
        // Initialize box collider
    }

    bool BoxCollider2D::Intersects(const Collider2D* other) const {
        return false; // Placeholder
    }

    void BoxCollider2D::SetSize(const Vector2& s) {
        size = s;
    }

    Vector2 BoxCollider2D::GetSize() const {
        return size;
    }

    // CircleCollider2D implementations
    CircleCollider2D::CircleCollider2D() : radius(1.0f) {
        // Initialize circle collider
    }

    bool CircleCollider2D::Intersects(const Collider2D* other) const {
        return false; // Placeholder
    }

    void CircleCollider2D::SetRadius(float r) {
        radius = r;
    }

    float CircleCollider2D::GetRadius() const {
        return radius;
    }

    // PolygonCollider2D implementations
    PolygonCollider2D::PolygonCollider2D() {
        // Initialize polygon collider
    }

    bool PolygonCollider2D::Intersects(const Collider2D* other) const {
        return false; // Placeholder
    }

    void PolygonCollider2D::SetVertices(const std::vector<Vector2>& verts) {
        vertices = verts;
    }

    const std::vector<Vector2>& PolygonCollider2D::GetVertices() const {
        return vertices;
    }

    // EdgeCollider2D implementations
    EdgeCollider2D::EdgeCollider2D() {
        // Initialize edge collider
    }

    bool EdgeCollider2D::Intersects(const Collider2D* other) const {
        return false; // Placeholder
    }

    void EdgeCollider2D::SetPoints(const Vector2& startPt, const Vector2& endPt) {
        start = startPt;
        end = endPt;
    }

    std::pair<Vector2, Vector2> EdgeCollider2D::GetPoints() const {
        return {start, end};
    }

    // CapsuleCollider2D implementations
    CapsuleCollider2D::CapsuleCollider2D() : radius(0.5f), height(1.0f) {
        // Initialize capsule collider
    }

    bool CapsuleCollider2D::Intersects(const Collider2D* other) const {
        return false; // Placeholder
    }

    void CapsuleCollider2D::SetRadius(float r) {
        radius = r;
    }

    float CapsuleCollider2D::GetRadius() const {
        return radius;
    }

    void CapsuleCollider2D::SetHeight(float h) {
        height = h;
    }

    float CapsuleCollider2D::GetHeight() const {
        return height;
    }

    // CompositeCollider2D implementations
    CompositeCollider2D::CompositeCollider2D() {
        // Initialize composite collider
    }

    CompositeCollider2D::~CompositeCollider2D() {
        // Clean up child colliders
        for (auto* collider : childColliders) {
            delete collider;
        }
        childColliders.clear();
    }

    bool CompositeCollider2D::Intersects(const Collider2D* other) const {
        return false; // Placeholder
    }

    void CompositeCollider2D::AddCollider(Collider2D* collider) {
        childColliders.push_back(collider);
    }

    void CompositeCollider2D::RemoveCollider(Collider2D* collider) {
        childColliders.erase(
            std::remove(childColliders.begin(), childColliders.end(), collider),
            childColliders.end()
        );
    }

    // Collider2D implementations
    Collider2D::Collider2D() : isTrigger(false), density(1.0f), friction(0.5f), restitution(0.0f), attachedBody(nullptr) {
        // Initialize collider properties
    }

    Collider2D::~Collider2D() {
        // Clean up resources
    }

    void Collider2D::SetIsTrigger(bool trigger) {
        isTrigger = trigger;
    }

    bool Collider2D::GetIsTrigger() const {
        return isTrigger;
    }

    void Collider2D::SetDensity(float d) {
        density = d;
    }

    float Collider2D::GetDensity() const {
        return density;
    }

    void Collider2D::SetFriction(float f) {
        friction = f;
    }

    float Collider2D::GetFriction() const {
        return friction;
    }

    void Collider2D::SetRestitution(float r) {
        restitution = r;
    }

    float Collider2D::GetRestitution() const {
        return restitution;
    }

}