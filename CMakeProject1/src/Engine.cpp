#include "../include/Engine.h"
#include "../include/Config.h"
#include "../include/ResourceManager.h"
#include "../include/ShaderManager.h"
#include "../include/MaterialManager.h"
#include "../include/TextureManager.h"
#include "../include/FontManager.h"
#include "../include/SoundManager.h"
#include "../include/AnimationManager.h"
#include "../include/PrefabManager.h"
#include "../include/TerrainSystem.h"
#include "../include/WeatherSystem.h"
#include "../include/DayNightSystem.h"
#include "../include/PostProcessing.h"
#include "../include/ShadowSystem.h"
#include "../include/ReflectionSystem.h"
#include "../include/WaterSystem.h"
#include "../include/DestructibleSystem.h"
#include "../include/DialogSystem.h"
#include "../include/QuestSystem.h"
#include "../include/InventorySystem.h"
#include "../include/CraftingSystem.h"
#include "../include/SkillSystem.h"
#include "../include/LevelingSystem.h"
#include "../include/SaveSystem.h"
#include "../include/CheckpointSystem.h"
#include "../include/CutsceneSystem.h"
#include "../include/TransitionSystem.h"
#include "../include/TutorialSystem.h"
#include "../include/AchievementSystem.h"
#include "../include/LeaderboardSystem.h"
#include "../include/MatchmakingSystem.h"
#include "../include/ReplaySystem.h"
#include "../include/ModdingSystem.h"
#include "../include/PluginSystem.h"
#include "../include/LocalizationSystem.h"
#include "../include/AccessibilitySystem.h"
#include "../include/PerformanceMonitor.h"
#include "../include/CrashReporter.h"
#include "../include/AutoUpdater.h"
#include "../include/Telemetry.h"
#include "../include/Benchmark.h"
#include "../include/StressTest.h"
#include "../include/UnitTest.h"

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>
#include <nfd.h>
#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>
#include <json/json.h>
#include <stb_image.h>
#include <stb_image_write.h>
#include <stb_truetype.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <entt/entt.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/async.h>
#include <fmt/format.h>
#include <zip.h>
#include <curl/curl.h>
#include <openssl/sha.h>
#include <sqlite3.h>
#include <discord_rpc.h>
#include <steam/steam_api.h>
#include <thread>
#include <future>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <random>
#include <execution>

#ifdef _WIN32
#include <windows.h>
#include <shellapi.h>
#include <dwmapi.h>
#include <comdef.h>
#include <Wincodec.h>
#elif defined(__linux__)
#include <dlfcn.h>
#include <sys/stat.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#elif defined(__APPLE__)
#include <CoreFoundation/CoreFoundation.h>
#include <CoreServices/CoreServices.h>
#include <Cocoa/Cocoa.h>
#include <IOKit/graphics/IOGraphicsLib.h>
#endif

namespace fs = std::filesystem;
using namespace std::chrono;
using namespace std::chrono_literals;

namespace CmakeProject1 {

    // Статические члены
    const char* Engine::VERSION = "2.0.0";
    const char* Engine::BUILD_DATE = __DATE__ " " __TIME__;
    
    // Singleton implementation
    Engine& Engine::getInstance() {
        static Engine instance;
        return instance;
    }

    Engine::Engine() {
        // Инициализация логгера
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
            "logs/engine.log", 1024 * 1024 * 5, 3);
        
        std::vector<spdlog::sink_ptr> sinks{console_sink, file_sink};
        auto logger = std::make_shared<spdlog::logger>("engine", sinks.begin(), sinks.end());
        
        spdlog::set_default_logger(logger);
        spdlog::set_level(spdlog::level::debug);
        spdlog::flush_on(spdlog::level::info);
        
        SPDLOG_INFO("Engine instance created");
        
        // Инициализация рандома
        std::random_device rd;
        std::mt19937::result_type seed = rd() ^ (
            (std::mt19937::result_type)
            std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::system_clock::now().time_since_epoch()
            ).count() +
            (std::mt19937::result_type)
            std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::high_resolution_clock::now().time_since_epoch()
            ).count()
        );
        std::mt19937 gen(seed);
        
        // Инициализация статистики
        lastStatUpdate = high_resolution_clock::now();
        stats = {};
    }

    Engine::~Engine() {
        shutdown();
    }

    bool Engine::initialize(int argc, char* argv[]) {
        SPDLOG_INFO("Initializing engine...");
        SPDLOG_INFO("Version: {}, Build: {}", VERSION, BUILD_DATE);
        
        // Парсинг аргументов командной строки
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            if (arg == "--editor") {
                editorMode = true;
            } else if (arg == "--play") {
                editorMode = false;
                playMode = true;
            } else if (arg.rfind("--project=", 0) == 0) {
                std::string projectPath = arg.substr(10);
                if (!projectPath.empty()) {
                    SPDLOG_INFO("Loading project from command line: {}", projectPath);
                    loadProject(projectPath);
                }
            } else if (arg == "--headless") {
                // Режим без графики для серверов
                config.headless = true;
            }
        }
        
        // Создание необходимых директорий
        std::vector<std::string> dirs = {
            "logs",
            "cache",
            "temp",
            "screenshots",
            "recordings",
            "backups",
            "projects",
            "plugins"
        };
        
        for (const auto& dir : dirs) {
            if (!fs::exists(dir)) {
                fs::create_directories(dir);
            }
        }
        
        // Инициализация SDL
        if (!initializeSDL()) {
            SPDLOG_ERROR("Failed to initialize SDL");
            return false;
        }
        
        // Инициализация OpenGL (если не в headless режиме)
        if (!config.headless && !initializeOpenGL()) {
            SPDLOG_ERROR("Failed to initialize OpenGL");
            return false;
        }
        
        // Инициализация ImGui (если не в headless режиме)
        if (!config.headless && !initializeImGui()) {
            SPDLOG_ERROR("Failed to initialize ImGui");
            return false;
        }
        
        // Инициализация NFD (диалоги выбора файлов)
        if (NFD_Init() != NFD_OKAY) {
            SPDLOG_WARN("Failed to initialize Native File Dialog");
        }
        
        // Инициализация систем
        if (!initializeSystems()) {
            SPDLOG_ERROR("Failed to initialize engine systems");
            return false;
        }
        
        // Инициализация инструментов
        if (!initializeTools()) {
            SPDLOG_ERROR("Failed to initialize tools");
            return false;
        }
        
        // Инициализация скриптинга
        if (!initializeScripting()) {
            SPDLOG_ERROR("Failed to initialize scripting");
            return false;
        }
        
        // Инициализация сетевой подсистемы
        if (!initializeNetworking()) {
            SPDLOG_WARN("Failed to initialize networking");
        }
        
        // Загрузка конфигурации
        if (fs::exists("config.json")) {
            loadConfig("config.json");
        } else {
            saveConfig("config.json"); // Создаем конфиг по умолчанию
        }
        
        // Инициализация графического планшета
        graphicsTablet = std::make_unique<GraphicsTablet>();
        if (!graphicsTablet->initialize()) {
            SPDLOG_WARN("Graphics tablet not found or failed to initialize");
        } else {
            SPDLOG_INFO("Graphics tablet initialized successfully");
        }
        
        // Запуск пула потоков
        workersRunning = true;
        unsigned int numThreads = std::thread::hardware_concurrency();
        if (numThreads == 0) numThreads = 4;
        
        for (unsigned int i = 0; i < numThreads; ++i) {
            workerThreads.emplace_back([this]() {
                while (workersRunning) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(taskMutex);
                        taskCondition.wait(lock, [this]() {
                            return !taskQueue.empty() || !workersRunning;
                        });
                        
                        if (!workersRunning && taskQueue.empty()) {
                            return;
                        }
                        
                        task = std::move(taskQueue.front());
                        taskQueue.pop();
                    }
                    
                    try {
                        task();
                    } catch (const std::exception& e) {
                        SPDLOG_ERROR("Task execution error: {}", e.what());
                    }
                }
            });
        }
        
        // Регистрация встроенных команд консоли
        registerBuiltinCommands();
        
        // Регистрация горячих клавиш
        registerDefaultHotkeys();
        
        running = true;
        
        SPDLOG_INFO("Engine initialized successfully!");
        SPDLOG_INFO("Available systems:");
        SPDLOG_INFO("  - Graphics: OpenGL 4.5");
        SPDLOG_INFO("  - Scripting: Lua 5.4 + C++17");
        SPDLOG_INFO("  - Physics: Custom 2D");
        SPDLOG_INFO("  - Audio: SDL_mixer");
        SPDLOG_INFO("  - Networking: TCP/UDP");
        SPDLOG_INFO("  - Platform: {}", getPlatform());
        
        return true;
    }

    bool Engine::initializeSDL() {
        SPDLOG_INFO("Initializing SDL...");
        
        uint32_t sdlFlags = SDL_INIT_EVERYTHING;
        if (config.headless) {
            sdlFlags = SDL_INIT_TIMER | SDL_INIT_EVENTS;
        }
        
        if (SDL_Init(sdlFlags) < 0) {
            SPDLOG_ERROR("SDL_Init failed: {}", SDL_GetError());
            return false;
        }
        
        // Инициализация SDL_image
        int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_TIF | IMG_INIT_WEBP;
        if ((IMG_Init(imgFlags) & imgFlags) != imgFlags) {
            SPDLOG_ERROR("IMG_Init failed: {}", IMG_GetError());
            return false;
        }
        
        // Инициализация SDL_ttf
        if (TTF_Init() == -1) {
            SPDLOG_ERROR("TTF_Init failed: {}", TTF_GetError());
            return false;
        }
        
        // Инициализация SDL_mixer
        if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_FLAC | MIX_INIT_MOD) == 0) {
            SPDLOG_ERROR("Mix_Init failed: {}", Mix_GetError());
            return false;
        }
        
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            SPDLOG_ERROR("Mix_OpenAudio failed: {}", Mix_GetError());
            return false;
        }
        
        // Настройка аудио
        Mix_AllocateChannels(32);
        Mix_Volume(-1, MIX_MAX_VOLUME / 2);
        
        SPDLOG_INFO("SDL initialized successfully");
        return true;
    }

    bool Engine::initializeOpenGL() {
        SPDLOG_INFO("Initializing OpenGL...");
        
        // Создание окна
        window = std::make_unique<Window>("2D Game Engine", 1280, 720);
        if (!window->isValid()) {
            return false;
        }
        
        // Создание OpenGL контекста
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
        
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
        
        window->createGLContext();
        
        // Инициализация GLEW
        glewExperimental = GL_TRUE;
        GLenum glewError = glewInit();
        if (glewError != GLEW_OK) {
            SPDLOG_ERROR("GLEW initialization failed: {}", glewGetErrorString(glewError));
            return false;
        }
        
        // Проверка версии OpenGL
        const GLubyte* renderer = glGetString(GL_RENDERER);
        const GLubyte* version = glGetString(GL_VERSION);
        SPDLOG_INFO("OpenGL Renderer: {}", renderer);
        SPDLOG_INFO("OpenGL Version: {}", version);
        
        // Проверка необходимых расширений
        if (!GLEW_ARB_direct_state_access) {
            SPDLOG_WARN("ARB_direct_state_access not supported, performance may be affected");
        }
        
        // Настройка OpenGL
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);
        
        if (GLEW_ARB_seamless_cube_map) {
            glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
        }
        
        // Включение VSync
        SDL_GL_SetSwapInterval(config.vsync ? 1 : 0);
        
        // Загрузка шейдеров
        if (!loadDefaultShaders()) {
            SPDLOG_ERROR("Failed to load default shaders");
            return false;
        }
        
        // Инициализация основного кадра буфера
        glGenFramebuffers(1, &mainFramebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, mainFramebuffer);
        
        // Создание текстур для буферов
        glGenTextures(1, &colorTexture);
        glBindTexture(GL_TEXTURE_2D, colorTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, window->getWidth(), window->getHeight(), 
                    0, GL_RGBA, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);
        
        glGenTextures(1, &depthTexture);
        glBindTexture(GL_TEXTURE_2D, depthTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, window->getWidth(), window->getHeight(), 
                    0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
        
        // Проверка фреймбуфера
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            SPDLOG_ERROR("Framebuffer is not complete!");
            return false;
        }
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
        SPDLOG_INFO("OpenGL initialized successfully");
        return true;
    }

    bool Engine::initializeImGui() {
        SPDLOG_INFO("Initializing ImGui...");
        
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        
        // Настройка стиля
        ImGui::StyleColorsDark();
        
        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowPadding = ImVec2(8, 8);
        style.FramePadding = ImVec2(4, 4);
        style.CellPadding = ImVec2(4, 2);
        style.ItemSpacing = ImVec2(8, 4);
        style.ItemInnerSpacing = ImVec2(4, 4);
        style.TouchExtraPadding = ImVec2(0, 0);
        style.IndentSpacing = 21;
        style.ScrollbarSize = 14;
        style.GrabMinSize = 12;
        style.WindowBorderSize = 1;
        style.ChildBorderSize = 1;
        style.PopupBorderSize = 1;
        style.FrameBorderSize = 1;
        style.TabBorderSize = 0;
        style.WindowRounding = 7;
        style.ChildRounding = 4;
        style.FrameRounding = 3;
        style.PopupRounding = 4;
        style.ScrollbarRounding = 9;
        style.GrabRounding = 3;
        style.LogSliderDeadzone = 4;
        style.TabRounding = 4;
        
        // Настройка цветовой схемы
        ImVec4* colors = style.Colors;
        colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
        colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
        colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.20f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
        colors[ImGuiCol_Tab] = ImVec4(0.18f, 0.35f, 0.58f, 0.86f);
        colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
        colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.41f, 0.68f, 1.00f);
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
        colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
        colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
        
        // Настройка шрифтов
        io.Fonts->AddFontFromFileTTF("assets/fonts/Roboto-Regular.ttf", 16.0f);
        io.Fonts->AddFontFromFileTTF("assets/fonts/Roboto-Bold.ttf", 16.0f);
        io.Fonts->AddFontFromFileTTF("assets/fonts/RobotoMono-Regular.ttf", 15.0f);
        
        // Когда изменяется размер viewport, ImGui может создать новые шрифты
        io.Fonts->AddFontDefault();
        
        // Инициализация бэкендов
        if (!ImGui_ImplSDL2_InitForOpenGL(window->getSDLWindow(), window->getGLContext())) {
            SPDLOG_ERROR("ImGui_ImplSDL2_InitForOpenGL failed");
            return false;
        }
        
        const char* glsl_version = "#version 450";
        if (!ImGui_ImplOpenGL3_Init(glsl_version)) {
            SPDLOG_ERROR("ImGui_ImplOpenGL3_Init failed");
            return false;
        }
        
        SPDLOG_INFO("ImGui initialized successfully");
        return true;
    }

    bool Engine::initializeSystems() {
        SPDLOG_INFO("Initializing engine systems...");
        
        // Менеджер времени
        timeManager = std::make_unique<TimeManager>();
        
        // Менеджер ввода
        inputManager = std::make_unique<InputManager>();
        inputManager->initialize(window->getSDLWindow());
        
        // Менеджер ресурсов
        assetManager = std::make_unique<AssetManager>();
        
        // Система событий
        eventSystem = std::make_unique<EventSystem>();
        
        // Физический движок
        physicsEngine = std::make_unique<PhysicsEngine>();
        physicsEngine->initialize();
        
        // Аудио движок
        audioEngine = std::make_unique<AudioEngine>();
        audioEngine->initialize();
        
        // Менеджер сцен
        sceneManager = std::make_unique<SceneManager>();
        
        // Менеджер проектов
        projectManager = std::make_unique<ProjectManager>();
        
        // Создание редактора
        editor = std::make_unique<Editor>();
        
        // Создание систем графики
        spriteBatch = std::make_unique<SpriteBatch>();
        spriteBatch->initialize();
        
        particleSystem = std::make_unique<ParticleSystem>();
        particleSystem->initialize();
        
        lightSystem = std::make_unique<LightSystem>();
        lightSystem->initialize();
        
        // Основная камера
        mainCamera = std::make_unique<Camera>();
        mainCamera->setOrthographic(window->getWidth(), window->getHeight());
        
        // UI системы
        uiManager = std::make_unique<UIManager>();
        uiManager->initialize(window->getSDLWindow());
        
        mainCanvas = std::make_unique<Canvas>();
        mainCanvas->initialize();
        
        // Системы анимации
        animationSystem = std::make_unique<AnimationSystem>();
        animationSystem->initialize();
        
        // AI системы
        behaviorTreeManager = std::make_unique<BehaviorTreeManager>();
        pathfinding = std::make_unique<Pathfinding>();
        
        // Системы сериализации
        serializer = std::make_unique<Serializer>();
        prefabSystem = std::make_unique<PrefabSystem>();
        
        // Дополнительные системы
        terrainSystem = std::make_unique<TerrainSystem>();
        weatherSystem = std::make_unique<WeatherSystem>();
        dayNightSystem = std::make_unique<DayNightSystem>();
        postProcessing = std::make_unique<PostProcessing>();
        shadowSystem = std::make_unique<ShadowSystem>();
        reflectionSystem = std::make_unique<ReflectionSystem>();
        waterSystem = std::make_unique<WaterSystem>();
        destructibleSystem = std::make_unique<DestructibleSystem>();
        dialogSystem = std::make_unique<DialogSystem>();
        questSystem = std::make_unique<QuestSystem>();
        inventorySystem = std::make_unique<InventorySystem>();
        craftingSystem = std::make_unique<CraftingSystem>();
        skillSystem = std::make_unique<SkillSystem>();
        levelingSystem = std::make_unique<LevelingSystem>();
        saveSystem = std::make_unique<SaveSystem>();
        checkpointSystem = std::make_unique<CheckpointSystem>();
        cutsceneSystem = std::make_unique<CutsceneSystem>();
        transitionSystem = std::make_unique<TransitionSystem>();
        tutorialSystem = std::make_unique<TutorialSystem>();
        achievementSystem = std::make_unique<AchievementSystem>();
        leaderboardSystem = std::make_unique<LeaderboardSystem>();
        matchmakingSystem = std::make_unique<MatchmakingSystem>();
        replaySystem = std::make_unique<ReplaySystem>();
        moddingSystem = std::make_unique<ModdingSystem>();
        pluginSystem = std::make_unique<PluginSystem>();
        localizationSystem = std::make_unique<LocalizationSystem>();
        accessibilitySystem = std::make_unique<AccessibilitySystem>();
        
        // Утилиты
        performanceMonitor = std::make_unique<PerformanceMonitor>();
        crashReporter = std::make_unique<CrashReporter>();
        autoUpdater = std::make_unique<AutoUpdater>();
        telemetry = std::make_unique<Telemetry>();
        benchmark = std::make_unique<Benchmark>();
        stressTest = std::make_unique<StressTest>();
        unitTest = std::make_unique<UnitTest>();
        
        SPDLOG_INFO("Engine systems initialized successfully");
        return true;
    }

    bool Engine::initializeTools() {
        SPDLOG_INFO("Initializing tools...");
        
        toolManager = std::make_unique<ToolManager>();
        
        contentBrowser = std::make_unique<ContentBrowser>();
        console = std::make_unique<Console>();
        profiler = std::make_unique<Profiler>();
        debugDraw = std::make_unique<DebugDraw>();
        sceneViewer = std::make_unique<SceneViewer>();
        gameView = std::make_unique<GameView>();
        inspector = std::make_unique<Inspector>();
        hierarchy = std::make_unique<Hierarchy>();
        
        // Регистрация инструментов в менеджере
        toolManager->registerTool("Content Browser", contentBrowser.get());
        toolManager->registerTool("Console", console.get());
        toolManager->registerTool("Profiler", profiler.get());
        toolManager->registerTool("Debug Draw", debugDraw.get());
        toolManager->registerTool("Scene Viewer", sceneViewer.get());
        toolManager->registerTool("Game View", gameView.get());
        toolManager->registerTool("Inspector", inspector.get());
        toolManager->registerTool("Hierarchy", hierarchy.get());
        
        SPDLOG_INFO("Tools initialized successfully");
        return true;
    }

    bool Engine::initializeScripting() {
        SPDLOG_INFO("Initializing scripting...");
        
        scriptEngine = std::make_unique<ScriptEngine>();
        if (!scriptEngine->initialize()) {
            SPDLOG_ERROR("Script engine initialization failed");
            return false;
        }
        
        scriptManager = std::make_unique<ScriptManager>();
        hotReload = std::make_unique<HotReload>();
        
        // Регистрация C++ функций в Lua
        registerLuaBindings();
        
        // Загрузка стандартных библиотек
        scriptEngine->loadStandardLibrary();
        
        SPDLOG_INFO("Scripting initialized successfully");
        return true;
    }

    bool Engine::initializeNetworking() {
        SPDLOG_INFO("Initializing networking...");
        
        networkManager = std::make_unique<NetworkManager>();
        if (!networkManager->initialize()) {
            SPDLOG_WARN("Network manager initialization failed");
            return false;
        }
        
        // Регистрация сетевых сообщений
        registerNetworkMessages();
        
        SPDLOG_INFO("Networking initialized successfully");
        return true;
    }

    void Engine::run() {
        SPDLOG_INFO("Starting engine main loop");
        
        // Создание дефолтного проекта если нет открытого
        if (!projectManager->hasProject()) {
            if (!editorMode) {
                SPDLOG_WARN("No project loaded, but running in play mode. Creating temporary project.");
                createProject("TemporaryProject", fs::current_path() / "projects/temp");
            } else {
                // В редакторском режиме показываем браузер проектов
                editor->showProjectBrowser();
            }
        }
        
        // Основной цикл
        mainLoop();
        
        SPDLOG_INFO("Engine main loop ended");
    }

    void Engine::mainLoop() {
        constexpr double targetFrameTime = 1.0 / 60.0; // 60 FPS
        double accumulator = 0.0;
        double currentTime = timeManager->getTime();
        
        while (running) {
            double newTime = timeManager->getTime();
            double frameTime = newTime - currentTime;
            
            // Защита от "spiral of death"
            if (frameTime > 0.25) {
                frameTime = 0.25;
            }
            
            currentTime = newTime;
            accumulator += frameTime;
            
            // Обработка событий
            processEvents();
            
            // Обновление с фиксированным шагом
            while (accumulator >= targetFrameTime) {
                preUpdate();
                update();
                postUpdate();
                
                // Обновление физики с фиксированным шагом
                physicsEngine->update(targetFrameTime);
                
                accumulator -= targetFrameTime;
            }
            
            // Интерполяция для плавной отрисовки
            double alpha = accumulator / targetFrameTime;
            
            // Рендеринг
            if (!config.headless) {
                render();
                
                // Рендеринг редактора если в режиме редактора
                if (editorMode && editor->isActive()) {
                    renderEditor();
                }
                
                // Обмен буферов
                window->swapBuffers();
            }
            
            // Ограничение FPS если нужно
            if (config.maxFPS > 0) {
                double frameEndTime = timeManager->getTime();
                double frameDuration = frameEndTime - newTime;
                double targetDuration = 1.0 / config.maxFPS;
                
                if (frameDuration < targetDuration) {
                    double sleepTime = targetDuration - frameDuration;
                    std::this_thread::sleep_for(std::chrono::duration<double>(sleepTime));
                }
            }
            
            // Обновление статистики
            updateStatistics();
            
            // Проверка на обновления (раз в секунду)
            static double lastUpdateCheck = 0;
            if (currentTime - lastUpdateCheck > 1.0) {
                checkForUpdates();
                lastUpdateCheck = currentTime;
            }
            
            // Создание бекапа проекта (раз в 5 минут)
            static double lastBackup = 0;
            if (currentTime - lastBackup > 300.0 && projectManager->hasProject()) {
                backupProject();
                lastBackup = currentTime;
            }
        }
    }

    void Engine::processEvents() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            // Сначала передаем события в ImGui
            if (editorMode) {
                ImGui_ImplSDL2_ProcessEvent(&event);
            }
            
            // Обработка событий окна
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                    
                case SDL_WINDOWEVENT:
                    handleWindowEvent(event);
                    break;
                    
                case SDL_KEYDOWN:
                case SDL_KEYUP:
                    inputManager->handleKeyboardEvent(event);
                    
                    // Глобальные горячие клавиши
                    if (event.type == SDL_KEYDOWN) {
                        handleHotkeys(event.key.keysym.sym);
                    }
                    break;
                    
                case SDL_MOUSEMOTION:
                case SDL_MOUSEBUTTONDOWN:
                case SDL_MOUSEBUTTONUP:
                case SDL_MOUSEWHEEL:
                    inputManager->handleMouseEvent(event);
                    
                    // Обработка графического планшета
                    if (graphicsTablet && graphicsTablet->isConnected()) {
                        graphicsTablet->handleEvent(event);
                    }
                    break;
                    
                case SDL_CONTROLLERBUTTONDOWN:
                case SDL_CONTROLLERBUTTONUP:
                case SDL_CONTROLLERAXISMOTION:
                case SDL_CONTROLLERDEVICEADDED:
                case SDL_CONTROLLERDEVICEREMOVED:
                    inputManager->handleGamepadEvent(event);
                    break;
                    
                case SDL_DROPFILE:
                case SDL_DROPTEXT:
                case SDL_DROPBEGIN:
                case SDL_DROPCOMPLETE:
                    handleDropEvent(event);
                    break;
                    
                case SDL_TEXTINPUT:
                    inputManager->handleTextInputEvent(event);
                    break;
                    
                case SDL_USEREVENT:
                    handleUserEvent(event);
                    break;
            }
            
            // Обработка событий редактора
            if (editorMode) {
                editor->processEvent(event);
            }
        }
    }

    void Engine::preUpdate() {
        // Обновление времени
        timeManager->update();
        
        // Обновление ввода
        inputManager->update();
        
        // Обновление графического планшета
        if (graphicsTablet) {
            graphicsTablet->update();
        }
        
        // Обновление UI
        uiManager->update();
        
        // Обновление консоли
        console->update();
        
        // Обновление скриптов
        scriptEngine->update();
        
        // Горячая перезагрузка скриптов
        if (hotReload->checkForChanges()) {
            hotReload->reloadScripts();
        }
    }

    void Engine::update() {
        // Обновление активной сцены
        if (auto scene = sceneManager->getActiveScene()) {
            scene->update();
        }
        
        // Обновление анимаций
        animationSystem->update();
        
        // Обновление частиц
        particleSystem->update();
        
        // Обновление освещения
        lightSystem->update();
        
        // Обновление AI
        behaviorTreeManager->update();
        pathfinding->update();
        
        // Обновление сетевого менеджера
        if (networkManager->isActive()) {
            networkManager->update();
        }
        
        // Обновление звука
        audioEngine->update();
        
        // Обновление систем игры
        updateGameSystems();
    }

    void Engine::postUpdate() {
        // Обновление событий
        eventSystem->dispatchEvents();
        
        // Сборка мусора
        garbageCollect();
        
        // Проверка на ошибки OpenGL
        checkGLErrors();
        
        // Обновление инструментов
        if (editorMode) {
            toolManager->update();
        }
    }

    void Engine::render() {
        // Начало нового кадра ImGui
        if (editorMode) {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL2_NewFrame();
            ImGui::NewFrame();
        }
        
        // Привязка основного фреймбуфера
        glBindFramebuffer(GL_FRAMEBUFFER, mainFramebuffer);
        glViewport(0, 0, window->getWidth(), window->getHeight());
        
        // Очистка буферов
        glClearColor(config.clearColor.r, config.clearColor.g, 
                    config.clearColor.b, config.clearColor.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
        // Рендеринг сцены
        if (auto scene = sceneManager->getActiveScene()) {
            // Настройка камеры
            glm::mat4 viewProj = mainCamera->getViewProjectionMatrix();
            
            // Рендеринг спрайтов
            spriteBatch->begin(viewProj);
            scene->render(spriteBatch.get());
            spriteBatch->end();
            
            // Рендеринг частиц
            particleSystem->render();
            
            // Рендеринг освещения
            lightSystem->render();
            
            // Рендеринг пост-обработки
            postProcessing->applyEffects(colorTexture);
            
            // Рендеринг UI
            uiManager->render();
            
            // Рендеринг дебаг-информации
            if (config.showDebugInfo) {
                debugDraw->render();
            }
        }
        
        // Копирование результата в основной буфер
        glBindFramebuffer(GL_READ_FRAMEBUFFER, mainFramebuffer);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBlitFramebuffer(0, 0, window->getWidth(), window->getHeight(),
                        0, 0, window->getWidth(), window->getHeight(),
                        GL_COLOR_BUFFER_BIT, GL_LINEAR);
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Engine::renderEditor() {
        // Рендеринг окон редактора
        if (editor->isActive()) {
            editor->render();
        }
        
        // Рендеринг инструментов
        toolManager->render();
        
        // Рендеринг ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        // Обновление viewports (для поддержки нескольких окон)
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
            SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
        }
    }

    bool Engine::createProject(const std::string& name, const fs::path& location) {
        SPDLOG_INFO("Creating project: {} at {}", name, location.string());
        
        if (name.empty()) {
            SPDLOG_ERROR("Project name cannot be empty");
            return false;
        }
        
        if (!fs::exists(location)) {
            if (!fs::create_directories(location)) {
                SPDLOG_ERROR("Failed to create project directory: {}", location.string());
                return false;
            }
        }
        
        // Создание структуры проекта
        std::vector<std::string> projectDirs = {
            "Assets",
            "Assets/Sprites",
            "Assets/Sounds",
            "Assets/Music",
            "Assets/Animations",
            "Assets/Prefabs",
            "Assets/Materials",
            "Assets/Shaders",
            "Assets/Fonts",
            "Assets/UI",
            "Scripts",
            "Scenes",
            "Settings",
            "Build",
            "Docs"
        };
        
        for (const auto& dir : projectDirs) {
            fs::path dirPath = location / dir;
            if (!fs::exists(dirPath)) {
                fs::create_directories(dirPath);
            }
        }
        
        // Создание файлов проекта
        Json::Value projectJson;
        projectJson["name"] = name;
        projectJson["version"] = "1.0.0";
        projectJson["engineVersion"] = VERSION;
        projectJson["created"] = time(nullptr);
        projectJson["lastModified"] = time(nullptr);
        
        projectJson["settings"]["window"]["width"] = 1280;
        projectJson["settings"]["window"]["height"] = 720;
        projectJson["settings"]["window"]["fullscreen"] = false;
        projectJson["settings"]["window"]["vsync"] = true;
        projectJson["settings"]["window"]["resizable"] = true;
        
        projectJson["settings"]["graphics"]["antialiasing"] = true;
        projectJson["settings"]["graphics"]["shadows"] = true;
        projectJson["settings"]["graphics"]["bloom"] = false;
        projectJson["settings"]["graphics"]["ssao"] = false;
        projectJson["settings"]["graphics"]["motionBlur"] = false;
        
        projectJson["settings"]["audio"]["masterVolume"] = 1.0;
        projectJson["settings"]["audio"]["musicVolume"] = 0.8;
        projectJson["settings"]["audio"]["sfxVolume"] = 0.9;
        projectJson["settings"]["audio"]["voiceVolume"] = 1.0;
        
        projectJson["settings"]["physics"]["gravity"] = 9.81;
        projectJson["settings"]["physics"]["fixedTimestep"] = 0.0166667;
        projectJson["settings"]["physics"]["velocityIterations"] = 8;
        projectJson["settings"]["physics"]["positionIterations"] = 3;
        
        // Сохранение файла проекта
        std::ofstream projectFile(location / (name + ".project"));
        if (!projectFile.is_open()) {
            SPDLOG_ERROR("Failed to create project file");
            return false;
        }
        
        Json::StreamWriterBuilder writer;
        writer["indentation"] = "    ";
        std::string jsonStr = Json::writeString(writer, projectJson);
        projectFile << jsonStr;
        projectFile.close();
        
        // Создание дефолтной сцены
        Json::Value sceneJson;
        sceneJson["name"] = "MainScene";
        sceneJson["ambientLight"] = Json::Value(Json::arrayValue);
        sceneJson["ambientLight"].append(0.2);
        sceneJson["ambientLight"].append(0.2);
        sceneJson["ambientLight"].append(0.2);
        sceneJson["gravity"] = Json::Value(Json::arrayValue);
        sceneJson["gravity"].append(0);
        sceneJson["gravity"].append(-9.81);
        
        std::ofstream sceneFile(location / "Scenes" / "MainScene.scene");
        if (sceneFile.is_open()) {
            jsonStr = Json::writeString(writer, sceneJson);
            sceneFile << jsonStr;
            sceneFile.close();
        }
        
        // Создание дефолтного скрипта
        std::ofstream scriptFile(location / "Scripts" / "Player.lua");
        if (scriptFile.is_open()) {
            scriptFile << R"(
-- Player script
Player = {}

function Player:Start()
    self.transform = self:GetComponent("Transform")
    self.sprite = self:GetComponent("SpriteRenderer")
    self.speed = 5.0
    self.jumpForce = 10.0
    self.isGrounded = false
end

function Player:Update(deltaTime)
    local input = Input:GetAxis("Horizontal")
    local velocity = self.transform.velocity
    
    velocity.x = input * self.speed
    
    if Input:GetKeyDown("Space") and self.isGrounded then
        velocity.y = self.jumpForce
        self.isGrounded = false
    end
    
    self.transform.velocity = velocity
    
    -- Анимация
    if math.abs(velocity.x) > 0.1 then
        self.sprite.flipX = velocity.x < 0
    end
end

function Player:OnCollisionEnter(other)
    if other.tag == "Ground" then
        self.isGrounded = true
    end
end

return Player
)";
            scriptFile.close();
        }
        
        // Загрузка проекта
        if (!projectManager->loadProject(location / (name + ".project"))) {
            SPDLOG_ERROR("Failed to load newly created project");
            return false;
        }
        
        SPDLOG_INFO("Project created successfully: {}", name);
        return true;
    }

    bool Engine::loadProject(const fs::path& projectPath) {
        SPDLOG_INFO("Loading project: {}", projectPath.string());
        
        if (!fs::exists(projectPath)) {
            SPDLOG_ERROR("Project file does not exist: {}", projectPath.string());
            return false;
        }
        
        // Закрытие текущего проекта если есть
        if (projectManager->hasProject()) {
            closeProject();
        }
        
        // Загрузка проекта
        if (!projectManager->loadProject(projectPath)) {
            SPDLOG_ERROR("Failed to load project: {}", projectPath.string());
            return false;
        }
        
        // Загрузка стартовой сцены
        auto startScene = projectManager->getStartScene();
        if (!startScene.empty()) {
            loadScene(startScene);
        }
        
        // Обновление UI редактора
        if (editorMode) {
            editor->onProjectLoaded();
        }
        
        SPDLOG_INFO("Project loaded successfully");
        return true;
    }

    void Engine::shutdown() {
        SPDLOG_INFO("Shutting down engine...");
        
        running = false;
        
        // Остановка пула потоков
        workersRunning = false;
        taskCondition.notify_all();
        
        for (auto& thread : workerThreads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
        
        workerThreads.clear();
        
        // Сохранение текущего проекта
        if (projectManager->hasProject()) {
            saveProject();
        }
        
        // Сохранение конфигурации
        saveConfig("config.json");
        
        // Очистка ресурсов
        assetManager->clear();
        
        // Завершение работы систем
        if (networkManager) {
            networkManager->shutdown();
        }
        
        if (audioEngine) {
            audioEngine->shutdown();
        }
        
        if (physicsEngine) {
            physicsEngine->shutdown();
        }
        
        if (scriptEngine) {
            scriptEngine->shutdown();
        }
        
        if (graphicsTablet) {
            graphicsTablet->shutdown();
        }
        
        // Завершение работы ImGui
        if (!config.headless) {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplSDL2_Shutdown();
            ImGui::DestroyContext();
        }
        
        // Очистка OpenGL ресурсов
        if (!config.headless) {
            glDeleteFramebuffers(1, &mainFramebuffer);
            glDeleteTextures(1, &colorTexture);
            glDeleteTextures(1, &depthTexture);
            
            for (auto& [name, shader] : shaders) {
                glDeleteProgram(shader);
            }
        }
        
        // Закрытие окна
        if (window) {
            window->destroy();
        }
        
        // Завершение SDL
        Mix_CloseAudio();
        Mix_Quit();
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        
        // Закрытие логгера
        spdlog::shutdown();
        
        SPDLOG_INFO("Engine shutdown complete");
    }

    // ... (другие методы класса Engine)

    const char* Engine::getPlatform() {
        #ifdef _WIN32
            return "Windows";
        #elif defined(__linux__)
            return "Linux";
        #elif defined(__APPLE__)
            return "macOS";
        #else
            return "Unknown";
        #endif
    }

    void Engine::handleError(const std::string& error) {
        SPDLOG_ERROR("Engine error: {}", error);
        
        // Запись в файл ошибок
        std::ofstream errorFile("logs/errors.log", std::ios::app);
        if (errorFile.is_open()) {
            auto now = std::chrono::system_clock::now();
            auto now_time = std::chrono::system_clock::to_time_t(now);
            errorFile << std::put_time(std::localtime(&now_time), "%Y-%m-%d %H:%M:%S");
            errorFile << " - " << error << std::endl;
            errorFile.close();
        }
        
        // Отправка отчета об ошибке
        if (crashReporter) {
            crashReporter->reportError(error);
        }
        
        // Показ диалога ошибки в редакторе
        if (editorMode && !config.headless) {
            editor->showErrorDialog("Engine Error", error);
        }
    }

} // namespace CmakeProject1