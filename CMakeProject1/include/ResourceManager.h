#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include "Core/AudioEngine.h"

namespace CmakeProject1 {

class ResourceManager {
public:
    static ResourceManager& getInstance();
    
    template<typename T>
    std::shared_ptr<T> loadResource(const std::string& name, const std::string& filepath) {
        // Implementation would depend on resource type
        return nullptr;
    }
    
    template<typename T>
    std::shared_ptr<T> getResource(const std::string& name) {
        // Implementation would depend on resource type
        return nullptr;
    }
    
    void unloadResource(const std::string& name);
    void clearCache();

private:
    ResourceManager() = default;
    ~ResourceManager() = default;
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    
    std::unordered_map<std::string, std::shared_ptr<void>> resources;
};

} // namespace CmakeProject1