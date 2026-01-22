#pragma once

#include "Types.h"
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>

namespace CmakeProject1 {

    class GameObject;
    class Camera;
    class Light2D;
    class AudioSource;

    class Scene {
    public:
        Scene(const std::string& name = "New Scene");
        ~Scene();

        // Basic properties
        std::string getName() const { return m_name; }
        void setName(const std::string& name) { m_name = name; }

        std::string getPath() const { return m_path; }
        void setPath(const std::string& path) { m_path = path; }

        bool isLoaded() const { return m_loaded; }
        bool isDirty() const { return m_dirty; }
        void setDirty(bool dirty = true) { m_dirty = dirty; }

        // Game object management
        std::shared_ptr<GameObject> createGameObject(const std::string& name = "GameObject");
        std::shared_ptr<GameObject> instantiate(std::shared_ptr<GameObject> prefab,
            const Vector2& position = Vector2(),
            float rotation = 0.0f);
        void destroy(std::shared_ptr<GameObject> gameObject);

        std::shared_ptr<GameObject> findGameObject(const std::string& name) const;
        std::vector<std::shared_ptr<GameObject>> findGameObjectsWithTag(const std::string& tag) const;
        std::shared_ptr<GameObject> findGameObjectWithTag(const std::string& tag) const;

        std::vector<std::shared_ptr<GameObject>> getAllGameObjects() const;
        std::vector<std::shared_ptr<GameObject>> getRootGameObjects() const;

        // Camera management
        std::shared_ptr<Camera> getMainCamera() const;
        void setMainCamera(std::shared_ptr<Camera> camera);
        std::vector<std::shared_ptr<Camera>> getAllCameras() const;

        // Lighting
        std::vector<std::shared_ptr<Light2D>> getAllLights() const;
        Color getAmbientLight() const { return m_ambientLight; }
        void setAmbientLight(const Color& color) { m_ambientLight = color; }

        // Physics
        void setGravity(const Vector2& gravity) { m_gravity = gravity; }
        Vector2 getGravity() const { return m_gravity; }

        // Audio
        std::vector<std::shared_ptr<AudioSource>> getAllAudioSources() const;

        // Lifecycle
        void load();
        void unload();
        void update(float deltaTime);
        void fixedUpdate(float fixedDeltaTime);
        void lateUpdate(float deltaTime);
        void render();

        // Serialization
        bool save(const std::string& path = "");
        bool loadFromFile(const std::string& path);

        // Scene settings
        struct Settings {
            Color backgroundColor = Color(0.1f, 0.1f, 0.1f, 1.0f);
            Vector2 gravity = Vector2(0, -9.81f);
            float physicsTimeStep = 1.0f / 60.0f;
            int maxPhysicsIterations = 10;
            bool enablePhysics = true;
            bool enableLighting = true;
            bool enableAudio = true;
            bool enableScripts = true;
            bool enableParticles = true;
        };

        Settings& getSettings() { return m_settings; }
        const Settings& getSettings() const { return m_settings; }

    private:
        std::string m_name;
        std::string m_path;
        bool m_loaded = false;
        bool m_dirty = false;

        Settings m_settings;

        std::vector<std::shared_ptr<GameObject>> m_gameObjects;
        std::vector<std::shared_ptr<GameObject>> m_rootGameObjects;

        // Optimized lookups
        std::unordered_map<std::string, std::shared_ptr<GameObject>> m_nameLookup;
        std::unordered_multimap<std::string, std::shared_ptr<GameObject>> m_tagLookup;

        // Special objects
        std::weak_ptr<Camera> m_mainCamera;

        // Environment
        Color m_ambientLight = Color(0.2f, 0.2f, 0.2f, 1.0f);
        Vector2 m_gravity = Vector2(0, -9.81f);

        // Physics world (to be implemented)
        void* m_physicsWorld = nullptr;

        // Destruction queue
        std::vector<std::shared_ptr<GameObject>> m_destructionQueue;

        void processDestructionQueue();

        friend class SceneManager;
        friend class Engine;
    };

} // namespace CmakeProject1