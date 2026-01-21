#pragma once

#include <string>
#include <memory>

namespace CmakeProject1 {
    class ScriptEngine {
    public:
        virtual ~ScriptEngine() = default;
        virtual bool initialize() = 0;
        virtual void update() = 0;
        virtual void shutdown() = 0;

        virtual bool loadScript(const std::string& path) = 0;
        virtual bool executeScript(const std::string& code) = 0;
        virtual void bindFunction(const std::string& name, void(*func)()) = 0;
    };
}