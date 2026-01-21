#pragma once

#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include <typeindex>
#include <SDL2/SDL.h>

namespace CmakeProject1 {

    class Component;
    class Transform;

    class GameObject : public std::enable_shared_from_this<GameObject> {
    public:
        GameObject(const std::string& objectName = "GameObject");
        ~GameObject();

        const std::vector<std::shared_ptr<Component>>& getComponents() const { return components; }

        void update(float deltaTime);
        void render(SDL_Renderer* renderer);

        template<typename T>
        std::shared_ptr<T> addComponent();

        template<typename T>
        std::shared_ptr<T> getComponent();

        void setName(const std::string& newName) { name = newName; }
        const std::string& getName() const { return name; }

        void setActive(bool isActive) { active = isActive; }
        bool isActive() const { return active; }

        std::shared_ptr<Transform> getTransform() { return transform; }

        void addChild(std::shared_ptr<GameObject> child);
        void removeChild(const std::string& childName);

    private:
        std::string name;
        bool active = true;

        std::shared_ptr<Transform> transform;
        std::vector<std::shared_ptr<Component>> components;
        std::vector<std::shared_ptr<GameObject>> children;

        std::unordered_map<std::type_index, std::shared_ptr<Component>> componentMap;
    };

    // Template method implementations
    template<typename T>
    std::shared_ptr<T> GameObject::addComponent() {
        static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");

        auto component = std::make_shared<T>();
        component->setOwner(shared_from_this());

        components.push_back(component);
        componentMap[std::type_index(typeid(T))] = component;

        return component;
    }

    template<typename T>
    std::shared_ptr<T> GameObject::getComponent() {
        static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");

        auto it = componentMap.find(std::type_index(typeid(T)));
        if (it != componentMap.end()) {
            return std::static_pointer_cast<T>(it->second);
        }
        return nullptr;
    }
} // namespace CmakeProject1