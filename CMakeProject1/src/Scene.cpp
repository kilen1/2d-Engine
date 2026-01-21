#include "../include/Scene.h"
#include "../include/Engine.h"
#include <algorithm>

namespace CmakeProject1 {

    Scene::Scene(const std::string& sceneName) : name(sceneName) {}

    Scene::~Scene() {}

    void Scene::update(float deltaTime) {
        for (auto& gameObject : gameObjects) {
            gameObject->update(deltaTime);
        }
    }

    void Scene::render(SDL_Renderer* renderer) {
        for (auto& gameObject : gameObjects) {
            gameObject->render(renderer);
        }
    }

    std::shared_ptr<GameObject> Scene::createGameObject(const std::string& objectName) {
        auto gameObject = std::make_shared<GameObject>(objectName.empty() ? "GameObject" : objectName);
        addGameObject(gameObject);
        return gameObject;
    }

    void Scene::destroyGameObject(const std::string& objectName) {
        gameObjects.erase(
            std::remove_if(gameObjects.begin(), gameObjects.end(),
                [&objectName](const std::shared_ptr<GameObject>& obj) {
                    return obj->getName() == objectName;
                }),
            gameObjects.end()
        );
    }

    void Scene::addGameObject(std::shared_ptr<GameObject> gameObject) {
        gameObjects.push_back(gameObject);
    }

    void Scene::removeGameObject(std::shared_ptr<GameObject> gameObject) {
        gameObjects.erase(
            std::remove(gameObjects.begin(), gameObjects.end(), gameObject),
            gameObjects.end()
        );
    }

} // namespace CmakeProject1