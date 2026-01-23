#include "SpriteRenderer.h"
#include <GameObject.h>
#include <Serializer.h>
#include <Deserializer.h>
#include "Transform.h"

namespace CmakeProject1 {

    SpriteRenderer::SpriteRenderer() {
        // Initialize with default values
        updateMesh();
    }

    void SpriteRenderer::setSprite(std::shared_ptr<Sprite> sprite) {
        m_sprite = sprite;
        updateMesh();
    }

    void SpriteRenderer::setSprite(std::shared_ptr<Texture> texture) {
        // Create a sprite from the texture
        // For now, just store the texture reference
        if (texture) {
            // In a real implementation, we would create a sprite from the texture
            // For now, we'll just consider it as having a sprite
        }
        updateMesh();
    }

    void SpriteRenderer::setMaterial(std::shared_ptr<Material> material) {
        m_material = material;
    }

    Vector2 SpriteRenderer::getSize() const {
        if (m_sprite) {
            // Return the size of the sprite
            // For now, return a default size
            return Vector2(1.0f, 1.0f);
        }
        return Vector2(1.0f, 1.0f); // Default size
    }

    void SpriteRenderer::setSize(const Vector2& size) {
        // For sprites, we usually scale the transform instead of changing the sprite size
        auto transform = getTransform();
        if (transform) {
            Vector2 currentSize = getSize();
            if (currentSize.x > 0 && currentSize.y > 0) {
                Vector2 scale = transform->getScale();
                transform->setScale(scale.x * (size.x / currentSize.x), 
                                   scale.y * (size.y / currentSize.y));
            }
        }
    }

    void SpriteRenderer::render() {
        // Render the sprite
        // This would involve setting up OpenGL rendering state, binding textures, etc.
        // For now, just placeholder
    }

    void SpriteRenderer::serialize(Serializer& serializer) const {
        Component::serialize(serializer);
        // Serialize sprite renderer properties
        serializer.write("color", m_color);
        serializer.write("pivot", m_pivot);
        serializer.write("tiling", m_tiling);
        serializer.write("offset", m_offset);
        serializer.write("flipX", m_flipX);
        serializer.write("flipY", m_flipY);
        serializer.write("sortingLayer", m_sortingLayer);
        serializer.write("orderInLayer", m_orderInLayer);
    }

    void SpriteRenderer::deserialize(Deserializer& deserializer) {
        Component::deserialize(deserializer);
        // Deserialize sprite renderer properties
        deserializer.read("color", m_color);
        deserializer.read("pivot", m_pivot);
        deserializer.read("tiling", m_tiling);
        deserializer.read("offset", m_offset);
        deserializer.read("flipX", m_flipX);
        deserializer.read("flipY", m_flipY);
        deserializer.read("sortingLayer", m_sortingLayer);
        deserializer.read("orderInLayer", m_orderInLayer);
    }

    void SpriteRenderer::onInspectorGUI() {
        // Draw sprite renderer properties in the inspector
    }

    void SpriteRenderer::updateMesh() {
        // Update the vertices, indices, and UVs for rendering
        if (!m_sprite) {
            // If no sprite, create a simple quad
            m_vertices = {
                Vector2(-0.5f, -0.5f),
                Vector2(0.5f, -0.5f),
                Vector2(0.5f, 0.5f),
                Vector2(-0.5f, 0.5f)
            };
            
            m_indices = {0, 1, 2, 2, 3, 0};
            
            m_uvs = {
                Vector2(0.0f, 0.0f),
                Vector2(1.0f, 0.0f),
                Vector2(1.0f, 1.0f),
                Vector2(0.0f, 1.0f)
            };
        } else {
            // If we have a sprite, update mesh based on sprite dimensions
            // This would depend on the sprite's properties
        }
        
        // Apply flipping if needed
        if (m_flipX) {
            for (auto& uv : m_uvs) {
                uv.x = 1.0f - uv.x;
            }
        }
        
        if (m_flipY) {
            for (auto& uv : m_uvs) {
                uv.y = 1.0f - uv.y;
            }
        }
    }

} // namespace CmakeProject1