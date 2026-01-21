#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include <functional>
#include "../Core/Types.h"

namespace CmakeProject1 {

    class GameObject;
    class Component;

    struct PrefabData {
        std::string name;
        std::vector<std::string> componentTypes;
        std::unordered_map<std::string, std::string> componentData; // Serialized component data
        std::vector<PrefabData> children;
    };

    class PrefabSystem {
    public:
        PrefabSystem();
        ~PrefabSystem();

        // Prefab creation and loading
        bool LoadPrefab(const std::string& prefabPath);
        bool SavePrefab(const std::string& prefabPath, GameObject* gameObject);
        bool SavePrefabAsTemplate(const std::string& prefabName, GameObject* gameObject);

        // Prefab instantiation
        GameObject* Instantiate(const std::string& prefabName);
        GameObject* Instantiate(const std::string& prefabName, const Vector2& position);
        GameObject* Instantiate(const std::string& prefabName, const Vector2& position, float rotation);
        GameObject* Instantiate(const std::string& prefabName, const Vector3& position);
        GameObject* Instantiate(const std::string& prefabName, const Vector3& position, const Quaternion& rotation);

        // Prefab management
        bool RegisterPrefab(const std::string& prefabName, const PrefabData& prefabData);
        bool UnregisterPrefab(const std::string& prefabName);
        bool HasPrefab(const std::string& prefabName) const;
        std::vector<std::string> GetAllPrefabNames() const;

        // Prefab modification
        bool UpdatePrefab(const std::string& prefabName, GameObject* gameObject);
        bool ReplacePrefab(const std::string& prefabName, GameObject* gameObject);
        bool DetachFromPrefab(GameObject* gameObject);

        // Prefab templates
        std::string CreatePrefabTemplate(GameObject* gameObject);
        bool ApplyPrefabTemplate(GameObject* gameObject, const std::string& templateData);

        // Batch operations
        void InstantiateMultiple(const std::string& prefabName, const std::vector<Vector2>& positions);
        std::vector<GameObject*> InstantiateWithCallback(const std::string& prefabName, 
                                                         const std::vector<Vector2>& positions,
                                                         std::function<void(GameObject*)> callback);

        // Prefab utilities
        std::string GetPrefabPath(const std::string& prefabName) const;
        PrefabData GetPrefabData(const std::string& prefabName) const;
        bool IsPrefabInstance(GameObject* gameObject) const;
        std::string GetPrefabName(GameObject* gameObject) const;

        // Prefab hierarchy
        GameObject* InstantiateHierarchy(const std::string& prefabName, GameObject* parent = nullptr);
        void SetParentOnInstantiate(bool setParent);

        // Cleanup
        void ClearAllPrefabs();
        void UnloadUnusedPrefabs();

    private:
        std::unordered_map<std::string, PrefabData> prefabs;
        std::unordered_map<GameObject*, std::string> prefabInstances; // Maps instances to their prefab names
        std::string prefabDirectory = "Assets/Prefabs/";
        bool autoSetParent = true;
    };

} // namespace CmakeProject1