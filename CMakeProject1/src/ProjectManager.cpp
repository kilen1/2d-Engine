#include "../include/ProjectManager.h"
#include "../include/Scene.h"
#include <fstream>
#include <filesystem>
#include <json/json.h> // Assuming JSON library is available

namespace CmakeProject1 {

    Project::Project() {
        time(&m_createdDate);
        time(&m_modifiedDate);
        m_engineVersion = "1.0.0";
    }

    bool Project::load(const std::string& path) {
        m_path = path;
        // Load project file (JSON format)
        // This would parse the project configuration file
        return true;
    }

    bool Project::save(const std::string& path) {
        std::string savePath = path.empty() ? m_path : path;
        if (savePath.empty()) return false;
        
        // Update modification date
        time(&m_modifiedDate);
        
        // Save project to file (JSON format)
        // This would serialize the project configuration to a file
        return true;
    }

    bool Project::create(const std::string& name, const std::string& path) {
        m_name = name;
        m_path = path;
        
        // Create project directories
        std::filesystem::create_directories(path + "/Assets");
        std::filesystem::create_directories(path + "/Assets/Scenes");
        std::filesystem::create_directories(path + "/Assets/Scripts");
        std::filesystem::create_directories(path + "/Assets/Prefabs");
        std::filesystem::create_directories(path + "/Assets/Textures");
        std::filesystem::create_directories(path + "/Assets/Audio");
        std::filesystem::create_directories(path + "/Assets/Fonts");
        
        // Set up default paths
        m_assetsPath = path + "/Assets";
        m_scenesPath = path + "/Assets/Scenes";
        m_scriptsPath = path + "/Assets/Scripts";
        m_prefabsPath = path + "/Assets/Prefabs";
        m_settingsPath = path + "/ProjectSettings";
        m_buildPath = path + "/Build";
        
        // Create default project structure
        createDefaultStructure();
        
        return true;
    }

    std::vector<std::string> Project::getScenes() const {
        std::vector<std::string> scenes;
        // Scan scenes directory for scene files
        if (std::filesystem::exists(m_scenesPath)) {
            for (const auto& entry : std::filesystem::directory_iterator(m_scenesPath)) {
                if (entry.path().extension() == ".scene") {
                    scenes.push_back(entry.path().string());
                }
            }
        }
        return scenes;
    }

    std::vector<std::string> Project::getPrefabs() const {
        std::vector<std::string> prefabs;
        // Scan prefabs directory for prefab files
        if (std::filesystem::exists(m_prefabsPath)) {
            for (const auto& entry : std::filesystem::directory_iterator(m_prefabsPath)) {
                if (entry.path().extension() == ".prefab") {
                    prefabs.push_back(entry.path().string());
                }
            }
        }
        return prefabs;
    }

    std::vector<std::string> Project::getScripts() const {
        std::vector<std::string> scripts;
        // Scan scripts directory for script files
        if (std::filesystem::exists(m_scriptsPath)) {
            for (const auto& entry : std::filesystem::directory_iterator(m_scriptsPath)) {
                if (entry.path().extension() == ".lua" || entry.path().extension() == ".cpp") {
                    scripts.push_back(entry.path().string());
                }
            }
        }
        return scripts;
    }

    std::vector<std::string> Project::getTextures() const {
        std::vector<std::string> textures;
        // Scan textures directory for texture files
        if (std::filesystem::exists(m_assetsPath + "/Textures")) {
            for (const auto& entry : std::filesystem::directory_iterator(m_assetsPath + "/Textures")) {
                if (entry.path().extension() == ".png" || entry.path().extension() == ".jpg" || 
                    entry.path().extension() == ".jpeg" || entry.path().extension() == ".bmp") {
                    textures.push_back(entry.path().string());
                }
            }
        }
        return textures;
    }

    std::vector<std::string> Project::getAudioFiles() const {
        std::vector<std::string> audioFiles;
        // Scan audio directory for audio files
        if (std::filesystem::exists(m_assetsPath + "/Audio")) {
            for (const auto& entry : std::filesystem::directory_iterator(m_assetsPath + "/Audio")) {
                if (entry.path().extension() == ".wav" || entry.path().extension() == ".mp3" || 
                    entry.path().extension() == ".ogg") {
                    audioFiles.push_back(entry.path().string());
                }
            }
        }
        return audioFiles;
    }

    std::vector<std::string> Project::getFonts() const {
        std::vector<std::string> fonts;
        // Scan fonts directory for font files
        if (std::filesystem::exists(m_assetsPath + "/Fonts")) {
            for (const auto& entry : std::filesystem::directory_iterator(m_assetsPath + "/Fonts")) {
                if (entry.path().extension() == ".ttf" || entry.path().extension() == ".otf") {
                    fonts.push_back(entry.path().string());
                }
            }
        }
        return fonts;
    }

    void Project::addRecentFile(const std::string& path) {
        // Add to recent files list, maintaining only the most recent ones
        m_recentFiles.insert(m_recentFiles.begin(), path);
        if (m_recentFiles.size() > 10) { // Keep only last 10 files
            m_recentFiles.resize(10);
        }
    }

    void Project::addDependency(const std::string& package, const std::string& version) {
        m_dependencies[package] = version;
    }

    void Project::removeDependency(const std::string& package) {
        m_dependencies.erase(package);
    }

    bool Project::build(const std::string& target, const std::string& configuration) {
        // Implementation would handle building the project
        return true;
    }

    bool Project::run() {
        // Implementation would handle running the project
        return true;
    }

    bool Project::deploy(const std::string& platform) {
        // Implementation would handle deploying the project
        return true;
    }

    bool Project::validateProjectStructure() const {
        // Check if required project directories exist
        return std::filesystem::exists(m_path + "/Assets") &&
               std::filesystem::exists(m_path + "/Assets/Scenes") &&
               std::filesystem::exists(m_path + "/Assets/Scripts");
    }

    void Project::createDefaultStructure() {
        // Create default project files and settings
        m_settings = Settings();
    }


    ProjectManager::ProjectManager() {
        // Initialize project manager
        m_projectsPath = "Projects";
    }

    ProjectManager::~ProjectManager() {
        // Save current project if needed
        if (m_currentProject) {
            saveProject();
        }
    }

    bool ProjectManager::createProject(const std::string& name, const std::string& path) {
        auto project = std::make_unique<Project>();
        if (project->create(name, path)) {
            m_currentProject = std::move(project);
            addRecentProject(path);
            return true;
        }
        return false;
    }

    bool ProjectManager::loadProject(const std::string& path) {
        auto project = std::make_unique<Project>();
        if (project->load(path)) {
            m_currentProject = std::move(project);
            addRecentProject(path);
            return true;
        }
        return false;
    }

    bool ProjectManager::saveProject() {
        if (m_currentProject) {
            return m_currentProject->save();
        }
        return false;
    }

    void ProjectManager::closeProject() {
        if (m_currentProject) {
            saveProject(); // Save before closing
            m_currentProject.reset();
        }
    }

    std::string ProjectManager::getProjectName() const {
        if (m_currentProject) {
            return m_currentProject->getName();
        }
        return "";
    }

    std::string ProjectManager::getProjectPath() const {
        if (m_currentProject) {
            return m_currentProject->getPath();
        }
        return "";
    }

    void ProjectManager::addRecentProject(const std::string& path) {
        // Add to recent projects list, maintaining only the most recent ones
        m_recentProjects.insert(m_recentProjects.begin(), path);
        if (m_recentProjects.size() > 10) { // Keep only last 10 projects
            m_recentProjects.resize(10);
        }
        saveRecentProjects();
    }

    std::vector<std::string> ProjectManager::getAvailableTemplates() const {
        std::vector<std::string> templates;
        // Look for template directories
        if (std::filesystem::exists("Templates")) {
            for (const auto& entry : std::filesystem::directory_iterator("Templates")) {
                if (entry.is_directory()) {
                    templates.push_back(entry.path().filename().string());
                }
            }
        }
        return templates;
    }

    bool ProjectManager::createProjectFromTemplate(const std::string& name, const std::string& path,
                                                   const std::string& templateName) {
        // Implementation would copy template files to create new project
        return createProject(name, path);
    }

    bool ProjectManager::buildCurrentProject(const std::string& target, const std::string& configuration) {
        if (m_currentProject) {
            return m_currentProject->build(target, configuration);
        }
        return false;
    }

    bool ProjectManager::runCurrentProject() {
        if (m_currentProject) {
            return m_currentProject->run();
        }
        return false;
    }

    bool ProjectManager::deployCurrentProject(const std::string& platform) {
        if (m_currentProject) {
            return m_currentProject->deploy(platform);
        }
        return false;
    }

    bool ProjectManager::importAsset(const std::string& sourcePath, const std::string& targetFolder) {
        if (!m_currentProject) return false;
        
        try {
            std::string targetPath = m_currentProject->getPath() + "/" + targetFolder;
            std::filesystem::create_directories(targetPath);
            
            std::string destPath = targetPath + "/" + std::filesystem::path(sourcePath).filename().string();
            std::filesystem::copy_file(sourcePath, destPath);
            
            return true;
        } catch (...) {
            return false;
        }
    }

    bool ProjectManager::deleteAsset(const std::string& assetPath) {
        if (!m_currentProject) return false;
        
        try {
            std::filesystem::remove(assetPath);
            return true;
        } catch (...) {
            return false;
        }
    }

    bool ProjectManager::renameAsset(const std::string& oldPath, const std::string& newName) {
        if (!m_currentProject) return false;
        
        try {
            std::string newPath = std::filesystem::path(oldPath).parent_path().string() + "/" + newName;
            std::filesystem::rename(oldPath, newPath);
            return true;
        } catch (...) {
            return false;
        }
    }

    bool ProjectManager::moveAsset(const std::string& oldPath, const std::string& newPath) {
        if (!m_currentProject) return false;
        
        try {
            std::filesystem::rename(oldPath, newPath);
            return true;
        } catch (...) {
            return false;
        }
    }

    bool ProjectManager::createScene(const std::string& name) {
        if (!m_currentProject) return false;
        
        // Create a new scene file
        std::string scenePath = m_currentProject->getScenesPath() + "/" + name + ".scene";
        std::ofstream file(scenePath);
        if (file.is_open()) {
            // Write basic scene data
            file << "{\n";
            file << "  \"name\": \"" << name << "\",\n";
            file << "  \"gameObjects\": []\n";
            file << "}\n";
            file.close();
            return true;
        }
        return false;
    }

    bool ProjectManager::saveScene(const std::string& scenePath, std::shared_ptr<Scene> scene) {
        if (!scene) return false;
        
        // Save scene to file
        // This would serialize the scene to a file
        return true;
    }

    std::shared_ptr<Scene> ProjectManager::loadScene(const std::string& scenePath) {
        // Load scene from file
        // This would deserialize a scene from a file
        return std::make_shared<Scene>("Loaded Scene");
    }

    bool ProjectManager::createPrefab(std::shared_ptr<GameObject> gameObject, const std::string& path) {
        if (!gameObject) return false;
        
        // Save game object as prefab
        // This would serialize the game object to a prefab file
        return true;
    }

    std::shared_ptr<GameObject> ProjectManager::loadPrefab(const std::string& path) {
        // Load prefab from file
        // This would deserialize a prefab into a game object
        return std::make_shared<GameObject>("Prefab Instance");
    }

    void ProjectManager::loadRecentProjects() {
        // Load recent projects from a file
        std::ifstream file("recent_projects.json");
        if (file.is_open()) {
            // Parse recent projects
        }
    }

    void ProjectManager::saveRecentProjects() {
        // Save recent projects to a file
        std::ofstream file("recent_projects.json");
        if (file.is_open()) {
            // Write recent projects
        }
    }

} // namespace CmakeProject1