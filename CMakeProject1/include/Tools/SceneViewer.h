#pragma once

#include <string>

namespace CmakeProject1 {
    class SceneViewer {
    public:
        virtual ~SceneViewer() = default;
        virtual bool initialize() = 0;
        virtual void update() = 0;
        virtual void shutdown() = 0;

        virtual void show() = 0;
        virtual void hide() = 0;
        virtual void loadScene(const std::string& scenePath) = 0;
        virtual void render() = 0;
    };
}