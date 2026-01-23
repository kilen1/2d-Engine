#include "ScriptComponent.h"
#include <Deserializer.h>
#include <Serializer.h>

namespace CmakeProject1 {

    ScriptComponent::ScriptComponent() {
        // Initialize script component
    }

    ScriptComponent::~ScriptComponent() {
        unloadScript();
    }

    

    void ScriptComponent::setScript(const std::string& scriptPath) {
        unloadScript();
        m_scriptPath = scriptPath;
        loadScript();
    }

    void ScriptComponent::onStart() {
        Component::onStart();
        if (m_scriptInstance) {
            // Call the start method on the script instance
        }
    }

    void ScriptComponent::onEnable() {
        Component::onEnable();
        if (m_scriptInstance) {
            // Call the onEnable method on the script instance
        }
    }

    void ScriptComponent::onDisable() {
        Component::onDisable();
        if (m_scriptInstance) {
            // Call the onDisable method on the script instance
        }
    }

    void ScriptComponent::update(float deltaTime) {
        Component::update(deltaTime);
        if (m_scriptInstance && m_enabled) {
            // Call the update method on the script instance
        }
    }

    void ScriptComponent::lateUpdate(float deltaTime) {
        Component::lateUpdate(deltaTime);
        if (m_scriptInstance && m_enabled) {
            // Call the lateUpdate method on the script instance
        }
    }

    void ScriptComponent::fixedUpdate(float fixedDeltaTime) {
        Component::fixedUpdate(fixedDeltaTime);
        if (m_scriptInstance && m_enabled) {
            // Call the fixedUpdate method on the script instance
        }
    }

    void ScriptComponent::onDestroy() {
        if (m_scriptInstance) {
            // Call the onDestroy method on the script instance
        }
        unloadScript();
        Component::onDestroy();
    }

    void ScriptComponent::onCollisionEnter(Collision2D& collision) {
        Component::onCollisionEnter(collision);
        if (m_scriptInstance) {
            // Call the onCollisionEnter method on the script instance
        }
    }

    void ScriptComponent::onCollisionStay(Collision2D& collision) {
        Component::onCollisionStay(collision);
        if (m_scriptInstance) {
            // Call the onCollisionStay method on the script instance
        }
    }

    void ScriptComponent::onCollisionExit(Collision2D& collision) {
        Component::onCollisionExit(collision);
        if (m_scriptInstance) {
            // Call the onCollisionExit method on the script instance
        }
    }

    void ScriptComponent::onTriggerEnter(Collider2D& other) {
        Component::onTriggerEnter(other);
        if (m_scriptInstance) {
            // Call the onTriggerEnter method on the script instance
        }
    }

    void ScriptComponent::onTriggerStay(Collider2D& other) {
        Component::onTriggerStay(other);
        if (m_scriptInstance) {
            // Call the onTriggerStay method on the script instance
        }
    }

    void ScriptComponent::onTriggerExit(Collider2D& other) {
        Component::onTriggerExit(other);
        if (m_scriptInstance) {
            // Call the onTriggerExit method on the script instance
        }
    }

    void ScriptComponent::onMouseEnter() {
        Component::onMouseEnter();
        if (m_scriptInstance) {
            // Call the onMouseEnter method on the script instance
        }
    }

    void ScriptComponent::onMouseOver() {
        Component::onMouseOver();
        if (m_scriptInstance) {
            // Call the onMouseOver method on the script instance
        }
    }

    void ScriptComponent::onMouseExit() {
        Component::onMouseExit();
        if (m_scriptInstance) {
            // Call the onMouseExit method on the script instance
        }
    }

    void ScriptComponent::onMouseDown(int button) {
        Component::onMouseDown(button);
        if (m_scriptInstance) {
            // Call the onMouseDown method on the script instance
        }
    }

    void ScriptComponent::onMouseUp(int button) {
        Component::onMouseUp(button);
        if (m_scriptInstance) {
            // Call the onMouseUp method on the script instance
        }
    }

    void ScriptComponent::onMouseClick(int button) {
        Component::onMouseClick(button);
        if (m_scriptInstance) {
            // Call the onMouseClick method on the script instance
        }
    }

    void ScriptComponent::setVariable(const std::string& name, const std::any& value) {
        // Find the property and update its value
        for (auto& prop : m_properties) {
            if (prop.name == name) {
                prop.value = value;
                break;
            }
        }
    }

    std::any ScriptComponent::getVariable(const std::string& name) const {
        for (const auto& prop : m_properties) {
            if (prop.name == name) {
                return prop.value;
            }
        }
        return std::any{};
    }

    bool ScriptComponent::hasVariable(const std::string& name) const {
        for (const auto& prop : m_properties) {
            if (prop.name == name) {
                return true;
            }
        }
        return false;
    }

    std::any ScriptComponent::callFunction(const std::string& name, const std::vector<std::any>& args) {
        // Call the specified function on the script instance
        return std::any{};
    }

    void ScriptComponent::reload() {
        unloadScript();
        loadScript();
    }

    void ScriptComponent::serialize(Serializer& serializer) const {
        Component::serialize(serializer);
        serializer.write("scriptPath", m_scriptPath);
        // Serialize script properties
        serializer.write("propertiesCount", static_cast<int>(m_properties.size()));
        for (size_t i = 0; i < m_properties.size(); ++i) {
            serializer.write("propertyName_" + std::to_string(i), m_properties[i].name);
            serializer.write("propertyType_" + std::to_string(i), m_properties[i].type);
            serializer.write("propertyExposed_" + std::to_string(i), m_properties[i].exposed);
            // Note: Actually serializing std::any values would require special handling
        }
    }

    void ScriptComponent::deserialize(Deserializer& deserializer) {
        Component::deserialize(deserializer);
        deserializer.read("scriptPath", m_scriptPath);
        loadScript(); // Reload the script after deserialization
        
        // Deserialize script properties
        int propertiesCount;
        deserializer.read("propertiesCount", propertiesCount);
        m_properties.resize(propertiesCount);
        for (int i = 0; i < propertiesCount; ++i) {
            deserializer.read("propertyName_" + std::to_string(i), m_properties[i].name);
            deserializer.read("propertyType_" + std::to_string(i), m_properties[i].type);
            deserializer.read("propertyExposed_" + std::to_string(i), m_properties[i].exposed);
        }
    }

    void ScriptComponent::onInspectorGUI() {
        // Draw script component properties in the inspector
    }

    void ScriptComponent::loadScript() {
        if (!m_scriptPath.empty()) {
            createScriptInstance();
            updateProperties();
            updateMethods();
        }
    }

    void ScriptComponent::unloadScript() {
        destroyScriptInstance();
        m_properties.clear();
        m_methods.clear();
    }

    void ScriptComponent::createScriptInstance() {
        // Create an instance of the script
        // This would depend on the scripting system implementation
        m_scriptInstance = nullptr; // Placeholder
    }

    void ScriptComponent::destroyScriptInstance() {
        // Destroy the script instance
        m_scriptInstance = nullptr; // Placeholder
    }

    void ScriptComponent::updateProperties() {
        // Refresh the list of properties from the script
        m_properties.clear();
        // This would involve querying the script for its public properties
    }

    void ScriptComponent::updateMethods() {
        // Refresh the list of methods from the script
        m_methods.clear();
        // This would involve querying the script for its public methods
    }

} // namespace CmakeProject1