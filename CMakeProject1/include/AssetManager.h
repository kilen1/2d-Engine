#pragma once
#include "Types.h"
#include <string>
#include <unordered_map>
#include <memory>
#include <filesystem>

namespace CmakeProject1 {

    class AssetManager {
    public:
        AssetManager();
        ~AssetManager();

        void initialize();
        void shutdown();

        // Texture
        std::shared_ptr<Texture> loadTexture(const std::string& path);
        std::shared_ptr<Texture> getTexture(const std::string& path);

        // Audio
        std::shared_ptr<AudioClip> loadAudio(const std::string& path);
        std::shared_ptr<AudioClip> getAudio(const std::string& path);

        // Font
        std::shared_ptr<Font> loadFont(const std::string& path, int size);
        std::shared_ptr<Font> getFont(const std::string& path, int size);

        // Shader
        std::shared_ptr<Shader> loadShader(const std::string& vertexPath, const std::string& fragmentPath);
        std::shared_ptr<Shader> getShader(const std::string& vertexPath, const std::string& fragmentPath);

        // Material
        std::shared_ptr<Material> createMaterial(const std::string& name);
        std::shared_ptr<Material> getMaterial(const std::string& name);

        // Animation
        std::shared_ptr<AnimationClip> createAnimation(const std::string& name);
        std::shared_ptr<AnimationClip> getAnimation(const std::string& name);

        // Sprite
        std::shared_ptr<Sprite> createSprite(const std::string& name, std::shared_ptr<Texture> texture, const Rect& rect);
        std::shared_ptr<Sprite> getSprite(const std::string& name);

        // Utility
        std::string getAssetPath(const std::string& relativePath) const;
        bool exists(const std::string& path) const;

    private:
        std::unordered_map<std::string, std::shared_ptr<Texture>> m_textures;
        std::unordered_map<std::string, std::shared_ptr<AudioClip>> m_audioClips;
        std::unordered_map<std::string, std::shared_ptr<Font>> m_fonts;
        std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaders;
        std::unordered_map<std::string, std::shared_ptr<Material>> m_materials;
        std::unordered_map<std::string, std::shared_ptr<AnimationClip>> m_animations;
        std::unordered_map<std::string, std::shared_ptr<Sprite>> m_sprites;

        std::string m_assetRoot = "assets/";
    };

} // namespace CmakeProject1