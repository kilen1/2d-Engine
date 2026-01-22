#pragma once

#include "Component.h"
#include "Types.h"
#include <memory>

namespace CmakeProject1 {

    class Texture;
    class Sprite;
    class Material;

    class SpriteRenderer : public Component {
    public:
        SpriteRenderer();

        ComponentType getType() const override { return ComponentType::SpriteRenderer; }
        std::string getTypeName() const override { return "SpriteRenderer"; }

        // Sprite
        std::shared_ptr<Sprite> getSprite() const { return m_sprite; }
        void setSprite(std::shared_ptr<Sprite> sprite);
        void setSprite(std::shared_ptr<Texture> texture);

        // Color
        Color getColor() const { return m_color; }
        void setColor(const Color& color) { m_color = color; }

        // Flip
        bool getFlipX() const { return m_flipX; }
        void setFlipX(bool flip) { m_flipX = flip; }

        bool getFlipY() const { return m_flipY; }
        void setFlipY(bool flip) { m_flipY = flip; }

        // Material
        std::shared_ptr<Material> getMaterial() const { return m_material; }
        void setMaterial(std::shared_ptr<Material> material);

        // Sorting
        int getSortingLayer() const { return m_sortingLayer; }
        void setSortingLayer(int layer) { m_sortingLayer = layer; }

        int getOrderInLayer() const { return m_orderInLayer; }
        void setOrderInLayer(int order) { m_orderInLayer = order; }

        // Size
        Vector2 getSize() const;
        void setSize(const Vector2& size);

        // Pivot
        Vector2 getPivot() const { return m_pivot; }
        void setPivot(const Vector2& pivot) { m_pivot = pivot; }

        // Tiling
        Vector2 getTiling() const { return m_tiling; }
        void setTiling(const Vector2& tiling) { m_tiling = tiling; }

        // Offset
        Vector2 getOffset() const { return m_offset; }
        void setOffset(const Vector2& offset) { m_offset = offset; }

        // Render
        void render() override;

        // Serialization
        void serialize(class Serializer& serializer) const override;
        void deserialize(class Deserializer& deserializer) override;

        // Editor
        void onInspectorGUI() override;

    private:
        std::shared_ptr<Sprite> m_sprite;
        std::shared_ptr<Material> m_material;

        Color m_color = Color::White();
        Vector2 m_pivot = Vector2(0.5f, 0.5f);
        Vector2 m_tiling = Vector2(1, 1);
        Vector2 m_offset = Vector2(0, 0);

        bool m_flipX = false;
        bool m_flipY = false;

        int m_sortingLayer = 0;
        int m_orderInLayer = 0;

        // For custom mesh (if needed)
        std::vector<Vector2> m_vertices;
        std::vector<uint32_t> m_indices;
        std::vector<Vector2> m_uvs;

        void updateMesh();

        friend class Renderer;
    };

} // namespace CmakeProject1