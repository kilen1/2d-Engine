#pragma once
#include <string>
#include <unordered_map>
#include <memory>

namespace CmakeProject1 {

class PrefabManager {
public:
    static PrefabManager& getInstance();
    
    struct Prefab {
        std::string name;
        // Data representing the prefab template
    };
    
    bool loadPrefab(const std::string& name, const std::string& filepath);
    Prefab* getPrefab(const std::string& name);
    void instantiatePrefab(const std::string& name);
    void clearPrefabs();

private:
    PrefabManager() = default;
    ~PrefabManager() = default;
    PrefabManager(const PrefabManager&) = delete;
    PrefabManager& operator=(const PrefabManager&) = delete;
    
    std::unordered_map<std::string, std::shared_ptr<Prefab>> prefabs;
};

} // namespace CmakeProject1