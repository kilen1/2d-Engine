#include "../include/GameObject.h"
#include "../include/Transform.h"
#include "../include/Component.h"
#include <algorithm>

namespace CmakeProject1 {

    GameObject::GameObject(const std::string& objectName) : name(objectName) {
        transform = std::make_shared<Transform>();
    }

    GameObject::~GameObject() {}

    void GameObject::update(float deltaTime) {
        if (!active) return;

        for (auto& component : components) {
            if (component->isEnabled()) {
                component->update(deltaTime);
            }
        }

        for (auto& child : children) {
            child->update(deltaTime);
        }
    }

    void GameObject::render(SDL_Renderer* renderer) {
        if (!active) return;

        for (auto& component : components) {
            if (component->isEnabled()) {
                component->render(renderer);
            }
        }

        for (auto& child : children) {
            child->render(renderer);
        }
    }

    void GameObject::addChild(std::shared_ptr<GameObject> child) {
        children.push_back(child);
    }

    void GameObject::removeChild(const std::string& childName) {
        children.erase(
            std::remove_if(children.begin(), children.end(),
                [&childName](const std::shared_ptr<GameObject>& obj) {
                    return obj->getName() == childName;
                }),
            children.end()
        );
    }

} // namespace CmakeProject1