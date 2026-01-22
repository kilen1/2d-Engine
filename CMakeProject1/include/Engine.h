#pragma once

#include "Types.h"
#include "Config.h"
#include <SDL2/SDL.h>
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include <functional>
#include <filesystem>
#include <thread>
#include <atomic>
#include <mutex>
#include <queue>
#include <condition_variable>

// Forward declarations
struct SDL_Window;
struct SDL_GLContext;

namespace CmakeProject1 {

    // System forward declarations
    class Window;
    class Renderer;
    class InputManager;
    class AssetManager;
    class SceneManager;
    class ProjectManager;
    class ScriptEngine;
    class PhysicsEngine;
    class AudioEngine;
    class EventSystem;
    class TimeManager;
    class Editor;
    class Camera;
    class SpriteBatch;
    class ParticleSystem;
    class LightSystem;
    class UIManager;
    class AnimationSystem;
    class NetworkManager;
    class GraphicsTablet;
    class ToolManager;
    class ContentBrowser;
    class Console;
    class Profiler;
    class Hierarchy;
    class Inspector;
    class SceneViewer;
    class GameView;
    class DebugDraw;

    class Engine {
    public:
        static Engine& getInstance();
        Engine(const Engine&) = delete;
        Engine& operator=(const Engine&) = delete;

        // Core
        bool initialize(int argc = 0, char* argv[] = nullptr);
        void run();
        void shutdown();

        // Project management
        bool createProject(const std::string& name, const std::string& path);
        bool openProject(const std::string& projectPath);
        bool saveProject();
        void closeProject();
        bool isProjectLoaded() const;
        std::string getProjectPath() const;
        std::string getProjectName() const;

        // Scene management
        std::shared_ptr<Scene> createScene(const std::string& name);
        bool loadScene(const std::string& scenePath);
        bool saveScene(const std::string& scenePath = "");
        std::shared_ptr<Scene> getCurrentScene() const;

        // Asset management
        std::shared_ptr<Texture> loadTexture(const std::string& path);
        std::shared_ptr<Sprite> createSprite(const std::string& name, std::shared_ptr<Texture> texture, const Rect& rect);
        std::shared_ptr<Material> createMaterial(const std::string& name);
        std::shared_ptr<Shader> loadShader(const std::string& vertexPath, const std::string& fragmentPath);
        std::shared_ptr<AudioClip> loadAudio(const std::string& path);
        std::shared_ptr<Font> loadFont(const std::string& path, int size);
        std::shared_ptr<AnimationClip> createAnimation(const std::string& name);

        // Game object management
        std::shared_ptr<GameObject> createGameObject(const std::string& name = "GameObject");
        std::shared_ptr<GameObject> instantiate(std::shared_ptr<GameObject> prefab);
        void destroy(std::shared_ptr<GameObject> gameObject);

        // Editor
        void toggleEditor();
        bool isEditorEnabled() const { return m_editorEnabled; }
        void setEditorEnabled(bool enabled);

        // Play mode
        void togglePlayMode();
        bool isPlaying() const { return m_playing; }
        void pause();
        void resume();
        void stop();

        // Graphics tablet
        void setTabletEnabled(bool enabled);
        bool isTabletEnabled() const;
        Vector2 getTabletPosition() const;
        float getTabletPressure() const;

        // Drawing tools
        void setBrushColor(const Color& color);
        void setBrushSize(float size);
        void setBrushTexture(std::shared_ptr<Texture> texture);
        void clearDrawing();
        void saveDrawing(const std::string& path);

        // System getters
        Window* getWindow() const { return m_window.get(); }
        Renderer* getRenderer() const { return m_renderer.get(); }
        InputManager* getInput() const { return m_inputManager.get(); }
        AssetManager* getAssetManager() const { return m_assetManager.get(); }
        SceneManager* getSceneManager() const { return m_sceneManager.get(); }
        Editor* getEditor() const { return m_editor.get(); }
        ProjectManager* getProjectManager() const { return m_projectManager.get(); }
        ScriptEngine* getScriptEngine() const { return m_scriptEngine.get(); }
        PhysicsEngine* getPhysicsEngine() const { return m_physicsEngine.get(); }
        AudioEngine* getAudioEngine() const { return m_audioEngine.get(); }
        GraphicsTablet* getGraphicsTablet() const { return m_graphicsTablet.get(); }

        // Config
        EngineConfig& getConfig() { return m_config; }
        const EngineConfig& getConfig() const { return m_config; }
        void loadConfig(const std::string& path = "config.json");
        void saveConfig(const std::string& path = "config.json");

        // Events
        void subscribe(const std::string& eventType, std::function<void(const Event&)> callback);
        void unsubscribe(const std::string& eventType, size_t id);
        void emit(const std::string& eventType, const Event& event);

        // Multithreading
        void submitTask(std::function<void()> task);
        void waitForTasks();

        // Debug
        void drawDebugLine(const Vector2& start, const Vector2& end, const Color& color);
        void drawDebugCircle(const Vector2& center, float radius, const Color& color);
        void drawDebugRect(const Rect& rect, const Color& color);

        // Statistics
        struct Statistics {
            float fps;
            float frameTime;
            int drawCalls;
            int triangles;
            size_t memoryUsage;
            int gameObjects;
            int components;
            int scripts;
        };

        Statistics getStatistics() const;

        // Version
        static const char* getVersion();
        static const char* getBuildDate();
        static const char* getPlatform();

    private:
        Engine();
        ~Engine();

        // Core systems
        std::unique_ptr<Window> m_window;
        std::unique_ptr<Renderer> m_renderer;
        std::unique_ptr<InputManager> m_inputManager;
        std::unique_ptr<AssetManager> m_assetManager;
        std::unique_ptr<SceneManager> m_sceneManager;
        std::unique_ptr<ProjectManager> m_projectManager;
        std::unique_ptr<ScriptEngine> m_scriptEngine;
        std::unique_ptr<PhysicsEngine> m_physicsEngine;
        std::unique_ptr<AudioEngine> m_audioEngine;
        std::unique_ptr<EventSystem> m_eventSystem;
        std::unique_ptr<TimeManager> m_timeManager;
        std::unique_ptr<Editor> m_editor;
        std::unique_ptr<NetworkManager> m_networkManager;
        std::unique_ptr<GraphicsTablet> m_graphicsTablet;
        std::unique_ptr<ToolManager> m_toolManager;

        // Graphics systems
        std::unique_ptr<Camera> m_mainCamera;
        std::unique_ptr<SpriteBatch> m_spriteBatch;
        std::unique_ptr<ParticleSystem> m_particleSystem;
        std::unique_ptr<LightSystem> m_lightSystem;
        std::unique_ptr<UIManager> m_uiManager;
        std::unique_ptr<AnimationSystem> m_animationSystem;

        // Tools
        std::unique_ptr<ContentBrowser> m_contentBrowser;
        std::unique_ptr<Console> m_console;
        std::unique_ptr<Profiler> m_profiler;
        std::unique_ptr<Hierarchy> m_hierarchy;
        std::unique_ptr<Inspector> m_inspector;
        std::unique_ptr<SceneViewer> m_sceneViewer;
        std::unique_ptr<GameView> m_gameView;
        std::unique_ptr<DebugDraw> m_debugDraw;

        // Thread pool
        std::vector<std::thread> m_workerThreads;
        std::queue<std::function<void()>> m_taskQueue;
        std::mutex m_taskMutex;
        std::condition_variable m_taskCondition;
        std::atomic<bool> m_workersRunning{ false };

        // State
        std::atomic<bool> m_running{ false };
        std::atomic<bool> m_editorEnabled{ true };
        std::atomic<bool> m_playing{ false };
        std::atomic<bool> m_paused{ false };

        // Config
        EngineConfig m_config;

        // Statistics
        Statistics m_stats;

        // Initialization
        bool initializeSDL();
        bool initializeOpenGL();
        bool initializeImGui();
        bool initializeSystems();
        bool initializeTools();
        bool initializeScripting();

        // Main loop
        void mainLoop();
        void processEvents();
        void update(float deltaTime);
        void render();
        void renderEditor();

        // Event handlers
        void handleWindowEvent(const SDL_Event& event);
        void handleResize(int width, int height);
        void handleDropFile(const std::string& filePath);
        void handleHotkey(SDL_Keycode key);

        // Project management
        void createDefaultProjectStructure(const std::filesystem::path& projectPath);
        std::string generateProjectFile(const std::string& name) const;

        // Utility
        void updateStatistics();
        void backupProject();
        void autoSave();
        void checkForUpdates();

        // Friend classes
        friend class Editor;
        friend class ScriptEngine;
    };

} // namespace CmakeProject1