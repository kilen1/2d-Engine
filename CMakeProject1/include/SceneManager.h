#pragma once
#include "Types.h"
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>

namespace CmakeProject1 {

    class SceneManager {
    public:
        SceneManager();
        ~SceneManager();

        void initialize();
        void shutdown();

        std::shared_ptr<Scene> createScene(const std::string& name);
        std::shared_ptr<Scene> loadScene(const std::string& scenePath);
        bool saveScene(const std::string& scenePath = "");
        std::shared_ptr<Scene> getCurrentScene() const;
        void setCurrentScene(std::shared_ptr<Scene> scene);

        std::vector<std::shared_ptr<Scene>> getAllScenes() const;
        std::shared_ptr<Scene> getSceneByName(const std::string& name) const;

        // Scene lifecycle
        void onSceneLoaded(std::shared_ptr<Scene> scene);
        void onSceneUnloaded(std::shared_ptr<Scene> scene);

    private:
        std::shared_ptr<Scene> m_currentScene;
        std::unordered_map<std::string, std::shared_ptr<Scene>> m_scenes;
    };

} // namespace CmakeProject1