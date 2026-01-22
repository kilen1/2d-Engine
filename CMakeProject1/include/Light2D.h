#pragma once

#include "Component.h"
#include "Types.h"
#include <memory>

namespace CmakeProject1 {

    class Light2D : public Component {
    public:
        enum class Type {
            Point,
            Directional,
            Spot,
            Area
        };

        Light2D();

        ComponentType getType() const override { return ComponentType::Light2D; }
        std::string getTypeName() const override { return "Light2D"; }

        // Type
        Type getLightType() const { return m_type; }
        void setLightType(Type type) { m_type = type; }

        // Color
        Color getColor() const { return m_color; }
        void setColor(const Color& color) { m_color = color; }

        // Intensity
        float getIntensity() const { return m_intensity; }
        void setIntensity(float intensity) { m_intensity = intensity; }

        // Range
        float getRange() const { return m_range; }
        void setRange(float range) { m_range = range; }

        // Spot angle (for spot lights)
        float getSpotAngle() const { return m_spotAngle; }
        void setSpotAngle(float angle) { m_spotAngle = angle; }

        // Direction (for directional and spot lights)
        Vector2 getDirection() const;
        void setDirection(const Vector2& direction);

        // Shadows
        bool getCastShadows() const { return m_castShadows; }
        void setCastShadows(bool cast) { m_castShadows = cast; }

        float getShadowStrength() const { return m_shadowStrength; }
        void setShadowStrength(float strength) { m_shadowStrength = strength; }

        // Cookies (texture masks)
        std::shared_ptr<class Texture> getCookie() const { return m_cookie; }
        void setCookie(std::shared_ptr<class Texture> texture) { m_cookie = texture; }

        // Blending
        bool getBlendAdditive() const { return m_blendAdditive; }
        void setBlendAdditive(bool additive) { m_blendAdditive = additive; }

        // Render
        void render() override;

        // Serialization
        void serialize(class Serializer& serializer) const override;
        void deserialize(class Deserializer& deserializer) override;

        // Editor
        void onInspectorGUI() override;

    private:
        Type m_type = Type::Point;
        Color m_color = Color::White();
        float m_intensity = 1.0f;
        float m_range = 10.0f;
        float m_spotAngle = 45.0f;

        bool m_castShadows = false;
        float m_shadowStrength = 0.5f;

        std::shared_ptr<class Texture> m_cookie;

        bool m_blendAdditive = false;

        // Internal rendering data
        unsigned int m_shadowMap = 0;

        void updateShadowMap();
        void renderShadowMap();

        friend class LightSystem;
    };

} // namespace CmakeProject1