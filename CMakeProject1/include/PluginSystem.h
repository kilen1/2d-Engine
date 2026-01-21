#pragma once
#include <string>

namespace CmakeProject1 {

class PluginSystem {
public:
    static PluginSystem& getInstance();
    
    struct PluginHandle {
        void* handle;
        std::string name;
        bool loaded;
    };
    
    PluginHandle* loadPlugin(const std::string& pluginPath);
    bool unloadPlugin(const std::string& pluginName);
    void* getPluginFunction(PluginHandle* plugin, const std::string& functionName);
    void updatePlugins();

private:
    PluginSystem() = default;
    ~PluginSystem() = default;
    PluginSystem(const PluginSystem&) = delete;
    PluginSystem& operator=(const PluginSystem&) = delete;
    
    std::vector<PluginHandle*> loadedPlugins;
};

} // namespace CmakeProject1