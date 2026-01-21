#pragma once

#include <string>

namespace CmakeProject1 {
    class Editor {
    public:
        virtual ~Editor() = default;
        virtual bool initialize() = 0;
        virtual void update() = 0;
        virtual void shutdown() = 0;

        virtual void show() = 0;
        virtual void hide() = 0;
        virtual bool isVisible() const = 0;
        
        virtual void openSceneInEditor(const std::string& scenePath) = 0;
        virtual void saveCurrentScene() = 0;
    };
}