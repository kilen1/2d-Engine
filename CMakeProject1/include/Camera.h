#pragma once

#include "Component.h"
#include "Types.h"

namespace CmakeProject1 {

    class Camera : public Component {
    public:
        enum class Projection {
            Orthographic,
            Perspective
        };

        Camera();

        ComponentType getType() const override { return ComponentType::Camera; }
        std::string getTypeName() const override { return "Camera"; }

        // Projection
        Projection getProjection() const { return m_projection; }
        void setProjection(Projection projection) { m_projection = projection; updateProjection(); }

        // Orthographic size
        float getOrthographicSize() const { return m_orthographicSize; }
        void setOrthographicSize(float size) { m_orthographicSize = size; updateProjection(); }

        // Field of view (for perspective)
        float getFieldOfView() const { return m_fieldOfView; }
        void setFieldOfView(float fov) { m_fieldOfView = fov; updateProjection(); }

        // Viewport
        Rect getViewport() const { return m_viewport; }
        void setViewport(const Rect& viewport) { m_viewport = viewport; updateProjection(); }

        // Clear flags
        enum class ClearFlags {
            Skybox,
            SolidColor,
            DepthOnly,
            DontClear
        };

        ClearFlags getClearFlags() const { return m_clearFlags; }
        void setClearFlags(ClearFlags flags) { m_clearFlags = flags; }

        // Background color
        Color getBackgroundColor() const { return m_backgroundColor; }
        void setBackgroundColor(const Color& color) { m_backgroundColor = color; }

        // Culling mask
        int getCullingMask() const { return m_cullingMask; }
        void setCullingMask(int mask) { m_cullingMask = mask; }

        // Depth
        int getDepth() const { return m_depth; }
        void setDepth(int depth) { m_depth = depth; }

        // Matrices
        Matrix4 getViewMatrix() const;
        Matrix4 getProjectionMatrix() const;
        Matrix4 getViewProjectionMatrix() const;

        // Viewport to world
        Vector2 screenToWorldPoint(const Vector2& screenPoint) const;
        Vector2 worldToScreenPoint(const Vector2& worldPoint) const;

        Rect getWorldRect() const;

        // Frustum
        bool isVisible(const Vector2& point) const;
        bool isVisible(const Rect& rect) const;

        // Render
        void render() override;

        // Serialization
        void serialize(class Serializer& serializer) const override;
        void deserialize(class Deserializer& deserializer) override;

        // Editor
        void onInspectorGUI() override;

    private:
        Projection m_projection = Projection::Orthographic;
        float m_orthographicSize = 5.0f;
        float m_fieldOfView = 60.0f;
        float m_nearClip = 0.1f;
        float m_farClip = 1000.0f;

        Rect m_viewport = Rect(0, 0, 1, 1); // Normalized

        ClearFlags m_clearFlags = ClearFlags::SolidColor;
        Color m_backgroundColor = Color(0.1f, 0.1f, 0.1f, 1.0f);

        int m_cullingMask = 0xFFFFFFFF; // All layers
        int m_depth = 0;

        mutable Matrix4 m_viewMatrix;
        mutable Matrix4 m_projectionMatrix;
        mutable Matrix4 m_viewProjectionMatrix;
        mutable bool m_dirty = true;

        void updateProjection();
        void updateMatrices() const;

        friend class Renderer;
    };

} // namespace CmakeProject1