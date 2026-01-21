#pragma once

#include "Component.h"
#include "../Core/Types.h"

namespace CmakeProject1 {

    enum class ColliderType {
        Box,
        Circle,
        Polygon,
        Edge,
        Capsule,
        Composite
    };

    class Rigidbody2D; // Forward declaration

    class Collider2D : public Component {
    public:
        Collider2D();
        virtual ~Collider2D();

        virtual bool Intersects(const Collider2D* other) const = 0;
        
        // Properties
        void SetIsTrigger(bool trigger);
        bool GetIsTrigger() const;
        void SetDensity(float density);
        float GetDensity() const;
        void SetFriction(float friction);
        float GetFriction() const;
        void SetRestitution(float restitution);
        float GetRestitution() const;

        // Attached rigidbody
        Rigidbody2D* attachedBody = nullptr;

        friend class Rigidbody2D;

    protected:
        bool isTrigger = false;
        float density = 1.0f;
        float friction = 0.5f;
        float restitution = 0.0f;
    };

    class BoxCollider2D : public Collider2D {
    public:
        BoxCollider2D();
        bool Intersects(const Collider2D* other) const override;

        void SetSize(const Vector2& size);
        Vector2 GetSize() const;

    private:
        Vector2 size = {1.0f, 1.0f};
    };

    class CircleCollider2D : public Collider2D {
    public:
        CircleCollider2D();
        bool Intersects(const Collider2D* other) const override;

        void SetRadius(float radius);
        float GetRadius() const;

    private:
        float radius = 1.0f;
    };

    class PolygonCollider2D : public Collider2D {
    public:
        PolygonCollider2D();
        bool Intersects(const Collider2D* other) const override;

        void SetVertices(const std::vector<Vector2>& vertices);
        const std::vector<Vector2>& GetVertices() const;

    private:
        std::vector<Vector2> vertices;
    };

    class EdgeCollider2D : public Collider2D {
    public:
        EdgeCollider2D();
        bool Intersects(const Collider2D* other) const override;

        void SetPoints(const Vector2& start, const Vector2& end);
        std::pair<Vector2, Vector2> GetPoints() const;

    private:
        Vector2 start = {0, 0};
        Vector2 end = {1, 0};
    };

    class CapsuleCollider2D : public Collider2D {
    public:
        CapsuleCollider2D();
        bool Intersects(const Collider2D* other) const override;

        void SetRadius(float radius);
        float GetRadius() const;
        void SetHeight(float height);
        float GetHeight() const;

    private:
        float radius = 0.5f;
        float height = 1.0f;
    };

    class CompositeCollider2D : public Collider2D {
    public:
        CompositeCollider2D();
        ~CompositeCollider2D();
        bool Intersects(const Collider2D* other) const override;

        void AddCollider(Collider2D* collider);
        void RemoveCollider(Collider2D* collider);
        const std::vector<Collider2D*>& GetColliders() const { return childColliders; }

    private:
        std::vector<Collider2D*> childColliders;
    };

} // namespace CmakeProject1