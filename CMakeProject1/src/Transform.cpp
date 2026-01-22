#include "../include/Transform.h"
#include "../include/GameObject.h"

namespace CmakeProject1 {

    Transform::Transform() {
        // Initialize transform with default values
        setDirty();
    }

    void Transform::setPosition(const Vector2& position) {
        m_position = position;
        setDirty();
    }

    void Transform::setPosition(float x, float y) {
        m_position = Vector2(x, y);
        setDirty();
    }

    void Transform::translate(const Vector2& translation) {
        m_position = m_position + translation;
        setDirty();
    }

    void Transform::setRotation(float degrees) {
        m_rotation = degrees;
        setDirty();
    }

    void Transform::rotate(float degrees) {
        m_rotation += degrees;
        setDirty();
    }

    void Transform::setScale(const Vector2& scale) {
        m_scale = scale;
        setDirty();
    }

    void Transform::setScale(float x, float y) {
        m_scale = Vector2(x, y);
        setDirty();
    }

    void Transform::setUniformScale(float scale) {
        m_scale = Vector2(scale, scale);
        setDirty();
    }

    Vector2 Transform::getWorldPosition() const {
        // If this transform has a parent, combine positions
        if (auto parent = getParent()) {
            Matrix4 worldMatrix = getWorldMatrix();
            return Vector2(worldMatrix.m[12], worldMatrix.m[13]);
        }
        return m_position;
    }

    void Transform::setWorldPosition(const Vector2& position) {
        // If this transform has a parent, convert to local space
        if (auto parent = getParent()) {
            Matrix4 invParentMatrix = parent->getWorldMatrix();
            // Invert the parent matrix to convert world to local space
            // This is a simplified approach - proper matrix inversion would be needed for full functionality
            Vector2 localPos = position - parent->getWorldPosition();
            setPosition(localPos);
        } else {
            setPosition(position);
        }
    }

    float Transform::getWorldRotation() const {
        if (auto parent = getParent()) {
            return parent->getWorldRotation() + m_rotation;
        }
        return m_rotation;
    }

    void Transform::setWorldRotation(float degrees) {
        if (auto parent = getParent()) {
            setRotation(degrees - parent->getWorldRotation());
        } else {
            setRotation(degrees);
        }
    }

    Vector2 Transform::getWorldScale() const {
        if (auto parent = getParent()) {
            Vector2 parentScale = parent->getWorldScale();
            return Vector2(m_scale.x * parentScale.x, m_scale.y * parentScale.y);
        }
        return m_scale;
    }

    Matrix4 Transform::getLocalMatrix() const {
        if (m_dirty) {
            updateMatrices();
        }
        return m_localMatrix;
    }

    Matrix4 Transform::getWorldMatrix() const {
        if (m_dirty) {
            updateMatrices();
        }
        return m_worldMatrix;
    }

    Vector2 Transform::getRight() const {
        float rad = m_rotation * M_PI / 180.0f;
        return Vector2(cos(rad), sin(rad));
    }

    Vector2 Transform::getUp() const {
        float rad = (m_rotation + 90) * M_PI / 180.0f;
        return Vector2(cos(rad), sin(rad));
    }

    Vector2 Transform::getForward() const {
        // In 2D, forward is typically considered along Z axis, but returning Y for convention
        float rad = m_rotation * M_PI / 180.0f;
        return Vector2(-sin(rad), cos(rad));
    }

    Vector2 Transform::transformPoint(const Vector2& point) const {
        Matrix4 worldMatrix = getWorldMatrix();
        // Apply transformation to the point
        float x = worldMatrix.m[0] * point.x + worldMatrix.m[4] * point.y + worldMatrix.m[12];
        float y = worldMatrix.m[1] * point.x + worldMatrix.m[5] * point.y + worldMatrix.m[13];
        return Vector2(x, y);
    }

    Vector2 Transform::inverseTransformPoint(const Vector2& point) const {
        Matrix4 invMatrix = getWorldMatrix();
        // This would require matrix inversion which is complex
        // Simplified implementation for now
        return Vector2(point.x - m_position.x, point.y - m_position.y);
    }

    Vector2 Transform::transformDirection(const Vector2& direction) const {
        Matrix4 worldMatrix = getWorldMatrix();
        // Apply rotation and scale to direction vector (no translation)
        float x = worldMatrix.m[0] * direction.x + worldMatrix.m[4] * direction.y;
        float y = worldMatrix.m[1] * direction.x + worldMatrix.m[5] * direction.y;
        return Vector2(x, y);
    }

    Vector2 Transform::inverseTransformDirection(const Vector2& direction) const {
        // Simplified implementation for now
        return direction / getWorldScale();
    }

    void Transform::lookAt(const Vector2& target) {
        Vector2 direction = target - getWorldPosition();
        float angle = atan2(direction.y, direction.x) * 180.0f / M_PI;
        setRotation(angle);
    }

    void Transform::lookAt(const Transform& target) {
        lookAt(target.getWorldPosition());
    }

    std::shared_ptr<Transform> Transform::getChild(int index) const {
        if (index >= 0 && index < static_cast<int>(m_children.size())) {
            return m_children[index].lock();
        }
        return nullptr;
    }

    int Transform::getChildCount() const {
        return static_cast<int>(m_children.size());
    }

    std::shared_ptr<Transform> Transform::getParent() const {
        return m_parent.lock();
    }

    void Transform::setParent(std::shared_ptr<Transform> parent) {
        setParent(parent, true); // Default to keeping world position
    }

    void Transform::setParent(std::shared_ptr<Transform> parent, bool worldPositionStays) {
        if (parent == shared_from_this()) return; // Cannot be own parent
        
        // Store world position if needed
        Vector2 oldWorldPos = getWorldPosition();
        float oldWorldRot = getWorldRotation();
        Vector2 oldWorldScale = getWorldScale();
        
        // Remove from current parent
        if (auto currentParent = getParent()) {
            auto it = std::find_if(currentParent->m_children.begin(), currentParent->m_children.end(),
                [this](const std::weak_ptr<Transform>& wp) {
                    return wp.lock() == shared_from_this();
                });
            if (it != currentParent->m_children.end()) {
                currentParent->m_children.erase(it);
            }
        }
        
        // Set new parent
        m_parent = parent;
        
        // Add to new parent's children
        if (parent) {
            parent->m_children.push_back(shared_from_this());
        }
        
        // Restore world position if requested
        if (worldPositionStays) {
            setWorldPosition(oldWorldPos);
            setWorldRotation(oldWorldRot - (parent ? parent->getWorldRotation() : 0));
            // For scale, we need to divide by parent's scale
            Vector2 parentScale = parent ? parent->getWorldScale() : Vector2(1, 1);
            setScale(oldWorldScale.x / parentScale.x, oldWorldScale.y / parentScale.y);
        }
        
        setDirty();
    }

    void Transform::serialize(Serializer& serializer) const {
        Component::serialize(serializer);
        serializer.write("position", m_position);
        serializer.write("rotation", m_rotation);
        serializer.write("scale", m_scale);
    }

    void Transform::deserialize(Deserializer& deserializer) {
        Component::deserialize(deserializer);
        deserializer.read("position", m_position);
        deserializer.read("rotation", m_rotation);
        deserializer.read("scale", m_scale);
        setDirty();
    }

    void Transform::onInspectorGUI() {
        // Draw transform properties in the inspector
    }

    void Transform::setDirty() {
        m_dirty = true;
        // Mark all children as dirty too since they depend on this transform
        for (const auto& childWeak : m_children) {
            if (auto child = childWeak.lock()) {
                child->setDirty();
            }
        }
    }

    void Transform::updateMatrices() const {
        // Create local transformation matrix
        // Apply scale
        Matrix4 scaleMat;
        scaleMat.m[0] = m_scale.x;
        scaleMat.m[5] = m_scale.y;
        
        // Apply rotation
        float rad = m_rotation * M_PI / 180.0f;
        Matrix4 rotMat;
        rotMat.m[0] = cos(rad);
        rotMat.m[1] = sin(rad);
        rotMat.m[4] = -sin(rad);
        rotMat.m[5] = cos(rad);
        
        // Apply translation
        Matrix4 transMat;
        transMat.m[12] = m_position.x;
        transMat.m[13] = m_position.y;
        
        // Combine transformations: T * R * S
        m_localMatrix = transMat;
        // Apply rotation
        for (int i = 0; i < 4; ++i) {
            float temp[4];
            for (int j = 0; j < 4; ++j) {
                temp[j] = 0;
                for (int k = 0; k < 4; ++k) {
                    if (j == 0) temp[j] += m_localMatrix.m[i * 4 + k] * rotMat.m[k * 4 + j];
                    else if (j == 1) temp[j] += m_localMatrix.m[i * 4 + k] * rotMat.m[k * 4 + j];
                    else if (j == 2) temp[j] += m_localMatrix.m[i * 4 + k] * rotMat.m[k * 4 + j];
                    else temp[j] += m_localMatrix.m[i * 4 + k] * rotMat.m[k * 4 + j];
                }
            }
            for (int j = 0; j < 4; ++j) {
                m_localMatrix.m[i * 4 + j] = temp[j];
            }
        }
        // Apply scale
        for (int i = 0; i < 4; ++i) {
            float temp[4];
            for (int j = 0; j < 4; ++j) {
                temp[j] = 0;
                for (int k = 0; k < 4; ++k) {
                    if (j == 0) temp[j] += m_localMatrix.m[i * 4 + k] * scaleMat.m[k * 4 + j];
                    else if (j == 1) temp[j] += m_localMatrix.m[i * 4 + k] * scaleMat.m[k * 4 + j];
                    else if (j == 2) temp[j] += m_localMatrix.m[i * 4 + k] * scaleMat.m[k * 4 + j];
                    else temp[j] += m_localMatrix.m[i * 4 + k] * scaleMat.m[k * 4 + j];
                }
            }
            for (int j = 0; j < 4; ++j) {
                m_localMatrix.m[i * 4 + j] = temp[j];
            }
        }
        
        // Calculate world matrix
        if (auto parent = getParent()) {
            Matrix4 parentWorld = parent->getWorldMatrix();
            // Multiply parent world matrix by local matrix
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 4; ++j) {
                    m_worldMatrix.m[i * 4 + j] = 0;
                    for (int k = 0; k < 4; ++k) {
                        m_worldMatrix.m[i * 4 + j] += parentWorld.m[i * 4 + k] * m_localMatrix.m[k * 4 + j];
                    }
                }
            }
        } else {
            m_worldMatrix = m_localMatrix;
        }
        
        m_dirty = false;
    }

} // namespace CmakeProject1