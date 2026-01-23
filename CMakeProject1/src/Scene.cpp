#include "../include/Scene.h"
#include "../include/GameObject.h"
#include "../include/Camera.h"
#include "../include/Light2D.h"
#include <Transform.h>
#include <algorithm>

namespace CmakeProject1 {

    Scene::Scene(const std::string& name) 
        : m_name(name) {
        // Initialize scene with default settings
        m_settings = Settings();
    }

    Scene::~Scene() {
        // Clean up all game objects
        for (auto& gameObject : m_gameObjects) {
            gameObject->onDestroy();
        }
        m_gameObjects.clear();
        m_rootGameObjects.clear();
    }

    std::shared_ptr<GameObject> Scene::createGameObject(const std::string& name) {
        auto gameObject = std::make_shared<GameObject>(name);
        
        m_gameObjects.push_back(gameObject);
        m_rootGameObjects.push_back(gameObject);
        
        // Add to name lookup
        m_nameLookup[name] = gameObject;
        
        m_dirty = true;
        return gameObject;
    }

    std::shared_ptr<GameObject> Scene::instantiate(std::shared_ptr<GameObject> prefab, 
                                                  const Vector2& position, float rotation) {
        if (!prefab) return nullptr;
        
        auto instance = prefab->clone();
        
        // Apply position and rotation to the root transform
        if (instance->getTransform()) {
            instance->getTransform()->setPosition(position);
            instance->getTransform()->setRotation(rotation);
        }
        
        m_gameObjects.push_back(instance);
        m_rootGameObjects.push_back(instance);
        
        // Add to name lookup
        m_nameLookup[instance->getName()] = instance;
        
        m_dirty = true;
        return instance;
    }

    void Scene::destroy(std::shared_ptr<GameObject> gameObject) {
        if (!gameObject) return;
        
        // Add to destruction queue to be processed later
        m_destructionQueue.push_back(gameObject);
    }

    std::shared_ptr<GameObject> Scene::findGameObject(const std::string& name) const {
        auto it = m_nameLookup.find(name);
        if (it != m_nameLookup.end()) {
            return it->second;
        }
        return nullptr;
    }

    std::vector<std::shared_ptr<GameObject>> Scene::findGameObjectsWithTag(const std::string& tag) const {
        std::vector<std::shared_ptr<GameObject>> result;
        auto range = m_tagLookup.equal_range(tag);
        for (auto it = range.first; it != range.second; ++it) {
            result.push_back(it->second);
        }
        return result;
    }

    std::shared_ptr<GameObject> Scene::findGameObjectWithTag(const std::string& tag) const {
        auto range = m_tagLookup.equal_range(tag);
        if (range.first != range.second) {
            return range.first->second;
        }
        return nullptr;
    }

    std::vector<std::shared_ptr<GameObject>> Scene::getAllGameObjects() const {
        return m_gameObjects;
    }

    std::vector<std::shared_ptr<GameObject>> Scene::getRootGameObjects() const {
        return m_rootGameObjects;
    }

    std::shared_ptr<Camera> Scene::getMainCamera() const {
        return m_mainCamera.lock();
    }

    void Scene::setMainCamera(std::shared_ptr<Camera> camera) {
        if (camera) {
            m_mainCamera = camera;
        }
    }

    std::vector<std::shared_ptr<Camera>> Scene::getAllCameras() const {
        std::vector<std::shared_ptr<Camera>> cameras;
        
        for (const auto& gameObject : m_gameObjects) {
            auto camera = gameObject->getComponent<Camera>();
            if (camera) {
                cameras.push_back(camera);
            }
        }
        
        return cameras;
    }

    std::vector<std::shared_ptr<Light2D>> Scene::getAllLights() const {
        std::vector<std::shared_ptr<Light2D>> lights;
        
        for (const auto& gameObject : m_gameObjects) {
            auto light = gameObject->getComponent<Light2D>();
            if (light) {
                lights.push_back(light);
            }
        }
        
        return lights;
    }

    void Scene::load() {
        m_loaded = true;
        m_dirty = false;
    }

    void Scene::unload() {
        m_loaded = false;
        // Clean up game objects
        for (auto& gameObject : m_gameObjects) {
            gameObject->onDestroy();
        }
        m_gameObjects.clear();
        m_rootGameObjects.clear();
        m_nameLookup.clear();
        m_tagLookup.clear();
    }

    void Scene::update(float deltaTime) {
        if (!m_loaded) return;
        
        // Process destruction queue first
        processDestructionQueue();
        
        // Update all root game objects (which will update their children recursively)
        for (auto& rootGO : m_rootGameObjects) {
            rootGO->update(deltaTime);
        }
    }

    void Scene::fixedUpdate(float fixedDeltaTime) {
        if (!m_loaded) return;
        
        // Fixed update all root game objects
        for (auto& rootGO : m_rootGameObjects) {
            // Call fixed update on components through the game object
            rootGO->lateUpdate(fixedDeltaTime);
        }
    }

    void Scene::lateUpdate(float deltaTime) {
        if (!m_loaded) return;
        
        // Late update all root game objects
        for (auto& rootGO : m_rootGameObjects) {
            rootGO->lateUpdate(deltaTime);
        }
    }

    void Scene::render() {
        if (!m_loaded) return;
        
        // Render all root game objects
        for (auto& rootGO : m_rootGameObjects) {
            rootGO->render();
        }
    }

    bool Scene::save(const std::string& path) {
        // Implementation would serialize the scene to a file
        return true;
    }

    bool Scene::loadFromFile(const std::string& path) {
        // Implementation would deserialize the scene from a file
        m_loaded = true;
        return true;
    }

    void Scene::processDestructionQueue() {
        for (auto& gameObject : m_destructionQueue) {
            // Remove from game objects list
            auto it = std::find(m_gameObjects.begin(), m_gameObjects.end(), gameObject);
            if (it != m_gameObjects.end()) {
                m_gameObjects.erase(it);
            }
            
            // Remove from root game objects if it's a root
            auto rootIt = std::find(m_rootGameObjects.begin(), m_rootGameObjects.end(), gameObject);
            if (rootIt != m_rootGameObjects.end()) {
                m_rootGameObjects.erase(rootIt);
            }
            
            // Remove from name lookup
            m_nameLookup.erase(gameObject->getName());
            
            // Remove from tag lookup
            m_tagLookup.erase(gameObject->getTag());
            
            // Call destroy on the game object
            gameObject->onDestroy();
        }
        
        m_destructionQueue.clear();
        m_dirty = true;
    }

} // namespace CmakeProject1