#include "SceneManager.h"
#include <iostream>
#include <Scene.h>



namespace CmakeProject1 {

    SceneManager::SceneManager() {}

    SceneManager::~SceneManager() {
        shutdown();
    }

    void SceneManager::initialize() {
        // Initialize scene system
    }

    void SceneManager::shutdown() {
        m_scenes.clear();
        m_currentScene = nullptr;
    }

    std::shared_ptr<Scene> SceneManager::createScene(const std::string& name) {
        auto scene = std::make_shared<Scene>();
        scene->setName(name);  
        m_scenes[name] = scene;
        return scene;
    }

    std::shared_ptr<Scene> SceneManager::loadScene(const std::string& scenePath) {
        // Load from file (JSON, binary, etc.)
        // For now, create empty scene
        auto scene = createScene("LoadedScene");
        // TODO: Deserialize scene from file
        onSceneLoaded(scene);
        return scene;
    }

    bool SceneManager::saveScene(const std::string& scenePath) {
        if (!m_currentScene) return false;

        // Serialize scene to file
        // TODO: Implement serialization
        std::cout << "Saving scene to: " << scenePath << std::endl;
        return true;
    }

    std::shared_ptr<Scene> SceneManager::getCurrentScene() const {
        return m_currentScene;
    }

    void SceneManager::setCurrentScene(std::shared_ptr<Scene> scene) {
        if (m_currentScene) {
            onSceneUnloaded(m_currentScene);
        }
        m_currentScene = scene;
        if (m_currentScene) {
            onSceneLoaded(m_currentScene);
        }
    }

    std::vector<std::shared_ptr<Scene>> SceneManager::getAllScenes() const {
        std::vector<std::shared_ptr<Scene>> scenes;
        for (const auto& [name, scene] : m_scenes) {
            scenes.push_back(scene);
        }
        return scenes;
    }

    std::shared_ptr<Scene> SceneManager::getSceneByName(const std::string& name) const {
        auto it = m_scenes.find(name);
        if (it != m_scenes.end()) {
            return it->second;
        }
        return nullptr;
    }

    void SceneManager::onSceneLoaded(std::shared_ptr<Scene> scene) {
        std::cout << "Scene loaded: " << scene->m_name << std::endl;
        // Notify systems that scene has changed
    }

    void SceneManager::onSceneUnloaded(std::shared_ptr<Scene> scene) {
        std::cout << "Scene unloaded: " << scene->m_name << std::endl;
        // Clean up scene resources
    }

} // namespace CmakeProject1