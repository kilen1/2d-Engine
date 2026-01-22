#include "../include/GameObject.h"
#include "../include/Transform.h"

namespace CmakeProject1 {

    GameObject::GameObject(const std::string& name) 
        : m_name(name) {
        // Create a default transform component
        m_transform = std::make_shared<Transform>();
        m_transform->m_gameObject = shared_from_this();
    }

    GameObject::~GameObject() {
        // Clean up components and children
        for (auto& component : m_components) {
            component->onDestroy();
        }
        m_components.clear();
        m_componentMap.clear();
        
        // Remove from parent if we have one
        if (auto parent = m_parent.lock()) {
            parent->removeChild(shared_from_this());
        }
        
        // Destroy all children
        for (auto& child : m_children) {
            child->m_parent.reset();
        }
        m_children.clear();
    }

    std::shared_ptr<GameObject> GameObject::addChild(const std::string& name) {
        auto child = std::make_shared<GameObject>(name);
        addChild(child);
        return child;
    }

    void GameObject::addChild(std::shared_ptr<GameObject> child) {
        if (!child) return;
        
        // Remove from current parent if it has one
        if (auto currentParent = child->getParent()) {
            currentParent->removeChild(child);
        }
        
        m_children.push_back(child);
        child->m_parent = shared_from_this();
    }

    void GameObject::removeChild(std::shared_ptr<GameObject> child) {
        if (!child) return;
        
        auto it = std::find(m_children.begin(), m_children.end(), child);
        if (it != m_children.end()) {
            m_children.erase(it);
            child->m_parent.reset();
        }
    }

    std::vector<std::shared_ptr<GameObject>> GameObject::getChildren() const {
        return m_children;
    }

    std::shared_ptr<GameObject> GameObject::getChild(const std::string& name) const {
        for (const auto& child : m_children) {
            if (child->getName() == name) {
                return child;
            }
        }
        return nullptr;
    }

    void GameObject::setParent(std::shared_ptr<GameObject> parent) {
        if (parent == shared_from_this()) return; // Cannot be own parent
        
        // Remove from current parent
        if (auto currentParent = getParent()) {
            currentParent->removeChild(shared_from_this());
        }
        
        // Add to new parent
        if (parent) {
            parent->addChild(shared_from_this());
        }
    }

    void GameObject::update(float deltaTime) {
        if (!m_active) return;
        
        // Update all components
        for (auto& component : m_components) {
            if (component->isEnabled()) {
                component->update(deltaTime);
            }
        }
        
        // Update all children
        for (auto& child : m_children) {
            child->update(deltaTime);
        }
    }

    void GameObject::lateUpdate(float deltaTime) {
        if (!m_active) return;
        
        // Late update all components
        for (auto& component : m_components) {
            if (component->isEnabled()) {
                component->lateUpdate(deltaTime);
            }
        }
        
        // Late update all children
        for (auto& child : m_children) {
            child->lateUpdate(deltaTime);
        }
    }

    void GameObject::render() {
        if (!m_active) return;
        
        // Render all components
        for (auto& component : m_components) {
            if (component->isEnabled()) {
                component->render();
            }
        }
        
        // Render all children
        for (auto& child : m_children) {
            child->render();
        }
    }

    void GameObject::onDestroy() {
        // Call destroy on all components
        for (auto& component : m_components) {
            component->onDestroy();
        }
        
        // Destroy all children
        for (auto& child : m_children) {
            child->onDestroy();
        }
    }

    void GameObject::serialize(Serializer& serializer) const {
        serializer.write("name", m_name);
        serializer.write("tag", m_tag);
        serializer.write("layer", m_layer);
        serializer.write("active", m_active);
        serializer.write("static", m_static);
        
        // Serialize transform
        if (m_transform) {
            serializer.write("transform", *m_transform);
        }
        
        // Serialize components
        serializer.write("components_count", static_cast<int>(m_components.size()));
        for (size_t i = 0; i < m_components.size(); ++i) {
            serializer.write("component_" + std::to_string(i), *m_components[i]);
        }
        
        // Serialize children
        serializer.write("children_count", static_cast<int>(m_children.size()));
        for (size_t i = 0; i < m_children.size(); ++i) {
            serializer.write("child_" + std::to_string(i), *m_children[i]);
        }
    }

    void GameObject::deserialize(Deserializer& deserializer) {
        deserializer.read("name", m_name);
        deserializer.read("tag", m_tag);
        deserializer.read("layer", m_layer);
        deserializer.read("active", m_active);
        deserializer.read("static", m_static);
        
        // Deserialize transform
        if (m_transform) {
            deserializer.read("transform", *m_transform);
        }
        
        // Deserialize components
        int componentsCount;
        deserializer.read("components_count", componentsCount);
        for (int i = 0; i < componentsCount; ++i) {
            // We'd need to know the component type to recreate it properly
            // This is a simplified approach
        }
        
        // Deserialize children
        int childrenCount;
        deserializer.read("children_count", childrenCount);
        for (int i = 0; i < childrenCount; ++i) {
            // We'd need to recreate children properly
        }
    }

    std::shared_ptr<GameObject> GameObject::clone() const {
        auto cloned = std::make_shared<GameObject>(m_name + " (Clone)");
        
        // Copy basic properties
        cloned->m_tag = m_tag;
        cloned->m_layer = m_layer;
        cloned->m_active = m_active;
        cloned->m_static = m_static;
        
        // Clone transform
        if (m_transform) {
            auto clonedTransform = cloned->addComponent<Transform>();
            // Copy transform properties
        }
        
        // Clone components
        for (const auto& component : m_components) {
            // Clone each component (would need reflection or factory pattern)
        }
        
        // Clone children
        for (const auto& child : m_children) {
            auto clonedChild = child->clone();
            cloned->addChild(clonedChild);
        }
        
        return cloned;
    }

} // namespace CmakeProject1