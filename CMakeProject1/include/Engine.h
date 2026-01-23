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

#include <DebugDraw.h>
#include <EventSystem.h>
#include <PhysicsEngine.h>
#include <AudioEngine.h>
#include <ScriptEngine.h>
#include <Script.h>
#include <Scene.h>
#include <SceneManager.h>
#include <share.h>
#include <AssetManager.h>
#include <Renderer.h>
#include <InputManager.h>
#include <Window.h>

// Forward declarations
struct SDL_Window;


namespace CmakeProject1 {

    
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
    
    class DebugDraw;

    class Engine {
    public:
        static Engine& getInstance();
        Engine(const Engine&) = delete;
        Engine& operator=(const Engine&) = delete;

        
        bool initialize(int argc = 0, char* argv[] = nullptr);
        void run();
        void shutdown();

       
        bool createProject(const std::string& name, const std::string& path);
        bool openProject(const std::string& projectPath);
        bool saveProject();
        void closeProject();
        bool isProjectLoaded() const;
        std::string getProjectPath() const;
        std::string getProjectName() const;
        
       
        std::shared_ptr<Scene> createScene(const std::string& name);
        bool loadScene(const std::string& scenePath);
        bool saveScene(const std::string& scenePath = "");
        std::shared_ptr<Scene> getCurrentScene() const;

        
        std::shared_ptr<Texture> loadTexture(const std::string& path);
        std::shared_ptr<Sprite> createSprite(const std::string& name, std::shared_ptr<Texture> texture, const Rect& rect);
        std::shared_ptr<Material> createMaterial(const std::string& name);
        std::shared_ptr<Shader> loadShader(const std::string& vertexPath, const std::string& fragmentPath);
        std::shared_ptr<AudioClip> loadAudio(const std::string& path);
        std::shared_ptr<Font> loadFont(const std::string& path, int size);
        std::shared_ptr<AnimationClip> createAnimation(const std::string& name);

      
        std::shared_ptr<GameObject> createGameObject(const std::string& name = "GameObject");
        std::shared_ptr<GameObject> instantiate(std::shared_ptr<GameObject> prefab);
        void destroy(std::shared_ptr<GameObject> gameObject);

        
        void toggleEditor();
        bool isEditorEnabled() const { return m_editorEnabled; }
        void setEditorEnabled(bool enabled);

        
        void togglePlayMode();
        bool isPlaying() const { return m_playing; }
        void pause();
        void resume();
        void stop();

        
        void setTabletEnabled(bool enabled);
        bool isTabletEnabled() const;
        Vector2 getTabletPosition() const;
        float getTabletPressure() const;

       
        void setBrushColor(const Color& color);
        void setBrushSize(float size);
        void setBrushTexture(std::shared_ptr<Texture> texture);
        void clearDrawing();
        void saveDrawing(const std::string& path);

       
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
        

        EngineConfig& getConfig() { return m_config; }
        const EngineConfig& getConfig() const { return m_config; }
        void loadConfig(const std::string& path = "config.json");
        void saveConfig(const std::string& path = "config.json");

       
        void subscribe(const std::string& eventType, std::function<void(const Event&)> callback);
        void unsubscribe(const std::string& eventType, size_t id);
        void emit(const std::string& eventType, const Event& event);

      
        void submitTask(std::function<void()> task);
        void waitForTasks();

      
        void drawDebugLine(const Vector2& start, const Vector2& end, const Color& color);
        void drawDebugCircle(const Vector2& center, float radius, const Color& color);
        void drawDebugRect(const Rect& rect, const Color& color);

      
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

   
        static const char* getVersion();
        static const char* getBuildDate();
        static const char* getPlatform();

    private:
        Engine();
        ~Engine();

      
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
       
        std::unique_ptr<Editor> m_editor;
        
        
        

    
        std::unique_ptr<Camera> m_mainCamera;
        
        
        
        
        

      
        
        
        
        
        
        
        
        std::unique_ptr<DebugDraw> m_debugDraw;

        std::vector<std::thread> m_workerThreads;
        std::queue<std::function<void()>> m_taskQueue;
        std::mutex m_taskMutex;
        std::condition_variable m_taskCondition;
        std::atomic<bool> m_workersRunning{ false };

 
        std::atomic<bool> m_running{ false };
        std::atomic<bool> m_editorEnabled{ true };
        std::atomic<bool> m_playing{ false };
        std::atomic<bool> m_paused{ false };

       
        EngineConfig m_config;

      
        Statistics m_stats;

      
        bool initializeSDL();
        bool initializeOpenGL();
        bool initializeImGui();
        bool initializeSystems();
        bool initializeTools();
        bool initializeScripting();

      
        void mainLoop();
        void processEvents();
        void update(float deltaTime);
        void render();
        void renderEditor();

        void handleWindowEvent(const SDL_Event& event);
        void handleResize(int width, int height);
        void handleDropFile(const std::string& filePath);
        void handleHotkey(SDL_Keycode key);

        void createDefaultProjectStructure(const std::filesystem::path& projectPath, const std::string& projectName) const;
        std::string generateProjectFile(const std::string& name);

       
        void updateStatistics();
        void backupProject();
        void autoSave();
        void checkForUpdates();

        friend class Editor;
        friend class ScriptEngine;
    };

} 