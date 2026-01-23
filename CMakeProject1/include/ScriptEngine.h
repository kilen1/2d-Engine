#pragma once
#include "Types.h"
#include <string>
#include <memory>
#include <unordered_map>

namespace CmakeProject1 {

    class ScriptEngine {
    public:
        ScriptEngine();
        ~ScriptEngine();

        bool initialize();
        void shutdown();

        // Script loading and execution
        bool loadScript(const std::string& path);
        bool executeScript(const std::string& scriptName);
        void update(float deltaTime);

        // Binding functions to scripts
        void bindFunction(const std::string& name, std::function<void()> func);
        void bindFunction(const std::string& name, std::function<void(GameObject*)> func);

        // Getters
        bool isInitialized() const { return m_initialized; }

    private:
        bool m_initialized = false;
        std::unordered_map<std::string, std::string> m_scripts;
        std::unordered_map<std::string, std::function<void()>> m_boundFunctions;
    };

} // namespace CmakeProject1