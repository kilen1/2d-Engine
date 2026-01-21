#pragma once

#include <string>

namespace CmakeProject1 {
    class Inspector {
    public:
        virtual ~Inspector() = default;
        virtual bool initialize() = 0;
        virtual void update() = 0;
        virtual void shutdown() = 0;

        virtual void show() = 0;
        virtual void hide() = 0;
        virtual void inspectObject(const std::string& objectName) = 0;
        virtual void render() = 0;
    };
}