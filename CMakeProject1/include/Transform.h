#pragma once

#include "Component.h"
#include "Types.h"

namespace CmakeProject1 {

    class Transform : public Component {
    public:
        Transform();

        ComponentType getType() const override { return ComponentType::Transform; }
        std::string getTypeName() const override { return "Transform"; }

        // Position
        Vector2 getPosition() const { return m_position; }
        void setPosition(const Vector2& position);
        void setPosition(float x, float y);
        void translate(const Vector2& translation);

        // Rotation (in degrees)
        float getRotation() const { return m_rotation; }
        void setRotation(float degrees);
        void rotate(float degrees);

        // Scale
        Vector2 getScale() const { return m_scale; }
        void setScale(const Vector2& scale);
        void setScale(float x, float y);
        void setUniformScale(float scale);

        // Local vs world space
        Vector2 getWorldPosition() const;
        void setWorldPosition(const Vector2& position);

        float getWorldRotation() const;
        void setWorldRotation(float degrees);

        Vector2 getWorldScale() const;

        // Transform matrix
        Matrix4 getLocalMatrix() const;
        Matrix4 getWorldMatrix() const;

        // Direction vectors
        Vector2 getRight() const;
        Vector2 getUp() const;
        Vector2 getForward() const;

        // Hierarchy
        Vector2 transformPoint(const Vector2& point) const;
        Vector2 inverseTransformPoint(const Vector2& point) const;

        Vector2 transformDirection(const Vector2& direction) const;
        Vector2 inverseTransformDirection(const Vector2& direction) const;

        // Look at
        void lookAt(const Vector2& target);
        void lookAt(const Transform& target);

        // Children
        std::shared_ptr<Transform> getChild(int index) const;
        int getChildCount() const;

        // Parent
        std::shared_ptr<Transform> getParent() const;
        void setParent(std::shared_ptr<Transform> parent);
        void setParent(std::shared_ptr<Transform> parent, bool worldPositionStays);

        // Serialization
        void serialize(class Serializer& serializer) const override;
        void deserialize(class Deserializer& deserializer) override;

        // Editor
        void onInspectorGUI() override;

    private:
        Vector2 m_position = Vector2(0, 0);
        float m_rotation = 0.0f;
        Vector2 m_scale = Vector2(1, 1);

        std::weak_ptr<Transform> m_parent;
        std::vector<std::weak_ptr<Transform>> m_children;

        mutable Matrix4 m_localMatrix;
        mutable Matrix4 m_worldMatrix;
        mutable bool m_dirty = true;

        void setDirty();
        void updateMatrices() const;

        friend class GameObject;
    };

} // namespace CmakeProject1