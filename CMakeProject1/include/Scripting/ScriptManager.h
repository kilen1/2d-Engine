#pragma once

#include <string>

namespace CmakeProject1 {
    class ScriptManager {
    public:
        virtual ~ScriptManager() = default;
        virtual bool initialize() = 0;
        virtual void update() = 0;
        virtual void shutdown() = 0;

        virtual bool loadScript(const std::string& scriptPath) = 0;
        virtual bool unloadScript(const std::string& scriptName) = 0;
        virtual void reloadScripts() = 0;
        virtual void executeScript(const std::string& scriptName) = 0;
    };
}