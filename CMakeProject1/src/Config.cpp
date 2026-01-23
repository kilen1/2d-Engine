#include "../include/Config.h"
#include <fstream>
#include <json/json.h>

namespace CmakeProject1 {

    void EngineConfig::load(const std::string& filepath) {
        // Load configuration from a file
        // This would typically parse a JSON or INI file
    }

    void EngineConfig::save(const std::string& filepath) {
        // Save configuration to a file
        // This would typically write a JSON or INI file
    }

    void EngineConfig::resetToDefaults() {
        // Reset all values to defaults
        windowTitle = "2D Game Engine";
        windowWidth = 1600;
        windowHeight = 900;
        windowFullscreen = false;
        windowResizable = true;
        windowBorderless = false;
        windowMaximized = false;
        windowPosX = 100;
        windowPosY = 100;

        vsync = true;
        targetFPS = 60;
        msaaSamples = 4;
        depthTest = true;
        blend = true;
        clearColor = Color(0.1f, 0.1f, 0.1f, 1.0f);

        showEditor = true;
        editorTheme = "Dark";
        editorLanguage = "en";
        autoSave = true;
        autoSaveInterval = 300;
        showGrid = true;
        snapToGrid = true;
        gridSize = 32.0f;

        lastProjectPath = "";
        defaultProjectPath = "Projects";

        mouseSensitivity = 1.0f;
        gamepadDeadzone = 0.2f;
        tabletEnabled = true;

        masterVolume = 1.0f;
        musicVolume = 0.8f;
        sfxVolume = 0.9f;
        audioChannels = 32;
        audioSampleRate = 44100;

        gravity = Vector2(0, -9.81f);
        physicsTimeStep = 1.0f / 60.0f;
        velocityIterations = 8;
        positionIterations = 3;
        showColliders = false;

        scriptLanguage = "Lua";
        hotReload = true;
        debugScripts = false;

        enableNetworking = false;
        defaultPort = 7777;
        defaultServer = "localhost";

        multithreadedRendering = true;
        compressAssets = false;
        cacheShaders = true;
        enableProfiling = false;
        logToFile = true;
        logLevel = 2;
    }

} // namespace CmakeProject1