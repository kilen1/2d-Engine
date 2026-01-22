#pragma once

#include "Component.h"
#include "Types.h"
#include <memory>
#include <string>

namespace CmakeProject1 {

    class Script;
    class ScriptEngine;

    class ScriptComponent : public Component {
    public:
        ScriptComponent();
        ~ScriptComponent();

        ComponentType getType() const override { return ComponentType::ScriptComponent; }
        std::string getTypeName() const override { return "ScriptComponent"; }

        // Script
        std::shared_ptr<Script> getScript() const { return m_script; }
        void setScript(std::shared_ptr<Script> script);
        void setScript(const std::string& scriptPath);

        // Properties
        std::string getScriptPath() const { return m_scriptPath; }

        // Lifecycle
        void onStart() override;
        void onEnable() override;
        void onDisable() override;
        void update(float deltaTime) override;
        void lateUpdate(float deltaTime) override;
        void fixedUpdate(float fixedDeltaTime) override;
        void onDestroy() override;

        // Event handlers
        void onCollisionEnter(class Collision2D& collision) override;
        void onCollisionStay(class Collision2D& collision) override;
        void onCollisionExit(class Collision2D& collision) override;
        void onTriggerEnter(class Collider2D& other) override;
        void onTriggerStay(class Collider2D& other) override;
        void onTriggerExit(class Collider2D& other) override;
        void onMouseEnter() override;
        void onMouseOver() override;
        void onMouseExit() override;
        void onMouseDown(int button) override;
        void onMouseUp(int button) override;
        void onMouseClick(int button) override;

        // Script variables
        void setVariable(const std::string& name, const std::any& value);
        std::any getVariable(const std::string& name) const;
        bool hasVariable(const std::string& name) const;

        // Call script function
        std::any callFunction(const std::string& name, const std::vector<std::any>& args = {});

        // Hot reload
        void reload();

        // Serialization
        void serialize(class Serializer& serializer) const override;
        void deserialize(class Deserializer& deserializer) override;

        // Editor
        void onInspectorGUI() override;

    private:
        std::shared_ptr<Script> m_script;
        std::string m_scriptPath;

        // Script instance
        void* m_scriptInstance = nullptr;
        ScriptEngine* m_scriptEngine = nullptr;

        // Property cache for inspector
        struct ScriptProperty {
            std::string name;
            std::string type;
            std::any value;
            std::any defaultValue;
            bool exposed = false;
        };

        std::vector<ScriptProperty> m_properties;

        // Method cache
        struct ScriptMethod {
            std::string name;
            std::vector<std::string> parameterTypes;
        };

        std::vector<ScriptMethod> m_methods;

        void loadScript();
        void unloadScript();
        void createScriptInstance();
        void destroyScriptInstance();

        void updateProperties();
        void updateMethods();

        friend class ScriptEngine;
    };

} // namespace CmakeProject1