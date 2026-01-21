#pragma once
#include <string>
#include <vector>

namespace CmakeProject1 {

class ModdingSystem {
public:
    static ModdingSystem& getInstance();
    
    struct ModInfo {
        std::string id;
        std::string name;
        std::string version;
        std::string author;
        bool enabled;
    };
    
    bool loadMod(const std::string& modPath);
    bool unloadMod(const std::string& modId);
    void enableMod(const std::string& modId);
    void disableMod(const std::string& modId);
    std::vector<ModInfo> getLoadedMods() const;
    bool validateMod(const std::string& modPath);

private:
    ModdingSystem() = default;
    ~ModdingSystem() = default;
    ModdingSystem(const ModdingSystem&) = delete;
    ModdingSystem& operator=(const ModdingSystem&) = delete;
    
    std::vector<ModInfo> loadedMods;
};

} // namespace CmakeProject1