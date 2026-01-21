#pragma once
#include <string>
#include <unordered_map>
#include <memory>

namespace CmakeProject1 {

class MaterialManager {
public:
    static MaterialManager& getInstance();
    
    struct Material {
        std::string name;
        // Add material properties here
    };
    
    bool createMaterial(const std::string& name);
    Material* getMaterial(const std::string& name);
    void destroyMaterial(const std::string& name);
    void clearMaterials();

private:
    MaterialManager() = default;
    ~MaterialManager() = default;
    MaterialManager(const MaterialManager&) = delete;
    MaterialManager& operator=(const MaterialManager&) = delete;
    
    std::unordered_map<std::string, std::shared_ptr<Material>> materials;
};

} // namespace CmakeProject1