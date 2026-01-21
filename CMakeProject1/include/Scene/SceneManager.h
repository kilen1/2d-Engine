#pragma once

#include <string>

namespace CmakeProject1 {
    class SceneManager {
    public:
        virtual ~SceneManager() = default;
        virtual bool initialize() = 0;
        virtual void update() = 0;
        virtual void shutdown() = 0;

        virtual bool loadScene(const std::string& scenePath) = 0;
        virtual bool saveScene(const std::string& scenePath) = 0;
        virtual void createNewScene(const std::string& name) = 0;
        virtual void unloadCurrentScene() = 0;
    };
}