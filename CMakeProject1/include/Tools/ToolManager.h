#pragma once

#include <string>

namespace CmakeProject1 {
    class ToolManager {
    public:
        virtual ~ToolManager() = default;
        virtual bool initialize() = 0;
        virtual void update() = 0;
        virtual void shutdown() = 0;

        virtual void registerTool(const std::string& toolName, void* toolPtr) = 0;
        virtual void activateTool(const std::string& toolName) = 0;
        virtual void deactivateTool(const std::string& toolName) = 0;
    };
}