#pragma once

#include "Types.h"
#include <memory>
#include <string>

namespace CmakeProject1 {

    class GameObject;
    class Scene;
    class Engine;

    class Component {
    public:
        Component() = default;
        virtual ~Component() = default;

        // Basic properties
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

        // Lifecycle methods
        virtual void onCreate() {}          // Called when component is created
        virtual void onStart() {}           // Called before first update
        virtual void onEnable() {}          // Called when component is enabled
        virtual void onDisable() {}         // Called when component is disabled
        virtual void update(float deltaTime) {} // Called every frame
        virtual void lateUpdate(float deltaTime) {} // Called after update
        virtual void fixedUpdate(float fixedDeltaTime) {} // Called at fixed rate
        virtual void render() {}            // Called during rendering
        virtual void onDestroy() {}         // Called before destruction

        // Event handlers
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

        // Serialization
        virtual void serialize(class Serializer& serializer) const {}
        virtual void deserialize(class Deserializer& deserializer) {}

        // Editor
        virtual void onInspectorGUI() {}    // Called when drawing in inspector
        virtual void onSceneGUI() {}        // Called when drawing in scene view

        // Type info
        virtual std::string getTypeName() const { return typeid(*this).name(); }
        virtual ComponentType getType() const { return ComponentType::Custom; }

    protected:
        std::weak_ptr<GameObject> m_gameObject;
        bool m_enabled = true;

        friend class GameObject;
    };

} // namespace CmakeProject1