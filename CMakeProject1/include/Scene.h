#pragma once

#include "GameObject.h"
#include <vector>
#include <string>
#include <memory>

namespace CmakeProject1 {

    class Scene {
    public:
        Scene(const std::string& sceneName);
        ~Scene();

        void update(float deltaTime);
        void render(SDL_Renderer* renderer);

        std::shared_ptr<GameObject> createGameObject(const std::string& name = "");
        void destroyGameObject(const std::string& objectName);

        const std::string& getName() const { return name; }
        void addGameObject(std::shared_ptr<GameObject> gameObject);
        void removeGameObject(std::shared_ptr<GameObject> gameObject);

        const std::vector<std::shared_ptr<GameObject>>& getGameObjects() const { return gameObjects; }

    private:
        std::string name;
        std::vector<std::shared_ptr<GameObject>> gameObjects;
    };

} // namespace CmakeProject1