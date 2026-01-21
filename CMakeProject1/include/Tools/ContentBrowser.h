#pragma once

#include <string>

namespace CmakeProject1 {
    class ContentBrowser {
    public:
        virtual ~ContentBrowser() = default;
        virtual bool initialize() = 0;
        virtual void update() = 0;
        virtual void shutdown() = 0;

        virtual void show() = 0;
        virtual void hide() = 0;
        virtual void refresh() = 0;
        virtual void setRootDirectory(const std::string& path) = 0;
    };
}