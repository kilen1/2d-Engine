#pragma once
#include <string>
#include <unordered_map>
#include <memory>

namespace CmakeProject1 {

class TextureManager {
public:
    static TextureManager& getInstance();
    
    struct TextureData {
        std::string name;
        unsigned int id;
        int width, height;
        int channels;
    };
    
    bool loadTexture(const std::string& name, const std::string& filepath);
    TextureData* getTexture(const std::string& name);
    void unloadTexture(const std::string& name);
    void clearTextures();

private:
    TextureManager() = default;
    ~TextureManager() = default;
    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;
    
    std::unordered_map<std::string, std::shared_ptr<TextureData>> textures;
};

} // namespace CmakeProject1