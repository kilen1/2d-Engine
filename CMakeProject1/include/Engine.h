#pragma once

#include "Core/System.h"
#include "Core/Window.h"
#include "Core/Renderer.h"
#include "Core/InputManager.h"
#include "Core/AssetManager.h"
#include "Core/TimeManager.h"
#include "Core/EventSystem.h"
#include "Core/PhysicsEngine.h"
#include "Core/AudioEngine.h"
#include "Core/ScriptEngine.h"
#include "Scene/SceneManager.h"
#include "Editor/Editor.h"
#include "Project/ProjectManager.h"
#include "Graphics/Camera.h"
#include "Graphics/SpriteBatch.h"
#include "Graphics/ParticleSystem.h"
#include "Graphics/LightSystem.h"
#include "UI/Canvas.h"
#include "UI/UIManager.h"
#include "Animation/AnimationSystem.h"
#include "AI/BehaviorTree.h"
#include "AI/Pathfinding.h"
#include "Tools/ContentBrowser.h"
#include "Tools/Console.h"
#include "Tools/Profiler.h"
#include "Tools/DebugDraw.h"
#include "Tools/SceneViewer.h"
#include "Tools/GameView.h"
#include "Tools/Inspector.h"
#include "Tools/Hierarchy.h"
#include "Tools/ToolManager.h"
#include "GraphicsTablet.h"
#include "Scripting/ScriptManager.h"
#include "Scripting/HotReload.h"
#include "Networking/NetworkManager.h"
#include "Serialization/Serializer.h"
#include "Serialization/PrefabSystem.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <GL/glew.h>
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include <functional>
#include <filesystem>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

namespace fs = std::filesystem;

namespace CmakeProject1 {

    class Engine {
    public:
        static Engine& getInstance();
        Engine(const Engine&) = delete;
        Engine& operator=(const Engine&) = delete;

        // Инициализация и запуск
        bool initialize(int argc = 0, char* argv[] = nullptr);
        void run();
        void shutdown();
        
        // Системы
        void preUpdate();
        void update();
        void postUpdate();
        void render();
        
        // Проекты
        bool createProject(const std::string& name, const fs::path& location);
        bool loadProject(const fs::path& projectPath);
        bool saveProject();
        void closeProject();
        
        // Сцены
        void loadScene(const std::string& scenePath);
        void saveCurrentScene();
        void createNewScene(const std::string& name);
        
        // Системные геттеры
        Window* getWindow() const { return window.get(); }
        Renderer* getRenderer() const { return renderer.get(); }
        InputManager* getInput() const { return inputManager.get(); }
        AssetManager* getAssets() const { return assetManager.get(); }
        SceneManager* getSceneManager() const { return sceneManager.get(); }
        Editor* getEditor() const { return editor.get(); }
        ProjectManager* getProjectManager() const { return projectManager.get(); }
        ScriptEngine* getScriptEngine() const { return scriptEngine.get(); }
        PhysicsEngine* getPhysics() const { return physicsEngine.get(); }
        
        // Состояние
        bool isRunning() const { return running; }
        bool isEditorMode() const { return editorMode; }
        void setEditorMode(bool mode);
        void togglePlayMode();
        
        // Конфигурация
        void loadConfig(const std::string& configPath);
        void saveConfig(const std::string& configPath);
        
        // Утилиты
        void takeScreenshot(const std::string& path = "");
        void recordVideo(const std::string& path, int fps = 60);
        void stopRecording();
        
        // Мультиплеер
        void startServer(int port = 7777);
        void connectToServer(const std::string& address, int port = 7777);
        void disconnect();
        
        // Скрипты
        void reloadScripts();
        void compileScript(const std::string& scriptPath);
        
        // Редактор спрайтов
        void startSpriteEditor();
        void importSpritesheet(const std::string& path, int frameWidth, int frameHeight);
        
        // Анимации
        void createAnimation(const std::string& name, const std::vector<std::string>& frames);
        void playAnimation(const std::string& gameObject, const std::string& animation);
        
        // Частицы
        void emitParticles(const std::string& systemName, const Vector2& position, int count = 100);
        void stopParticles(const std::string& systemName);
        
        // Освещение
        void addLight(const Vector2& position, float radius, const Color& color);
        void removeLight(const std::string& lightId);
        
        // Звук
        void playSound(const std::string& soundPath, bool loop = false);
        void stopAllSounds();
        void setMasterVolume(float volume);
        
        // UI
        void createUIElement(const std::string& type, const std::string& name, const Vector2& position);
        void showMessageBox(const std::string& title, const std::string& message);
        
        // Дебаг
        void drawDebugLine(const Vector2& start, const Vector2& end, const Color& color);
        void drawDebugCircle(const Vector2& center, float radius, const Color& color);
        void drawDebugRect(const SDL_Rect& rect, const Color& color);
        
        // ГП планшет
        void enableTabletSupport(bool enable);
        bool isTabletConnected() const;
        Vector2 getTabletPosition() const;
        float getTabletPressure() const;
        
        // Горячие клавиши
        void registerHotkey(const std::string& name, SDL_Keycode key, std::function<void()> action);
        void unregisterHotkey(const std::string& name);
        
        // События
        void subscribe(const std::string& eventType, std::function<void(const Event&)> callback);
        void unsubscribe(const std::string& eventType, size_t id);
        void emit(const std::string& eventType, const EventData& data = {});
        
        // Сохранение/загрузка
        void saveGame(const std::string& slot);
        void loadGame(const std::string& slot);
        
        // Консольные команды
        void executeConsoleCommand(const std::string& command);
        void registerCommand(const std::string& command, std::function<void(const std::vector<std::string>&)> handler);
        
        // Профилирование
        void startProfiling(const std::string& sessionName);
        void stopProfiling();
        void saveProfileData(const std::string& path);
        
        // Мультипоточность
        void submitTask(std::function<void()> task);
        void waitForTasks();
        
        // Плагины
        void loadPlugin(const std::string& pluginPath);
        void unloadPlugin(const std::string& pluginName);
        std::vector<std::string> getLoadedPlugins() const;
        
        // Расширенные возможности
        void enableVR(bool enable);
        void enableAR(bool enable);
        void enableRayTracing(bool enable);
        
        // Настройки качества
        void setGraphicsQuality(QualityLevel level);
        void enableAntiAliasing(bool enable);
        void enableVSync(bool enable);
        void setTargetFPS(int fps);
        
        // Локализация
        void loadLocalization(const std::string& language);
        std::string translate(const std::string& key);
        
        // Доступные сцены
        std::vector<std::string> getAvailableScenes() const;
        
        // Статистика
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
        
        // Версия движка
        static const char* getVersion();
        static const char* getBuildDate();
        static const char* getPlatform();
        
    private:
        Engine();
        ~Engine();
        
        // Основные системы
        std::unique_ptr<Window> window;
        std::unique_ptr<Renderer> renderer;
        std::unique_ptr<InputManager> inputManager;
        std::unique_ptr<AssetManager> assetManager;
        std::unique_ptr<TimeManager> timeManager;
        std::unique_ptr<EventSystem> eventSystem;
        std::unique_ptr<PhysicsEngine> physicsEngine;
        std::unique_ptr<AudioEngine> audioEngine;
        std::unique_ptr<ScriptEngine> scriptEngine;
        std::unique_ptr<SceneManager> sceneManager;
        std::unique_ptr<Editor> editor;
        std::unique_ptr<ProjectManager> projectManager;
        std::unique_ptr<NetworkManager> networkManager;
        std::unique_ptr<GraphicsTablet> graphicsTablet;
        std::unique_ptr<ToolManager> toolManager;
        std::unique_ptr<ScriptManager> scriptManager;
        std::unique_ptr<HotReload> hotReload;
        
        // Графические системы
        std::unique_ptr<SpriteBatch> spriteBatch;
        std::unique_ptr<ParticleSystem> particleSystem;
        std::unique_ptr<LightSystem> lightSystem;
        std::unique_ptr<Camera> mainCamera;
        
        // UI системы
        std::unique_ptr<UIManager> uiManager;
        std::unique_ptr<Canvas> mainCanvas;
        
        // Анимации
        std::unique_ptr<AnimationSystem> animationSystem;
        
        // AI
        std::unique_ptr<BehaviorTreeManager> behaviorTreeManager;
        std::unique_ptr<Pathfinding> pathfinding;
        
        // Инструменты
        std::unique_ptr<ContentBrowser> contentBrowser;
        std::unique_ptr<Console> console;
        std::unique_ptr<Profiler> profiler;
        std::unique_ptr<DebugDraw> debugDraw;
        std::unique_ptr<SceneViewer> sceneViewer;
        std::unique_ptr<GameView> gameView;
        std::unique_ptr<Inspector> inspector;
        std::unique_ptr<Hierarchy> hierarchy;
        
        // Сериализация
        std::unique_ptr<Serializer> serializer;
        std::unique_ptr<PrefabSystem> prefabSystem;
        
        // Пул потоков
        std::vector<std::thread> workerThreads;
        std::queue<std::function<void()>> taskQueue;
        std::mutex taskMutex;
        std::condition_variable taskCondition;
        std::atomic<bool> workersRunning{false};
        
        // Состояние
        std::atomic<bool> running{false};
        std::atomic<bool> editorMode{true};
        std::atomic<bool> playMode{false};
        std::atomic<bool> paused{false};
        std::atomic<bool> recording{false};
        
        // Конфигурация
        Config config;
        
        // Статистика
        Statistics stats;
        std::chrono::high_resolution_clock::time_point lastStatUpdate;
        
        // Инициализация систем
        bool initializeSDL();
        bool initializeOpenGL();
        bool initializeImGui();
        bool initializeSystems();
        bool initializeTools();
        bool initializeScripting();
        bool initializeNetworking();
        
        // Основной цикл
        void mainLoop();
        void processEvents();
        void updateSystems();
        void renderSystems();
        void renderEditor();
        
        // Обработка оконных событий
        void handleWindowEvent(const SDL_Event& event);
        void handleResize(int width, int height);
        void handleDropFile(const std::string& filePath);
        
        // Управление сценами
        void unloadCurrentScene();
        void reloadCurrentScene();
        
        // Управление памятью
        void garbageCollect();
        void defragmentMemory();
        
        // Утилиты
        void updateStatistics();
        void checkForUpdates();
        void backupProject();
        
        // Обработчики ошибок
        void handleError(const std::string& error);
        void logError(const std::string& error);
        
        // Шейдеры
        std::unordered_map<std::string, GLuint> shaders;
        bool loadShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);
        void useShader(const std::string& name);
        
        // Ресурсы
        std::unordered_map<std::string, std::shared_ptr<Resource>> resources;
        
        // Регистрация компонентов
        void registerBuiltinComponents();
        
        // Команды консоли
        std::unordered_map<std::string, std::function<void(const std::vector<std::string>&)>> consoleCommands;
        
        // Горячие клавиши
        std::unordered_map<std::string, std::pair<SDL_Keycode, std::function<void()>>> hotkeys;
        
        // События
        std::unordered_map<std::string, std::vector<std::pair<size_t, std::function<void(const Event&)>>>> eventListeners;
        size_t nextListenerId = 0;
        
        // Версия
        static const char* VERSION;
        static const char* BUILD_DATE;
        
        // Дружественные классы
        friend class Editor;
        friend class ScriptEngine;
        friend class ResourceManager;
    };
}