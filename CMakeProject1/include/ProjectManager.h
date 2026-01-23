#pragma once

#include "Types.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <ProjectManager.h>
#include <GameObject.h>

namespace CmakeProject1 {
    class GameObject;
    class Project {
    public:
        Project();

        bool load(const std::string& path);
        bool save(const std::string& path = "");
        bool create(const std::string& name, const std::string& path);

        // Project info
        std::string getName() const { return m_name; }
        std::string getPath() const { return m_path; }
        std::string getVersion() const { return m_version; }
        std::string getEngineVersion() const { return m_engineVersion; }
        time_t getCreatedDate() const { return m_createdDate; }
        time_t getModifiedDate() const { return m_modifiedDate; }


        std::string getAssetsPath() const { return m_assetsPath; }
        std::string getScenesPath() const { return m_scenesPath; }
        std::string getScriptsPath() const { return m_scriptsPath; }
        std::string getPrefabsPath() const { return m_prefabsPath; }
        std::string getSettingsPath() const { return m_settingsPath; }
        std::string getBuildPath() const { return m_buildPath; }
        // Settings
        struct Settings {
            // Window
            int windowWidth = 1280;
            int windowHeight = 720;
            bool fullscreen = false;
            bool resizable = true;
            bool vsync = true;

            // Graphics
            int targetFPS = 60;
            int msaaSamples = 4;
            bool enableLighting = true;
            bool enableShadows = false;
            bool enableBloom = false;
            bool enableSSAO = false;

            // Physics
            Vector2 gravity = Vector2(0, -9.81f);
            float physicsTimeStep = 1.0f / 60.0f;
            bool enablePhysics = true;

            // Audio
            float masterVolume = 1.0f;
            float musicVolume = 0.8f;
            float sfxVolume = 0.9f;
            bool enableAudio = true;

            // Input
            float mouseSensitivity = 1.0f;
            float gamepadDeadzone = 0.2f;
            bool enableTablet = true;

            // Build
            std::string buildTarget = "Windows";
            std::string buildConfiguration = "Development";
            bool debugBuild = false;
            bool optimizeBuild = true;
        };

        Settings& getSettings() { return m_settings; }
        const Settings& getSettings() const { return m_settings; }

        // Asset management
        std::vector<std::string> getScenes() const;
        std::string getStartScene() const { return m_startScene; }
        void setStartScene(const std::string& scene) { m_startScene = scene; }

        std::vector<std::string> getPrefabs() const;
        std::vector<std::string> getScripts() const;
        std::vector<std::string> getTextures() const;
        std::vector<std::string> getAudioFiles() const;
        std::vector<std::string> getFonts() const;

        // Recent files
        void addRecentFile(const std::string& path);
        std::vector<std::string> getRecentFiles() const { return m_recentFiles; }

        // Dependencies
        void addDependency(const std::string& package, const std::string& version);
        void removeDependency(const std::string& package);
        std::unordered_map<std::string, std::string> getDependencies() const { return m_dependencies; }

        // Build
        bool build(const std::string& target = "", const std::string& configuration = "");
        bool run();
        bool deploy(const std::string& platform = "");

    private:
        std::string m_name;
        std::string m_path;
        std::string m_version = "1.0.0";
        std::string m_engineVersion;
        std::string m_startScene;

        time_t m_createdDate;
        time_t m_modifiedDate;

        Settings m_settings;

        std::vector<std::string> m_recentFiles;
        std::unordered_map<std::string, std::string> m_dependencies;

        // File structure
        std::string m_assetsPath;
        std::string m_scenesPath;
        std::string m_scriptsPath;
        std::string m_prefabsPath;
        std::string m_settingsPath;
        std::string m_buildPath;

        bool validateProjectStructure() const;
        void createDefaultStructure();
    };

    class ProjectManager {
    public:
        ProjectManager();
        ~ProjectManager();

        bool createProject(const std::string& name, const std::string& path);
        bool loadProject(const std::string& path);
        bool saveProject();
        void closeProject();

        bool hasProject() const { return m_currentProject != nullptr; }
        Project* getCurrentProject() const { return m_currentProject.get(); }

        std::string getProjectName() const;
        std::string getProjectPath() const;

        // Recent projects
        void addRecentProject(const std::string& path);
        std::vector<std::string> getRecentProjects() const { return m_recentProjects; }
        void clearRecentProjects() { m_recentProjects.clear(); }

        // Templates
        std::vector<std::string> getAvailableTemplates() const;
        bool createProjectFromTemplate(const std::string& name, const std::string& path,
            const std::string& templateName);

        // Build
        bool buildCurrentProject(const std::string& target = "", const std::string& configuration = "");
        bool runCurrentProject();
        bool deployCurrentProject(const std::string& platform = "");

        // Asset operations
        bool importAsset(const std::string& sourcePath, const std::string& targetFolder = "Assets");
        bool deleteAsset(const std::string& assetPath);
        bool renameAsset(const std::string& oldPath, const std::string& newName);
        bool moveAsset(const std::string& oldPath, const std::string& newPath);

        // Scene management
        bool createScene(const std::string& name);
        bool saveScene(const std::string& scenePath, std::shared_ptr<class Scene> scene);
        std::shared_ptr<class Scene> loadScene(const std::string& scenePath);

        // Prefab management
        bool createPrefab(std::shared_ptr<class GameObject> gameObject, const std::string& path);
        std::shared_ptr<class GameObject> loadPrefab(const std::string& path);

    private:
        std::unique_ptr<Project> m_currentProject;
        std::vector<std::string> m_recentProjects;

        std::string m_projectsPath;

        void loadRecentProjects();
        void saveRecentProjects();
    };

} // namespace CmakeProject1