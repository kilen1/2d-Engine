#pragma once
#include <string>
#include <unordered_map>
#include <memory>

namespace CmakeProject1 {

class ShaderManager {
public:
    static ShaderManager& getInstance();
    
    bool loadShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);
    void* getShader(const std::string& name);
    void unloadShader(const std::string& name);
    void clearShaders();

private:
    ShaderManager() = default;
    ~ShaderManager() = default;
    ShaderManager(const ShaderManager&) = delete;
    ShaderManager& operator=(const ShaderManager&) = delete;
    
    std::unordered_map<std::string, std::shared_ptr<void>> shaders;
};

} // namespace CmakeProject1