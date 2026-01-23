#include "Light2D.h"
#include "GameObject.h"
#include "Transform.h"
#include <Serializer.h>
#include <Deserializer.h>
#include <Types.h>
#include <cmath>

namespace CmakeProject1 {
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
    Light2D::Light2D() {
        // Initialize light with default values
    }

    Vector2 Light2D::getDirection() const {
        // Get the direction from the transform's rotation
        auto transform = getTransform();
        if (transform) {
            // Use transform's rotation to determine direction
            float rad = transform->getRotation() * M_PI / 180.0f;
            return Vector2(cos(rad), sin(rad));
        }
        // Default direction is along positive X axis
        return Vector2(1.0f, 0.0f);
    }

    void Light2D::setDirection(const Vector2& direction) {
        // Calculate the rotation needed to face the given direction
        auto transform = getTransform();
        if (transform) {
            float angle = atan2(direction.y, direction.x) * 180.0f / M_PI;
            transform->setRotation(angle);
        }
    }

    void Light2D::render() {
        // Render the light effect
        // This would involve setting up lighting calculations and rendering
    }

    void Light2D::serialize(Serializer& serializer) const {
        Component::serialize(serializer);
        serializer.write("lightType", static_cast<int>(m_type));
        serializer.write("color", m_color);
        serializer.write("intensity", m_intensity);
        serializer.write("range", m_range);
        serializer.write("spotAngle", m_spotAngle);
        serializer.write("castShadows", m_castShadows);
        serializer.write("shadowStrength", m_shadowStrength);
        serializer.write("blendAdditive", m_blendAdditive);
    }

    void Light2D::deserialize(Deserializer& deserializer) {
        Component::deserialize(deserializer);
        int typeInt;
        deserializer.read("lightType", typeInt);
        m_type = static_cast<Type>(typeInt);
        deserializer.read("color", m_color);
        deserializer.read("intensity", m_intensity);
        deserializer.read("range", m_range);
        deserializer.read("spotAngle", m_spotAngle);
        deserializer.read("castShadows", m_castShadows);
        deserializer.read("shadowStrength", m_shadowStrength);
        deserializer.read("blendAdditive", m_blendAdditive);
    }

    void Light2D::onInspectorGUI() {
        // Draw light properties in the inspector
    }

    void Light2D::updateShadowMap() {
        // Update the shadow map texture if shadows are enabled
        if (m_castShadows) {
            // Create or update shadow map texture
        }
    }

    void Light2D::renderShadowMap() {
        // Render the shadow map for this light
        // This would involve rendering shadow casting objects from the light's perspective
    }

} // namespace CmakeProject1