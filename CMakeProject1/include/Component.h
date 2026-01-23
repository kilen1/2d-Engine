#pragma once

#include "Types.h"
#include <memory>
#include <string>
#include <Deserializer.h>


namespace CmakeProject1 {

    class GameObject; 
    class Transform;  
    class Scene;      
    class Engine;

    class Component {
    public:
        Component() = default;
        virtual ~Component() = default;

        
        std::shared_ptr<GameObject> getGameObject() const { return m_gameObject.lock(); }
        std::shared_ptr<Transform> getTransform() const;
        std::shared_ptr<Scene> getScene() const;
        Engine* getEngine() const;

        bool isEnabled() const { return m_enabled; }
        void setEnabled(bool enabled) {
            if (m_enabled != enabled) {
                m_enabled = enabled;
                if (m_enabled) {
                    onEnable();
                }
                else {
                    onDisable();
                }
            }
        }

      
        virtual void onCreate() {}         
        virtual void onStart() {}           
        virtual void onEnable() {}          
        virtual void onDisable() {}         
        virtual void update(float deltaTime) {}
        virtual void lateUpdate(float deltaTime) {  }
        virtual void fixedUpdate(float deltaTime) {}
        virtual void render() {}           
        virtual void onDestroy() {}         

        
        virtual void onCollisionEnter(class Collision2D& collision) {}
        virtual void onCollisionStay(class Collision2D& collision) {}
        virtual void onCollisionExit(class Collision2D& collision) {}
        virtual void onTriggerEnter(class Collider2D& other) {}
        virtual void onTriggerStay(class Collider2D& other) {}
        virtual void onTriggerExit(class Collider2D& other) {}
        virtual void onMouseEnter() {}
        virtual void onMouseOver() {}
        virtual void onMouseExit() {}
        virtual void onMouseDown(int button) {}
        virtual void onMouseUp(int button) {}
        virtual void onMouseClick(int button) {}

        
        virtual void serialize(class Serializer& serializer) const {}
        virtual void deserialize(class Deserializer& deserializer) {}

       
        virtual void onInspectorGUI() {}    
        virtual void onSceneGUI() {}      

        
        virtual std::string getTypeName() const { return typeid(*this).name(); }
        virtual ComponentType getType() const { return ComponentType::Custom; }

    protected:
        std::weak_ptr<GameObject> m_gameObject;
        bool m_enabled = true;

        friend class GameObject;
    };

} 