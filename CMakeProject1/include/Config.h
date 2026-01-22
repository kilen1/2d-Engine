#pragma once

#include "Types.h"
#include <string>

namespace CmakeProject1 {

    struct EngineConfig {
        // Window
        std::string windowTitle = "2D Game Engine";
        int windowWidth = 1600;
        int windowHeight = 900;
        bool windowFullscreen = false;
        bool windowResizable = true;
        bool windowBorderless = false;
        bool windowMaximized = false;
        int windowPosX = 100;
        int windowPosY = 100;

        // Graphics
        bool vsync = true;
        int targetFPS = 60;
        int msaaSamples = 4;
        bool depthTest = true;
        bool blend = true;
        Color clearColor = Color(0.1f, 0.1f, 0.1f, 1.0f);

        // Editor
        bool showEditor = true;
        std::string editorTheme = "Dark";
        std::string editorLanguage = "en";
        bool autoSave = true;
        int autoSaveInterval = 300; // seconds
        bool showGrid = true;
        bool snapToGrid = true;
        float gridSize = 32.0f;

        // Project
        std::string lastProjectPath = "";
        std::vector<std::string> recentProjects;
        std::string defaultProjectPath = "Projects";

        // Input
        float mouseSensitivity = 1.0f;
        float gamepadDeadzone = 0.2f;
        bool tabletEnabled = true;

        // Audio
        float masterVolume = 1.0f;
        float musicVolume = 0.8f;
        float sfxVolume = 0.9f;
        int audioChannels = 32;
        int audioSampleRate = 44100;

        // Physics
        Vector2 gravity = Vector2(0, -9.81f);
        float physicsTimeStep = 1.0f / 60.0f;
        int velocityIterations = 8;
        int positionIterations = 3;
        bool showColliders = false;

        // Scripting
        std::string scriptLanguage = "Lua";
        bool hotReload = true;
        bool debugScripts = false;

        // Networking
        bool enableNetworking = false;
        int defaultPort = 7777;
        std::string defaultServer = "localhost";

        // Advanced
        bool multithreadedRendering = true;
        bool compressAssets = false;
        bool cacheShaders = true;
        bool enableProfiling = false;
        bool logToFile = true;
        int logLevel = 2; // 0=Error, 1=Warn, 2=Info, 3=Debug

        // Platform specific
#ifdef _WIN32
        bool useDirectX = false;
#endif

        void load(const std::string& filepath);
        void save(const std::string& filepath);
        void resetToDefaults();
    };

} // namespace CmakeProject1