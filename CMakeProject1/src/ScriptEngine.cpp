#include "ScriptEngine.h"
#include <iostream>

namespace CmakeProject1 {

    ScriptEngine::ScriptEngine() {}

    ScriptEngine::~ScriptEngine() {
        shutdown();
    }

    bool ScriptEngine::initialize() {
        m_initialized = true;
        std::cout << "ScriptEngine initialized." << std::endl;
        return true;
    }

    void ScriptEngine::shutdown() {
        m_scripts.clear();
        m_boundFunctions.clear();
        m_initialized = false;
    }

    bool ScriptEngine::loadScript(const std::string& path) {
        // Placeholder: in real project, you'd parse Lua/Python/JS/etc.
        std::cout << "Loading script: " << path << std::endl;
        m_scripts[path] = "dummy_script_content";
        return true;
    }

    bool ScriptEngine::executeScript(const std::string& scriptName) {
        auto it = m_scripts.find(scriptName);
        if (it == m_scripts.end()) {
            std::cerr << "Script not found: " << scriptName << std::endl;
            return false;
        }
        std::cout << "Executing script: " << scriptName << std::endl;
        // Execute logic here
        return true;
    }

    void ScriptEngine::update(float deltaTime) {
        // Run per-frame script logic
    }

    void ScriptEngine::bindFunction(const std::string& name, std::function<void()> func) {
        m_boundFunctions[name] = func;
    }

    void ScriptEngine::bindFunction(const std::string& name, std::function<void(GameObject*)> func) {
        // You can wrap this into a lambda or use variant
        m_boundFunctions[name] = [func]() { /* dummy */ };
    }

} // namespace CmakeProject1