#pragma once

#include "Types.h"
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>

namespace CmakeProject1 {

    class Component;
    class Transform;

    class GameObject : public std::enable_shared_from_this<GameObject> {
    public:
        GameObject(const std::string& name = "GameObject");
        virtual ~GameObject();

        // Basic properties
        std::string getName() const { return m_name; }
        void setName(const std::string& name) { m_name = name; }

        std::string getTag() const { return m_tag; }
        void setTag(const std::string& tag) { m_tag = tag; }

        int getLayer() const { return m_layer; }
        void setLayer(int layer) { m_layer = layer; }

        bool isActive() const { return m_active; }
        void setActive(bool active) { m_active = active; }

        bool isStatic() const { return m_static; }
        void setStatic(bool isStatic) { m_static = isStatic; }

        // Transform
        std::shared_ptr<Transform> getTransform() const { return m_transform; }

        // Component management
        template<typename T>
        std::shared_ptr<T> addComponent() {
            static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");

            auto component = std::make_shared<T>();
            component->m_gameObject = shared_from_this();
            component->onCreate();

            m_components.push_back(component);
            m_componentMap[typeid(T).hash_code()] = component;

            component->onStart();
            return std::dynamic_pointer_cast<T>(component);
        }

        template<typename T>
        std::shared_ptr<T> getComponent() const {
            static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");

            auto it = m_componentMap.find(typeid(T).hash_code());
            if (it != m_componentMap.end()) {
                return std::dynamic_pointer_cast<T>(it->second);
            }
            return nullptr;
        }

        template<typename T>
        std::vector<std::shared_ptr<T>> getComponents() const {
            static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");

            std::vector<std::shared_ptr<T>> components;
            for (const auto& component : m_components) {
                if (auto casted = std::dynamic_pointer_cast<T>(component)) {
                    components.push_back(casted);
                }
            }
            return components;
        }

        template<typename T>
        bool hasComponent() const {
            return getComponent<T>() != nullptr;
        }

        template<typename T>
        void removeComponent() {
            static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");

            size_t hash = typeid(T).hash_code();
            auto it = m_componentMap.find(hash);
            if (it != m_componentMap.end()) {
                it->second->onDestroy();
                m_componentMap.erase(it);

                // Remove from vector
                m_components.erase(
                    std::remove_if(m_components.begin(), m_components.end(),
                        [hash](const std::shared_ptr<Component>& comp) {
                            return typeid(*comp).hash_code() == hash;
                        }),
                    m_components.end()
                );
            }
        }

        // Child management
        std::shared_ptr<GameObject> addChild(const std::string& name = "GameObject");
        void addChild(std::shared_ptr<GameObject> child);
        void removeChild(std::shared_ptr<GameObject> child);
        std::vector<std::shared_ptr<GameObject>> getChildren() const;
        std::shared_ptr<GameObject> getChild(const std::string& name) const;
        std::shared_ptr<GameObject> getParent() const { return m_parent.lock(); }
        void setParent(std::shared_ptr<GameObject> parent);

        // Lifecycle
        void update(float deltaTime);
        void lateUpdate(float deltaTime);
        void render();
        void onDestroy();

        // Serialization
        virtual void serialize(class Serializer& serializer) const;
        virtual void deserialize(class Deserializer& deserializer);

        // Instantiation
        std::shared_ptr<GameObject> clone() const;

    private:
        std::string m_name;
        std::string m_tag = "Untagged";
        int m_layer = 0;
        bool m_active = true;
        bool m_static = false;

        std::weak_ptr<GameObject> m_parent;
        std::vector<std::shared_ptr<GameObject>> m_children;

        std::shared_ptr<Transform> m_transform;
        std::vector<std::shared_ptr<Component>> m_components;
        std::unordered_map<size_t, std::shared_ptr<Component>> m_componentMap;

        // For prefabs
        std::string m_prefabPath;

        friend class Scene;
        friend class PrefabSystem;
    };

} // namespace CmakeProject1