#include "AssetManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <Texture.h>

#include <AudioClip.h>
#include <Shader.h>
#include <Material.h>

#include "Font.h"
#include "AnimationClip.h"
#include "Sprite.h"

namespace CmakeProject1 {

    AssetManager::AssetManager() {}

    AssetManager::~AssetManager() {
        shutdown();
    }

    void AssetManager::initialize() {
        // Ensure asset root exists
        std::filesystem::create_directories(m_assetRoot);
    }

    void AssetManager::shutdown() {
        m_textures.clear();
        m_audioClips.clear();
        m_fonts.clear();
        m_shaders.clear();
        m_materials.clear();
        m_animations.clear();
        m_sprites.clear();
    }

    std::shared_ptr<Texture> AssetManager::loadTexture(const std::string& path) {
        std::string fullPath = getAssetPath(path);
        if (!exists(fullPath)) {
            std::cerr << "Texture file not found: " << fullPath << std::endl;
            return nullptr;
        }

        auto texture = std::make_shared<Texture>();
        // Load texture logic here (using SDL_image or OpenGL)
        // For now, just mark as loaded
        texture->loaded = true;
        texture->path = fullPath;

        m_textures[path] = texture;
        return texture;
    }

    std::shared_ptr<Texture> AssetManager::getTexture(const std::string& path) {
        auto it = m_textures.find(path);
        if (it != m_textures.end()) {
            return it->second;
        }
        return loadTexture(path);
    }

    std::shared_ptr<AudioClip> AssetManager::loadAudio(const std::string& path) {
        std::string fullPath = getAssetPath(path);
        if (!exists(fullPath)) {
            std::cerr << "Audio file not found: " << fullPath << std::endl;
            return nullptr;
        }

        auto audio = std::make_shared<AudioClip>();
        audio->path = fullPath;
        // Load audio logic here (using SDL_mixer or similar)
        audio->loaded = true;

        m_audioClips[path] = audio;
        return audio;
    }

    std::shared_ptr<AudioClip> AssetManager::getAudio(const std::string& path) {
        auto it = m_audioClips.find(path);
        if (it != m_audioClips.end()) {
            return it->second;
        }
        return loadAudio(path);
    }

    std::shared_ptr<Font> AssetManager::loadFont(const std::string& path, int size) {
        std::string fullPath = getAssetPath(path);
        if (!exists(fullPath)) {
            std::cerr << "Font file not found: " << fullPath << std::endl;
            return nullptr;
        }

        auto font = std::make_shared<Font>();
        font->path = fullPath;
        font->size = size;
        // Load font logic here (using SDL_ttf or similar)
        font->loaded = true;

        std::string key = path + "_" + std::to_string(size);
        m_fonts[key] = font;
        return font;
    }

    std::shared_ptr<Font> AssetManager::getFont(const std::string& path, int size) {
        std::string key = path + "_" + std::to_string(size);
        auto it = m_fonts.find(key);
        if (it != m_fonts.end()) {
            return it->second;
        }
        return loadFont(path, size);
    }

    std::shared_ptr<Shader> AssetManager::loadShader(const std::string& vertexPath, const std::string& fragmentPath) {
        std::string vPath = getAssetPath(vertexPath);
        std::string fPath = getAssetPath(fragmentPath);

        if (!exists(vPath)) {
            std::cerr << "Vertex shader not found: " << vPath << std::endl;
            return nullptr;
        }
        if (!exists(fPath)) {
            std::cerr << "Fragment shader not found: " << fPath << std::endl;
            return nullptr;
        }

        auto shader = std::make_shared<Shader>();
        shader->vertexPath = vPath;
        shader->fragmentPath = fPath;
        // Load shader logic here (OpenGL compilation)
        shader->loaded = true;

        std::string key = vPath + "|" + fPath;
        m_shaders[key] = shader;
        return shader;
    }

    std::shared_ptr<Shader> AssetManager::getShader(const std::string& vertexPath, const std::string& fragmentPath) {
        std::string key = getAssetPath(vertexPath) + "|" + getAssetPath(fragmentPath);
        auto it = m_shaders.find(key);
        if (it != m_shaders.end()) {
            return it->second;
        }
        return loadShader(vertexPath, fragmentPath);
    }

    std::shared_ptr<Material> AssetManager::createMaterial(const std::string& name) {
        auto material = std::make_shared<Material>();
        material->name = name;
        m_materials[name] = material;
        return material;
    }

    std::shared_ptr<Material> AssetManager::getMaterial(const std::string& name) {
        auto it = m_materials.find(name);
        if (it != m_materials.end()) {
            return it->second;
        }
        return createMaterial(name);
    }

    std::shared_ptr<AnimationClip> AssetManager::createAnimation(const std::string& name) {
        auto animation = std::make_shared<AnimationClip>();
        animation->name = name;
        m_animations[name] = animation;
        return animation;
    }

    std::shared_ptr<AnimationClip> AssetManager::getAnimation(const std::string& name) {
        auto it = m_animations.find(name);
        if (it != m_animations.end()) {
            return it->second;
        }
        return createAnimation(name);
    }

    std::shared_ptr<Sprite> AssetManager::createSprite(const std::string& name, std::shared_ptr<Texture> texture, const Rect& rect) {
        auto sprite = std::make_shared<Sprite>();
        sprite->name = name;
        sprite->texture = texture;
        sprite->sourceRect = rect;
        m_sprites[name] = sprite;
        return sprite;
    }

    std::shared_ptr<Sprite> AssetManager::getSprite(const std::string& name) {
        auto it = m_sprites.find(name);
        if (it != m_sprites.end()) {
            return it->second;
        }
        return nullptr;
    }

    std::string AssetManager::getAssetPath(const std::string& relativePath) const {
        return m_assetRoot + relativePath;
    }

    bool AssetManager::exists(const std::string& path) const {
        return std::filesystem::exists(path);
    }

} // namespace CmakeProject1