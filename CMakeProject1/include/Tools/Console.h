#pragma once

#include <string>
#include <vector>
#include <functional>

namespace CmakeProject1 {
    class Console {
    public:
        virtual ~Console() = default;
        virtual bool initialize() = 0;
        virtual void update() = 0;
        virtual void shutdown() = 0;

        virtual void show() = 0;
        virtual void hide() = 0;
        virtual void executeCommand(const std::string& command) = 0;
        virtual void registerCommand(const std::string& command, std::function<void(const std::vector<std::string>&)> handler) = 0;
        virtual void logMessage(const std::string& message) = 0;
    };
}