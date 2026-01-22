#include "../include/Camera.h"
#include "../include/GameObject.h"
#include "../include/Transform.h"
#include <cmath>

namespace CmakeProject1 {

    Camera::Camera() {
        // Initialize camera with default values
    }

    Matrix4 Camera::getViewMatrix() const {
        if (m_dirty) {
            updateMatrices();
        }
        return m_viewMatrix;
    }

    Matrix4 Camera::getProjectionMatrix() const {
        if (m_dirty) {
            updateMatrices();
        }
        return m_projectionMatrix;
    }

    Matrix4 Camera::getViewProjectionMatrix() const {
        if (m_dirty) {
            updateMatrices();
        }
        return m_viewProjectionMatrix;
    }

    Vector2 Camera::screenToWorldPoint(const Vector2& screenPoint) const {
        // Convert screen point to world point
        // Implementation depends on camera matrices and viewport
        return Vector2();
    }

    Vector2 Camera::worldToScreenPoint(const Vector2& worldPoint) const {
        // Convert world point to screen point
        // Implementation depends on camera matrices and viewport
        return Vector2();
    }

    Rect Camera::getWorldRect() const {
        // Calculate the world rectangle based on the camera's view
        if (m_projection == Projection::Orthographic) {
            float halfWidth = m_orthographicSize * (m_viewport.width / m_viewport.height);
            float halfHeight = m_orthographicSize;
            
            auto transform = getTransform();
            Vector2 position = transform ? transform->getPosition() : Vector2();
            
            return Rect(position.x - halfWidth, position.y - halfHeight, 
                       halfWidth * 2, halfHeight * 2);
        }
        
        // For perspective cameras, we might want to calculate frustum bounds
        return Rect();
    }

    bool Camera::isVisible(const Vector2& point) const {
        Rect worldRect = getWorldRect();
        return worldRect.contains(point);
    }

    bool Camera::isVisible(const Rect& rect) const {
        Rect worldRect = getWorldRect();
        // Simple overlap test - could be improved with more sophisticated frustum culling
        return !(rect.x > worldRect.x + worldRect.width || 
                 rect.x + rect.width < worldRect.x ||
                 rect.y > worldRect.y + worldRect.height || 
                 rect.y + rect.height < worldRect.y);
    }

    void Camera::render() {
        // Render camera-specific logic
    }

    void Camera::serialize(Serializer& serializer) const {
        Component::serialize(serializer);
        // Serialize camera-specific properties
        serializer.write("projection", static_cast<int>(m_projection));
        serializer.write("orthographicSize", m_orthographicSize);
        serializer.write("fieldOfView", m_fieldOfView);
        serializer.write("nearClip", m_nearClip);
        serializer.write("farClip", m_farClip);
        serializer.write("viewport", m_viewport);
        serializer.write("clearFlags", static_cast<int>(m_clearFlags));
        serializer.write("backgroundColor", m_backgroundColor);
        serializer.write("cullingMask", m_cullingMask);
        serializer.write("depth", m_depth);
    }

    void Camera::deserialize(Deserializer& deserializer) {
        Component::deserialize(deserializer);
        // Deserialize camera-specific properties
        int projectionInt;
        deserializer.read("projection", projectionInt);
        m_projection = static_cast<Projection>(projectionInt);
        
        deserializer.read("orthographicSize", m_orthographicSize);
        deserializer.read("fieldOfView", m_fieldOfView);
        deserializer.read("nearClip", m_nearClip);
        deserializer.read("farClip", m_farClip);
        deserializer.read("viewport", m_viewport);
        
        int clearFlagsInt;
        deserializer.read("clearFlags", clearFlagsInt);
        m_clearFlags = static_cast<ClearFlags>(clearFlagsInt);
        
        deserializer.read("backgroundColor", m_backgroundColor);
        deserializer.read("cullingMask", m_cullingMask);
        deserializer.read("depth", m_depth);
        
        m_dirty = true; // Mark matrices as needing update
    }

    void Camera::onInspectorGUI() {
        // Draw camera-specific properties in the inspector
    }

    void Camera::updateProjection() {
        m_dirty = true; // Mark matrices as needing update
    }

    void Camera::updateMatrices() const {
        // Get transform information
        auto transform = getTransform();
        if (!transform) {
            m_dirty = false;
            return;
        }

        Vector2 position = transform->getPosition();

        // Create view matrix (inverse of transform)
        // For now, just create a basic translation matrix
        m_viewMatrix = Matrix4::translate(Vector2(-position.x, -position.y));

        // Create projection matrix based on projection type
        if (m_projection == Projection::Orthographic) {
            float halfWidth = m_orthographicSize * (m_viewport.width / m_viewport.height);
            float halfHeight = m_orthographicSize;
            m_projectionMatrix = Matrix4::orthographic(
                -halfWidth, halfWidth, 
                -halfHeight, halfHeight, 
                m_nearClip, m_farClip
            );
        } else {
            // Perspective projection would go here
            m_projectionMatrix.identity();
        }

        // Combine view and projection matrices
        // For now, just copy the projection matrix
        m_viewProjectionMatrix = m_projectionMatrix;

        m_dirty = false;
    }

} // namespace CmakeProject1