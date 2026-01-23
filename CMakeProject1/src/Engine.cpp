#include "Engine.h"
#include "Scene.h"
#include <Editor.h>
#include "GameObject.h"
#include "Camera.h"
#include "Config.h"
#include <ProjectManager.h>
#include <SpriteRenderer.h>

// Include necessary SDL2 headers
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>
#include "Window.h"

// Include OpenGL if needed
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <iostream>
#include <chrono>
#include <thread>


namespace CmakeProject1 {

    Engine& Engine::getInstance() {
        static Engine instance;
        return instance;
    }

    Engine::Engine() {
        // Initialize default config
        m_config.resetToDefaults();
    }

    Engine::~Engine() {
        shutdown();
    }

    bool Engine::initialize(int argc, char* argv[]) {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) < 0) {
            std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
            return false;
        }

        
        if (!initializeSystems()) {
            return false;
        }

        
        if (!initializeImGui()) {
            std::cerr << "ImGui initialization failed!" << std::endl;
            return false;
        }

       
        if (!initializeTools() || !initializeScripting()) {
            return false;
        }

        m_running = true;
        return true;
    }

    void Engine::run() {
        if (!m_running) {
            std::cout << "Engine not initialized. Call initialize() first." << std::endl;
            return;
        }

        mainLoop();
    }

    void Engine::shutdown() {
        m_running = false;

        // Wait for tasks to complete
        waitForTasks();

        // Shutdown worker threads
        m_workersRunning = false;
        m_taskCondition.notify_all();
        for (auto& thread : m_workerThreads) {
            if (thread.joinable()) {
                thread.join();
            }
        }

        // Shutdown systems
        m_renderer.reset();
        m_window.reset();
        m_inputManager.reset();
        m_assetManager.reset();
        m_sceneManager.reset();
        m_projectManager.reset();
        m_scriptEngine.reset();
        m_physicsEngine.reset();
        m_audioEngine.reset();
        m_eventSystem.reset();
        
        m_editor.reset();
        
       
        

        // Shutdown graphics systems
        m_mainCamera.reset();
        
        
        
        
        

        // Shutdown tools
        
        
        
       
       
        m_debugDraw.reset();

        // Quit SDL
        IMG_Quit();
        Mix_Quit();
        TTF_Quit();
        SDL_Quit();
    }

    inline bool Engine::createProject(const std::string& name, const std::string& path) {
        if (m_projectManager) {
            return m_projectManager->createProject(name, path);
        }
        return false;
    }

    void CmakeProject1::Engine::handleResize(int width, int height) {
       
    }


    void Engine::render() {
      
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        
        if (m_editorEnabled && m_editor) {
            renderEditor();
        }

        
        if (m_window) {
            m_window->swapBuffers();
        }
    }



    bool Engine::openProject(const std::string& projectPath) {
        if (m_projectManager) {
            return m_projectManager->loadProject(projectPath);
        }
        return false;
    }

    bool Engine::saveProject() {
        if (m_projectManager) {
            return m_projectManager->saveProject();
        }
        return false;
    }

    void Engine::closeProject() {
        if (m_projectManager) {
            m_projectManager->closeProject();
        }
    }

    bool Engine::isProjectLoaded() const {
        if (m_projectManager) {
            return m_projectManager->hasProject();
        }
        return false;
    }

    std::string Engine::getProjectPath() const {
        if (m_projectManager) {
            return m_projectManager->getProjectPath();
        }
        return "";
    }

    std::string Engine::getProjectName() const {
        if (m_projectManager) {
            return m_projectManager->getProjectName();
        }
        return "";
    }

    std::shared_ptr<Scene> Engine::createScene(const std::string& name) {
        if (m_sceneManager) {
            // Return a new scene - this would normally be handled by the scene manager
            return std::make_shared<Scene>(name);
        }
        return nullptr;
    }

    bool Engine::loadScene(const std::string& scenePath) {
        if (m_sceneManager) {
            // Load scene via scene manager
            return true; // Placeholder
        }
        return false;
    }






    

    bool Engine::initializeImGui() {
       
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; 
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;    
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   

        
        ImGui::StyleColorsDark(); 

       
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            ImGuiStyle& style = ImGui::GetStyle();
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        // Получаем указатели на SDL_Window и OpenGL контекст
        SDL_Window* window = m_window ? m_window->getSDLWindow() : nullptr;
        void* glContext = m_window ? m_window->getGLContext() : nullptr;

        if (!window || !glContext) {
            std::cerr << "Failed to get SDL window or OpenGL context for ImGui!" << std::endl;
            return false;
        }

        // Инициализируем бэкенды
        if (!ImGui_ImplSDL2_InitForOpenGL(window, glContext)) {
            std::cerr << "Failed to initialize ImGui SDL2 backend!" << std::endl;
            return false;
        }

        // Укажите версию OpenGL шейдеров (важно!)
        // Для современных контекстов (core profile) используйте "#version 130" или выше
        const char* glsl_version = "#version 130";
        if (!ImGui_ImplOpenGL3_Init(glsl_version)) {
            std::cerr << "Failed to initialize ImGui OpenGL3 backend!" << std::endl;
            return false;
        }

        return true;
    }










    bool Engine::saveScene(const std::string& scenePath) {
        if (m_sceneManager) {
            // Save scene via scene manager
            return true; // Placeholder
        }
        return false;
    }

    std::shared_ptr<Scene> Engine::getCurrentScene() const {
        // Return current scene - would be managed by scene manager
        return nullptr; // Placeholder
    }

    std::shared_ptr<GameObject> Engine::createGameObject(const std::string& name) {
        if (m_sceneManager && getCurrentScene()) {
            return getCurrentScene()->createGameObject(name);
        }
        // If no scene is loaded, create a temporary game object
        return std::make_shared<GameObject>(name);
    }

    void Engine::toggleEditor() {
        m_editorEnabled = !m_editorEnabled;
    }

    void Engine::setEditorEnabled(bool enabled) {
        m_editorEnabled = enabled;
    }

    void Engine::togglePlayMode() {
        m_playing = !m_playing;
    }

    void Engine::pause() {
        m_paused = true;
    }

    void Engine::resume() {
        m_paused = false;
    }

    void Engine::stop() {
        m_playing = false;
        m_paused = false;
    }

    void Engine::setTabletEnabled(bool enabled) {
        m_config.tabletEnabled = enabled;
    }

    bool Engine::isTabletEnabled() const {
        return m_config.tabletEnabled;
    }

    Vector2 Engine::getTabletPosition() const {
        // Placeholder implementation
        return Vector2();
    }

    float Engine::getTabletPressure() const {
        // Placeholder implementation
        return 0.0f;
    }

    void Engine::setBrushColor(const Color& color) {
        // Set brush color in drawing system
    }

    void Engine::setBrushSize(float size) {
        // Set brush size in drawing system
    }

    void Engine::clearDrawing() {
        // Clear drawing canvas
    }

    void Engine::saveDrawing(const std::string& path) {
        // Save drawing to file
    }

    void Engine::submitTask(std::function<void()> task) {
        if (!m_workersRunning) {
            // Start worker threads if not already running
            m_workersRunning = true;
            int numThreads = std::thread::hardware_concurrency();
            if (numThreads == 0) numThreads = 4; // Default to 4 threads if hardware concurrency fails
            
            for (int i = 0; i < numThreads; ++i) {
                m_workerThreads.emplace_back([this]() {
                    while (m_workersRunning) {
                        std::function<void()> task;
                        
                        {
                            std::unique_lock<std::mutex> lock(m_taskMutex);
                            m_taskCondition.wait(lock, [this] { return !m_taskQueue.empty() || !m_workersRunning; });
                            
                            if (!m_workersRunning && m_taskQueue.empty()) {
                                return;
                            }
                            
                            task = std::move(m_taskQueue.front());
                            m_taskQueue.pop();
                        }
                        
                        task();
                    }
                });
            }
        }
        
        {
            std::lock_guard<std::mutex> lock(m_taskMutex);
            m_taskQueue.push(task);
        }
        m_taskCondition.notify_one();
    }

    void Engine::waitForTasks() {
        std::unique_lock<std::mutex> lock(m_taskMutex);
        m_taskCondition.wait(lock, [this] { return m_taskQueue.empty(); });
    }

    Engine::Statistics Engine::getStatistics() const {
        return m_stats;
    }

    const char* Engine::getVersion() {
        return "1.0.0";
    }

    const char* Engine::getBuildDate() {
        return __DATE__;
    }

    const char* Engine::getPlatform() {
#ifdef _WIN32
        return "Windows";
#elif __APPLE__
        return "macOS";
#elif __linux__
        return "Linux";
#else
        return "Unknown";
#endif
    }

    bool Engine::initializeSDL() {
        // SDL is already initialized in the constructor
        return true;
    }

    bool Engine::initializeOpenGL() {
        // Initialize OpenGL context
        return true;
    }

    bool Engine::initializeSystems() {
        m_window = std::make_unique<Window>();

        
        if (!m_window->initialize("My Engine", 1280, 720)) {
            std::cerr << "Failed to initialize window!" << std::endl;
            return false;
        }

        m_renderer = std::make_unique<Renderer>();
        m_inputManager = std::make_unique<InputManager>();
        m_assetManager = std::make_unique<AssetManager>();
        m_sceneManager = std::make_unique<SceneManager>();
        m_projectManager = std::make_unique<ProjectManager>();
        m_scriptEngine = std::make_unique<ScriptEngine>();
        m_physicsEngine = std::make_unique<PhysicsEngine>();
        m_audioEngine = std::make_unique<AudioEngine>();
        m_eventSystem = std::make_unique<EventSystem>();

        return true;
    }

    bool Engine::initializeTools() {
        // Initialize editor and tools
        m_editor = std::make_unique<class Editor>();
        
       
        
        // Initialize UI tools
       
        
        
       
       
        
        
        m_debugDraw = std::make_unique<class DebugDraw>();
        
        return true;
    }

    bool Engine::initializeScripting() {
        // Initialize scripting system
        return true;
    }

    void Engine::mainLoop() {
        auto lastFrame = std::chrono::high_resolution_clock::now();
        
        while (m_running) {
            auto currentFrame = std::chrono::high_resolution_clock::now();
            float deltaTime = std::chrono::duration<float>(currentFrame - lastFrame).count();
            lastFrame = currentFrame;
            
            processEvents();
            
            if (!m_paused) {
                update(deltaTime);
            }
            
            render();
            
            // Limit FPS if VSync is off
            if (!m_config.vsync) {
                float frameTime = 1.0f / m_config.targetFPS;
                if (deltaTime < frameTime) {
                    std::this_thread::sleep_for(std::chrono::duration<float>(frameTime - deltaTime));
                }
            }
        }
    }

    void Engine::processEvents() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            handleWindowEvent(event);
            
            if (m_editor && m_editorEnabled) {
                m_editor->processEvent(event);
            }
        }
    }

    void Engine::update(float deltaTime) {
        // Update statistics
        updateStatistics();
        
       
        
        // Update current scene
        if (auto scene = getCurrentScene()) {
            scene->update(deltaTime);
        }
    }

    

    void Engine::renderEditor() {
        if (m_editor) {
            m_editor->beginFrame();
            m_editor->render();
            m_editor->endFrame();
        }
    }

    void Engine::handleWindowEvent(const SDL_Event& event) {
        switch (event.type) {
            case SDL_QUIT:
                m_running = false;
                break;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    handleResize(event.window.data1, event.window.data2);
                }
                break;
            case SDL_DROPFILE:
                handleDropFile(event.drop.file);
                SDL_free(event.drop.file);
                break;
            case SDL_KEYDOWN:
                handleHotkey(event.key.keysym.sym);
                break;
        }
    }

    

    void Engine::handleDropFile(const std::string& filePath) {
        // Handle dropped file
    }

    void Engine::handleHotkey(SDL_Keycode key) {
        // Handle keyboard shortcuts
    }

    void Engine::updateStatistics() {
        // Update engine statistics
        static auto lastUpdate = std::chrono::high_resolution_clock::now();
        auto now = std::chrono::high_resolution_clock::now();
        float elapsed = std::chrono::duration<float>(now - lastUpdate).count();
        
        if (elapsed > 0.5f) { // Update every 0.5 seconds
            m_stats.frameTime = elapsed * 1000.0f; // Convert to milliseconds
            m_stats.fps = 1.0f / elapsed;
            
            lastUpdate = now;
        }
    }

} // namespace CmakeProject1