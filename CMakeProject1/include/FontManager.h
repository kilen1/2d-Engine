#pragma once
#include <string>
#include <unordered_map>
#include <memory>

namespace CmakeProject1 {

class FontManager {
public:
    static FontManager& getInstance();
    
    struct FontData {
        std::string name;
        void* fontPtr; // Pointer to actual font implementation
    };
    
    bool loadFont(const std::string& name, const std::string& filepath, int size);
    FontData* getFont(const std::string& name);
    void unloadFont(const std::string& name);
    void clearFonts();

private:
    FontManager() = default;
    ~FontManager() = default;
    FontManager(const FontManager&) = delete;
    FontManager& operator=(const FontManager&) = delete;
    
    std::unordered_map<std::string, std::shared_ptr<FontData>> fonts;
};

} // namespace CmakeProject1