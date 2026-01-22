#pragma once

#include "Types.h"
#include <SDL2/SDL.h>
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include <functional>
#include <filesystem>
#include <any>

// Forward declarations
struct ImVec2;
struct ImVec4;
namespace ImGui {}
struct ImGuiContext;

namespace CmakeProject1 {

    class GameObject;
    class Scene;
    class Texture;
    class Sprite;
    class Material;
    class Shader;
    class Script;
    class Camera;
    class Light2D;
    class ParticleSystem;
    class Terrain2D;
    class Tilemap;
    class AnimationClip;
    class AudioClip;
    class Font;

    class Editor {
    public:
        Editor();
        ~Editor();

        bool initialize(SDL_Window* window, SDL_GLContext context);
        void shutdown();

        void beginFrame();
        void endFrame();

        void render();
        void processEvent(const SDL_Event& event);

        // State
        void setActive(bool active) { m_active = active; }
        bool isActive() const { return m_active; }

        void setPlayMode(bool play) { m_playMode = play; }
        bool isPlayMode() const { return m_playMode; }

        void setPaused(bool paused) { m_paused = paused; }
        bool isPaused() const { return m_paused; }

        // Windows
        void showSceneHierarchy(bool show = true);
        void showInspector(bool show = true);
        void showProjectBrowser(bool show = true);
        void showConsole(bool show = true);
        void showAnimationEditor(bool show = true);
        void showParticleEditor(bool show = true);
        void showMaterialEditor(bool show = true);
        void showShaderEditor(bool show = true);
        void showTilemapEditor(bool show = true);
        void showTerrainEditor(bool show = true);
        void showLightingEditor(bool show = true);
        void showPostProcessingEditor(bool show = true);
        void showPhysicsEditor(bool show = true);
        void showAudioEditor(bool show = true);
        void showNavigationEditor(bool show = true);
        void showUIEditor(bool show = true);
        void showScriptEditor(bool show = true);
        void showBuildSettings(bool show = true);
        void showPlayerSettings(bool show = true);
        void showPackageManager(bool show = true);
        void showVersionControl(bool show = true);
        void showProfiler(bool show = true);
        void showMemoryProfiler(bool show = true);

        // Drawing space
        void showDrawingSpace(bool show = true);
        void setDrawingTool(int tool); // 0: Brush, 1: Eraser, 2: Fill, 3: Line, 4: Rectangle, 5: Circle
        void setDrawingColor(const Color& color);
        void setDrawingSize(float size);
        void clearDrawing();
        void saveDrawing(const std::string& path);
        void loadDrawing(const std::string& path);

        // Sprite editing
        void showSpriteEditor(bool show = true);
        void loadSpriteForEditing(std::shared_ptr<Texture> texture);
        void createSpriteFromDrawing();
        void sliceSprite(int rows, int cols);
        void editSpritePixels();

        // Scene editing
        void showSceneView(bool show = true);
        void setSceneCamera(std::shared_ptr<Camera> camera);
        void focusOnSelected();
        void frameSelected();

        // Selection
        void selectGameObject(std::shared_ptr<GameObject> gameObject);
        void deselectGameObject();
        std::shared_ptr<GameObject> getSelectedGameObject() const { return m_selectedGameObject.lock(); }

        void selectMultiple(const std::vector<std::shared_ptr<GameObject>>& gameObjects);
        std::vector<std::shared_ptr<GameObject>> getSelectedGameObjects() const;

        // Tools
        enum class Tool {
            Select,
            Move,
            Rotate,
            Scale,
            Rect,
            Circle,
            Brush,
            Eraser,
            Fill,
            Text,
            Camera,
            Light,
            Particle,
            Terrain
        };

        void setTool(Tool tool) { m_currentTool = tool; }
        Tool getCurrentTool() const { return m_currentTool; }

        // Operations
        void duplicateSelected();
        void deleteSelected();
        void groupSelected();
        void ungroupSelected();

        void copySelected();
        void paste();
        void cut();

        void undo();
        void redo();
        bool canUndo() const;
        bool canRedo() const;

        // Layers
        void createLayer(const std::string& name);
        void deleteLayer(const std::string& name);
        void setActiveLayer(const std::string& name);
        std::string getActiveLayer() const;

        // Prefabs
        void createPrefab(std::shared_ptr<GameObject> gameObject, const std::string& path);
        void unpackPrefab(std::shared_ptr<GameObject> gameObject);
        void applyPrefab(std::shared_ptr<GameObject> gameObject);
        void revertPrefab(std::shared_ptr<GameObject> gameObject);

        // Script workspace
        void showScriptWorkspace(bool show = true);
        void createScript(const std::string& name, const std::string& language = "Lua");
        void editScript(const std::string& path);
        void compileScript(const std::string& path);
        void runScript(const std::string& path);
        void debugScript(const std::string& path);

        // Asset creation
        std::shared_ptr<Material> createNewMaterial(const std::string& name);
        std::shared_ptr<Shader> createNewShader(const std::string& name, const std::string& vertex,
            const std::string& fragment);
        std::shared_ptr<ParticleSystem> createNewParticleSystem(const std::string& name);
        std::shared_ptr<AnimationClip> createNewAnimation(const std::string& name);
        std::shared_ptr<Terrain2D> createNewTerrain(const std::string& name, int width, int height);
        std::shared_ptr<Tilemap> createNewTilemap(const std::string& name, int width, int height,
            int tileWidth, int tileHeight);

        // Build
        void buildProject(const std::string& platform = "", const std::string& configuration = "");
        void runBuild();
        void deployBuild(const std::string& target = "");

        // Settings
        void loadEditorSettings();
        void saveEditorSettings();
        void resetEditorSettings();

        // Theme
        void setTheme(const std::string& theme);
        std::string getTheme() const { return m_theme; }

        // Language
        void setLanguage(const std::string& language);
        std::string getLanguage() const { return m_language; }

        // Hotkeys
        void setHotkey(const std::string& action, const std::string& key);
        std::string getHotkey(const std::string& action) const;

        // Messages
        void showMessage(const std::string& title, const std::string& message,
            MessageType type = MessageType::Info);
        void showError(const std::string& title, const std::string& message);
        void showWarning(const std::string& title, const std::string& message);
        void showSuccess(const std::string& title, const std::string& message);

        // Dialogs
        bool showConfirmDialog(const std::string& title, const std::string& message);
        std::string showInputDialog(const std::string& title, const std::string& label,
            const std::string& defaultValue = "");
        std::string showFileDialog(const std::string& title, const std::string& filter,
            FileDialogMode mode = FileDialogMode::Open);
        std::string showColorDialog(const std::string& title, const Color& defaultColor);

        // Preview
        void previewAsset(const std::string& path);
        void stopPreview();

        // Recent projects
        void showRecentProjects();
        void clearRecentProjects();

        // Tutorial
        void showTutorial(const std::string& tutorial = "welcome");
        void showHelp();
        void showAbout();

        // Project events
        void onProjectLoaded();
        void onProjectSaved();
        void onProjectClosed();

    private:
        // State
        bool m_active = true;
        bool m_playMode = false;
        bool m_paused = false;

        // Windows
        bool m_showSceneHierarchy = true;
        bool m_showInspector = true;
        bool m_showProjectBrowser = true;
        bool m_showConsole = true;
        bool m_showSceneView = true;
        bool m_showGameView = false;
        bool m_showDrawingSpace = false;
        bool m_showSpriteEditor = false;
        bool m_showScriptWorkspace = false;
        bool m_showAnimationEditor = false;
        bool m_showParticleEditor = false;
        bool m_showMaterialEditor = false;
        bool m_showShaderEditor = false;
        bool m_showTilemapEditor = false;
        bool m_showTerrainEditor = false;
        bool m_showLightingEditor = false;
        bool m_showPostProcessingEditor = false;
        bool m_showPhysicsEditor = false;
        bool m_showAudioEditor = false;
        bool m_showNavigationEditor = false;
        bool m_showUIEditor = false;
        bool m_showBuildSettings = false;
        bool m_showPlayerSettings = false;
        bool m_showPackageManager = false;
        bool m_showVersionControl = false;
        bool m_showProfiler = false;
        bool m_showMemoryProfiler = false;

        // Selection
        std::weak_ptr<GameObject> m_selectedGameObject;
        std::vector<std::weak_ptr<GameObject>> m_selectedGameObjects;

        // Tools
        Tool m_currentTool = Tool::Select;

        // Drawing
        std::shared_ptr<Texture> m_drawingCanvas;
        std::vector<std::shared_ptr<Texture>> m_drawingHistory;
        int m_drawingHistoryIndex = -1;
        Color m_drawingColor = Color::White();
        float m_drawingSize = 5.0f;
        int m_drawingTool = 0; // 0: Brush

        // Sprite editing
        std::shared_ptr<Texture> m_editingTexture;
        std::shared_ptr<Sprite> m_editingSprite;

        // Script editing
        struct ScriptEditorState {
            std::string path;
            std::string content;
            bool modified = false;
            int cursorLine = 0;
            int cursorColumn = 0;
        };

        std::unordered_map<std::string, ScriptEditorState> m_scriptEditors;
        std::string m_currentScriptPath;

        // Scene camera
        std::shared_ptr<Camera> m_sceneCamera;

        // Layers
        std::vector<std::string> m_layers;
        std::string m_activeLayer = "Default";

        // History
        struct EditorState {
            std::string description;
            std::string data;
            time_t timestamp;
        };

        std::vector<EditorState> m_undoStack;
        std::vector<EditorState> m_redoStack;

        // Settings
        std::string m_theme = "Dark";
        std::string m_language = "en";
        std::unordered_map<std::string, std::string> m_hotkeys;

        // ImGui context
        ImGuiContext* m_imguiContext = nullptr;

        // Rendering methods
        void renderMenuBar();
        void renderToolbar();
        void renderStatusBar();

        void renderSceneHierarchy();
        void renderInspector();
        void renderProjectBrowser();
        void renderConsole();
        void renderSceneView();
        void renderGameView();
        void renderDrawingSpace();
        void renderSpriteEditor();
        void renderScriptWorkspace();
        void renderAnimationEditor();
        void renderParticleEditor();
        void renderMaterialEditor();
        void renderShaderEditor();
        void renderTilemapEditor();
        void renderTerrainEditor();
        void renderLightingEditor();
        void renderPostProcessingEditor();
        void renderPhysicsEditor();
        void renderAudioEditor();
        void renderNavigationEditor();
        void renderUIEditor();
        void renderBuildSettings();
        void renderPlayerSettings();
        void renderPackageManager();
        void renderVersionControl();
        void renderProfiler();
        void renderMemoryProfiler();

        // Utility methods
        void handleShortcuts();
        void handleDragDrop();
        void handleContextMenu();

        void saveState();
        void loadState(const EditorState& state);

        void updateDrawing(float deltaTime);
        void updateSpriteEditing(float deltaTime);
        void updateScriptEditing(float deltaTime);

        void applyDrawingTool(const Vector2& position, bool pressed);
        void applySpriteEditingTool(const Vector2& position, bool pressed);

        void syncWithEngine();

        void logMessage(const std::string& message, MessageType type = MessageType::Info);
        void clearMessages();

        void setStatus(const std::string& status, float duration = 3.0f);

        void loadTheme(const std::string& theme);
        void loadLanguage(const std::string& language);

        void createDefaultLayout();
        void saveLayout();
        void loadLayout(const std::string& name);

        void showWelcomeScreen();
        void hideWelcomeScreen();

        friend class Engine;
    };

} // namespace CmakeProject1