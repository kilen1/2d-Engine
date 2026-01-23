
#include "Transform.h"

#include <Serializer.h>

#include <Deserializer.h>

#include "Component.h"
#include "GameObject.h"
#include "Scene.h"



    

 

namespace CmakeProject1 {

    GameObject::GameObject(const std::string& name) 
        : m_name(name) {
        
        m_transform = std::make_shared<Transform>();
        m_transform->m_gameObject = shared_from_this();
    }



    GameObject::~GameObject() {
        
        for (auto& component : m_components) {
            component->onDestroy();
        }
        m_components.clear();
        m_componentMap.clear();
        
       
        if (auto parent = m_parent.lock()) {
            parent->removeChild(shared_from_this());
        }
        
       
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
        if (parent == shared_from_this()) return; 
        
        
        if (auto currentParent = getParent()) {
            currentParent->removeChild(shared_from_this());
        }
        
        
        if (parent) {
            parent->addChild(shared_from_this());
        }
    }

    void GameObject::update(float deltaTime) {
        if (!m_active) return;
        
        
        for (auto& component : m_components) {
            if (component->isEnabled()) {
                component->update(deltaTime);
            }
        }
        
       
        for (auto& child : m_children) {
            child->update(deltaTime);
        }
    }

    void GameObject::lateUpdate(float deltaTime) {
        if (!m_active) return;
        
        
        for (auto& component : m_components) {
            if (component->isEnabled()) {
                component->lateUpdate(deltaTime);
            }
        }
        
      
        for (auto& child : m_children) {
            child->lateUpdate(deltaTime);
        }
    }

    void GameObject::render() {
        if (!m_active) return;
        
       
        for (auto& component : m_components) {
            if (component->isEnabled()) {
                component->render();
            }
        }
        
       
        for (auto& child : m_children) {
            child->render();
        }
    }

    void GameObject::onDestroy() {
     
        for (auto& component : m_components) {
            component->onDestroy();
        }
        
        
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

      
        if (m_transform) {
            serializer.writeObject("transform", "Transform");
            m_transform->serialize(serializer); 
            serializer.endObject();
        }

        // Serialize components
        serializer.write("components_count", static_cast<int>(m_components.size()));
        for (size_t i = 0; i < m_components.size(); ++i) {
            std::string key = "component_" + std::to_string(i);
            serializer.writeObject(key, m_components[i]->getTypeName()); 
            m_components[i]->serialize(serializer);
            serializer.endObject();
        }

     
        serializer.write("children_count", static_cast<int>(m_children.size()));
        for (size_t i = 0; i < m_children.size(); ++i) {
            std::string key = "child_" + std::to_string(i);
            serializer.writeObject(key, "GameObject");
            m_children[i]->serialize(serializer);
            serializer.endObject();
        }
    }

    void GameObject::deserialize(Deserializer& deserializer) {
        deserializer.read("name", m_name);
        deserializer.read("tag", m_tag);
        deserializer.read("layer", m_layer);
        deserializer.read("active", m_active);
        deserializer.read("static", m_static);
        
        
        
        
        int componentsCount;
        deserializer.read("components_count", componentsCount);
        for (int i = 0; i < componentsCount; ++i) {
            
            
        }
        
        
        int childrenCount;
        deserializer.read("children_count", childrenCount);
        for (int i = 0; i < childrenCount; ++i) {
            
        }
    }

    std::shared_ptr<GameObject> GameObject::clone() const {
        auto cloned = std::make_shared<GameObject>(m_name + " (Clone)");
        
       
        cloned->m_tag = m_tag;
        cloned->m_layer = m_layer;
        cloned->m_active = m_active;
        cloned->m_static = m_static;
        
      
        if (m_transform) {
            auto clonedTransform = cloned->addComponent<Transform>();
            
        }
        
      
        for (const auto& component : m_components) {
            
        }
        
        
        for (const auto& child : m_children) {
            auto clonedChild = child->clone();
            cloned->addChild(clonedChild);
        }
        
        return cloned;
    }

    std::shared_ptr<Scene> GameObject::getScene() const {
       
        return m_scene.lock();
    }


} 