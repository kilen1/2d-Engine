#pragma once

#include <string>

namespace CmakeProject1 {
    class HotReload {
    public:
        virtual ~HotReload() = default;
        virtual bool initialize() = 0;
        virtual void update() = 0;
        virtual void shutdown() = 0;

        virtual bool watchFile(const std::string& filePath) = 0;
        virtual bool reloadScript(const std::string& scriptPath) = 0;
        virtual void setEnabled(bool enabled) = 0;
    };
}