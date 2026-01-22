#pragma once

// Forward declarations for imgui types
struct ImVec2;
struct ImVec4;
namespace ImGui { }
namespace ImGuizmo { }
namespace imnodes { }
namespace ImSequencer { }
namespace ImCurveEdit { }
class TextEditor;

#include <SDL2/SDL.h>
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include <functional>
#include <filesystem>
#include <any>

// Include core types
#include "Core/Types.h"
#include "Editor/EditorTypes.h"
#include "Editor/EditorClasses.h"

// Forward declarations for needed types
namespace CmakeProject1 {
    class GameObject;
    struct Vector2;
    struct Vector3;
    struct Color;
    
    // Forward declare enum classes
    enum class ShaderType;
    enum class MaterialProperty;
    enum class TextureOperation;
    enum class TextureType;
    enum class SpriteEditOperation;
    enum class AnimationProperty;
    enum class ParticleProperty;
    enum class TerrainTool;
    enum class TerrainGeneratorType;
    enum class WaterProperty;
    enum class LightType;
    enum class LightProperty;
    enum class PostProcessingEffect;
    enum class PhysicsMaterialProperty;
    enum class NavMeshEditOperation;
    enum class UIElementType;
    enum class UIProperty;
    enum class UILayoutType;
    enum class ScriptLanguage;
    enum class MessageType;
    enum class FileDialogMode;
    enum class ExportFormat;
    
    // Forward declare other editor classes
    class EditorTool;
    enum class EditorState;
    class NodeGraph;
    class ShaderEditor;
    class MaterialEditor;
    class TextureEditor;
    class SpriteEditor;
    class AnimationEditor;
    class ParticleEditor;
    class TerrainEditor;
    class WaterEditor;
    class LightEditor;
    class PostProcessingEditor;
    class PhysicsEditor;
    class NavigationEditor;
    class UIEditor;
    class ScriptEditor;
    class BuildSettings;
    struct PackageInfo;
    class VersionControl;
    class Profiler;
    class NetworkEditor;
    struct EditorSettings;
    struct Macro;
    class Plugin;
    struct Tutorial;
    struct Message;
    class Dialog;
    class PreviewWindow;
    class QuickSearch;
    struct ProjectHistory;
    struct Backup;
    struct EditorStatistics;
    struct DebugInfo;
}

namespace fs = std::filesystem;

namespace CmakeProject1 {

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
        
        // Состояние
        void setActive(bool active) { m_isActive = active; }
        bool isActive() const { return m_isActive; }
        
        void setPlayMode(bool play) { m_isPlayMode = play; }
        bool isPlayMode() const { return m_isPlayMode; }
        
        // Окна редактора
        void showSceneHierarchy(bool show = true);
        void showInspector(bool show = true);
        void showProjectBrowser(bool show = true);
        void showConsole(bool show = true);
        void showAnimationEditor(bool show = true);
        void showParticleEditor(bool show = true);
        void showMaterialEditor(bool show = true);
        void showShaderEditor(bool show = true);
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
        void showFrameDebugger(bool show = true);
        void showNetworkDebugger(bool show = true);
        void showGraphicsSettings(bool show = true);
        void showInputSettings(bool show = true);
        void showAudioSettings(bool show = true);
        
        // Инструменты
        void selectTool(ToolType tool);
        ToolType getCurrentTool() const { return currentTool; }
        
        enum class ToolType {
            Select,
            Move,
            Rotate,
            Scale,
            Rect,
            Circle,
            Polygon,
            Brush,
            Eraser,
            Fill,
            Text,
            Measure,
            Camera,
            Light,
            Particle,
            Terrain,
            Vegetation,
            Water,
            Path,
            NavMesh,
            Trigger,
            Volume
        };
        
        // Выделение
        void selectGameObject(GameObject* gameObject);
        void deselectGameObject();
        GameObject* getSelectedGameObject() const { return selectedGameObject; }
        
        void selectMultiple(const std::vector<GameObject*>& gameObjects);
        const std::vector<GameObject*>& getSelectedGameObjects() const { return selectedGameObjects; }
        
        // Операции
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
        
        // Камеры
        void focusOnSelected();
        void frameSelected();
        void alignViewToSelected();
        void alignSelectedToView();
        
        // Слои
        void createLayer(const std::string& name);
        void deleteLayer(const std::string& name);
        void setActiveLayer(const std::string& name);
        std::string getActiveLayer() const;
        
        // Снимки
        void takeSnapshot();
        void restoreSnapshot();
        void clearSnapshots();
        
        // Префабы
        void createPrefab(GameObject* gameObject, const std::string& path);
        void unpackPrefab(GameObject* gameObject);
        void applyPrefab(GameObject* gameObject);
        void revertPrefab(GameObject* gameObject);
        
        // Таймлайн
        void setTimelineTime(float time);
        float getTimelineTime() const { return timelineTime; }
        
        void playTimeline();
        void pauseTimeline();
        void stopTimeline();
        bool isTimelinePlaying() const { return timelinePlaying; }
        
        void recordKeyframe(const std::string& property, const std::any& value);
        void deleteKeyframe(const std::string& property, float time);
        void modifyKeyframe(const std::string& property, float time, const std::any& value);
        
        // Кривые
        void createCurve(const std::string& name, const std::vector<ImVec2>& points);
        void editCurve(const std::string& name, const std::vector<ImVec2>& points);
        void deleteCurve(const std::string& name);
        
        // Ноды
        void createNodeGraph(const std::string& name);
        void addNode(const std::string& graph, const std::string& type, const ImVec2& position);
        void connectNodes(const std::string& graph, int outputNode, int outputPin, 
                         int inputNode, int inputPin);
        void deleteNode(const std::string& graph, int nodeId);
        
        // Шейдеры
        void createShader(const std::string& name, ShaderType type);
        void compileShader(const std::string& name);
        void hotReloadShader(const std::string& name);
        
        // Материалы
        void createMaterial(const std::string& name);
        void editMaterial(const std::string& name, MaterialProperty property, 
                         const std::any& value);
        void saveMaterial(const std::string& name, const std::string& path);
        void loadMaterial(const std::string& path);
        
        // Текстуры
        void importTexture(const std::string& path);
        void editTexture(const std::string& name, TextureOperation operation);
        void generateTexture(const std::string& name, TextureType type, 
                            int width, int height);
        
        // Спрайты
        void createSprite(const std::string& name, const std::string& texturePath);
        void editSprite(const std::string& name, SpriteEditOperation operation);
        void sliceSprite(const std::string& name, int rows, int cols);
        void packSprites(const std::vector<std::string>& spritePaths, 
                        const std::string& outputPath);
        
        // Анимации
        void createAnimation(const std::string& name);
        void addAnimationFrame(const std::string& name, const std::string& spritePath, 
                              float duration);
        void editAnimationCurve(const std::string& name, AnimationProperty property, 
                               const std::vector<ImVec2>& points);
        void bakeAnimation(const std::string& name);
        
        // Частицы
        void createParticleSystem(const std::string& name);
        void editParticleSystem(const std::string& name, ParticleProperty property, 
                               const std::any& value);
        void previewParticleSystem(const std::string& name);
        void exportParticleSystem(const std::string& name, const std::string& path);
        
        // Террайн
        void createTerrain(const std::string& name, int width, int height, 
                          float heightScale);
        void editTerrain(const std::string& name, TerrainTool tool, 
                        const Vector2& position, float strength);
        void paintTerrain(const std::string& name, int textureLayer, 
                         const Vector2& position, float radius);
        void generateTerrain(const std::string& name, TerrainGeneratorType type);
        
        // Вода
        void createWater(const std::string& name, const Vector2& size);
        void editWater(const std::string& name, WaterProperty property, 
                      const std::any& value);
        void simulateWater(const std::string& name);
        
        // Освещение
        void createLight(const std::string& name, LightType type);
        void editLight(const std::string& name, LightProperty property, 
                      const std::any& value);
        void bakeLighting();
        void clearBakedLighting();
        
        // Пост-обработка
        void createPostProcessingProfile(const std::string& name);
        void editPostProcessing(const std::string& name, PostProcessingEffect effect, 
                               const std::any& value);
        void applyPostProcessing(const std::string& name);
        
        // Физика
        void createPhysicsMaterial(const std::string& name);
        void editPhysicsMaterial(const std::string& name, PhysicsMaterialProperty property, 
                                const std::any& value);
        void simulatePhysics(bool simulate);
        void stepPhysics();
        
        // Навигация
        void bakeNavMesh();
        void editNavMesh(NavMeshEditOperation operation, const Vector2& position, 
                        float radius);
        void testNavMesh(const Vector2& start, const Vector2& end);
        
        // UI
        void createUICanvas(const std::string& name);
        void createUIElement(const std::string& canvas, UIElementType type, 
                            const std::string& name);
        void editUIElement(const std::string& canvas, const std::string& element, 
                          UIProperty property, const std::any& value);
        void layoutUI(const std::string& canvas, UILayoutType type);
        
        // Скрипты
        void createScript(const std::string& name, ScriptLanguage language);
        void editScript(const std::string& name);
        void compileScript(const std::string& name);
        void debugScript(const std::string& name);
        void runScript(const std::string& name);
        
        // Сборка
        void buildProject(const std::string& platform, const std::string& configuration);
        void runBuild();
        void deployBuild(const std::string& target);
        
        // Пакеты
        void installPackage(const std::string& name, const std::string& version = "latest");
        void uninstallPackage(const std::string& name);
        void updatePackage(const std::string& name);
        void listPackages();
        
        // Контроль версий
        void commitChanges(const std::string& message);
        void pushChanges();
        void pullChanges();
        void showHistory();
        void revertChanges(const std::string& revision);
        
        // Профилирование
        void startProfiling();
        void stopProfiling();
        void saveProfile(const std::string& path);
        void loadProfile(const std::string& path);
        
        // Сети
        void startNetworkServer();
        void stopNetworkServer();
        void connectToServer(const std::string& address);
        void disconnectFromServer();
        
        // Настройки
        void loadEditorSettings();
        void saveEditorSettings();
        void resetEditorSettings();
        
        // Язык
        void setLanguage(const std::string& language);
        std::string getLanguage() const;
        
        // Темы
        void setTheme(const std::string& theme);
        std::string getTheme() const;
        
        // Горячие клавиши
        void setHotkey(const std::string& action, const std::string& key);
        std::string getHotkey(const std::string& action) const;
        
        // Макросы
        void recordMacro();
        void stopRecordingMacro();
        void playMacro(const std::string& name);
        void saveMacro(const std::string& name, const std::string& path);
        void loadMacro(const std::string& path);
        
        // Плагины
        void loadPlugin(const std::string& path);
        void unloadPlugin(const std::string& name);
        void reloadPlugin(const std::string& name);
        
        // Обучающие материалы
        void showTutorial(const std::string& tutorial);
        void showHelp();
        void showAbout();
        
        // Сообщения
        void showMessage(const std::string& title, const std::string& message, 
                        MessageType type = MessageType::Info);
        void showError(const std::string& title, const std::string& message);
        void showWarning(const std::string& title, const std::string& message);
        void showSuccess(const std::string& title, const std::string& message);
        
        // Диалоги
        bool showConfirmDialog(const std::string& title, const std::string& message);
        std::string showInputDialog(const std::string& title, const std::string& label, 
                                   const std::string& defaultValue = "");
        std::string showFileDialog(const std::string& title, const std::string& filter, 
                                  FileDialogMode mode = FileDialogMode::Open);
        std::string showColorDialog(const std::string& title, const Color& defaultColor);
        std::string showFontDialog(const std::string& title, const std::string& defaultFont);
        
        // Предпросмотр
        void previewAsset(const std::string& path);
        void stopPreview();
        
        // Быстрые действия
        void quickSearch();
        void quickReplace();
        void quickOpen();
        void quickRun();
        
        // История
        void showRecentProjects();
        void clearRecentProjects();
        
        // Резервное копирование
        void createBackup();
        void restoreBackup(const std::string& backupId);
        void deleteBackup(const std::string& backupId);
        
        // Экспорт
        void exportScene(const std::string& path, ExportFormat format);
        void exportModel(const std::string& path, ExportFormat format);
        void exportAnimation(const std::string& path, ExportFormat format);
        void exportTexture(const std::string& path, ExportFormat format);
        
        // Импорт
        void importAsset(const std::string& path);
        void importScene(const std::string& path);
        void importModel(const std::string& path);
        
        // Оптимизация
        void optimizeScene();
        void optimizeTextures();
        void optimizeMeshes();
        void optimizeAnimations();
        
        // Статистика
        void showStatistics();
        
        // Дебаг
        void showDebugInfo();
        void showWireframe();
        void showNormals();
        void showBounds();
        void showColliders();
        void showLights();
        void showCameras();
        
    private:
        // Состояние
        bool m_isActive = true;
        bool m_isPlayMode = false;
        bool isPaused = false;
        
        // Окна
        bool showSceneHierarchyWindow = true;
        bool showInspectorWindow = true;
        bool showProjectBrowserWindow = true;
        bool showConsoleWindow = true;
        bool showAnimationWindow = false;
        bool showParticleWindow = false;
        bool showMaterialWindow = false;
        bool showShaderWindow = false;
        bool showTerrainWindow = false;
        bool showLightingWindow = false;
        bool showPostProcessingWindow = false;
        bool showPhysicsWindow = false;
        bool showAudioWindow = false;
        bool showNavigationWindow = false;
        bool showUIWindow = false;
        bool showScriptWindow = false;
        bool showBuildWindow = false;
        bool showSettingsWindow = false;
        bool showProfilerWindow = false;
        bool showNetworkWindow = false;
        
        // Выделение
        GameObject* selectedGameObject = nullptr;
        std::vector<GameObject*> selectedGameObjects;
        
        // Инструменты
        ToolType currentTool = ToolType::Select;
        std::unordered_map<ToolType, std::unique_ptr<EditorTool>> tools;
        
        // Слои
        std::vector<std::string> layers;
        std::string activeLayer = "Default";
        
        // История
        std::vector<EditorState> undoStack;
        std::vector<EditorState> redoStack;
        
        // Таймлайн
        float timelineTime = 0.0f;
        bool timelinePlaying = false;
        float timelineSpeed = 1.0f;
        
        // Кривые
        std::unordered_map<std::string, std::vector<ImVec2>> curves;
        
        // Ноды
        std::unordered_map<std::string, NodeGraph> nodeGraphs;
        
        // Шейдеры
        std::unordered_map<std::string, ShaderEditor> shaderEditors;
        
        // Материалы
        std::unordered_map<std::string, MaterialEditor> materialEditors;
        
        // Текстуры
        std::unordered_map<std::string, TextureEditor> textureEditors;
        
        // Спрайты
        std::unordered_map<std::string, SpriteEditor> spriteEditors;
        
        // Анимации
        std::unordered_map<std::string, AnimationEditor> animationEditors;
        
        // Частицы
        std::unordered_map<std::string, ParticleEditor> particleEditors;
        
        // Террайн
        std::unordered_map<std::string, TerrainEditor> terrainEditors;
        
        // Вода
        std::unordered_map<std::string, WaterEditor> waterEditors;
        
        // Освещение
        std::unordered_map<std::string, LightEditor> lightEditors;
        
        // Пост-обработка
        std::unordered_map<std::string, PostProcessingEditor> postProcessingEditors;
        
        // Физика
        std::unordered_map<std::string, PhysicsEditor> physicsEditors;
        
        // Навигация
        NavigationEditor navigationEditor;
        
        // UI
        std::unordered_map<std::string, UIEditor> uiEditors;
        
        // Скрипты
        std::unordered_map<std::string, ScriptEditor> scriptEditors;
        TextEditor textEditor;
        
        // Сборка
        BuildSettings buildSettings;
        
        // Пакеты
        std::vector<PackageInfo> packages;
        
        // Контроль версий
        VersionControl vcs;
        
        // Профилирование
        Profiler profiler;
        
        // Сети
        NetworkEditor networkEditor;
        
        // Настройки
        EditorSettings settings;
        
        // Язык
        std::string currentLanguage = "en";
        
        // Темы
        std::string currentTheme = "Dark";
        
        // Горячие клавиши
        std::unordered_map<std::string, std::string> hotkeys;
        
        // Макросы
        std::unordered_map<std::string, Macro> macros;
        bool recordingMacro = false;
        Macro currentMacro;
        
        // Плагины
        std::unordered_map<std::string, Plugin> plugins;
        
        // Обучающие материалы
        std::unordered_map<std::string, Tutorial> tutorials;
        
        // Сообщения
        std::vector<Message> messages;
        
        // Диалоги
        std::unique_ptr<Dialog> activeDialog;
        
        // Предпросмотр
        std::unique_ptr<PreviewWindow> previewWindow;
        
        // Быстрый поиск
        QuickSearch quickSearchTool;
        
        // История
        std::vector<ProjectHistory> recentProjects;
        
        // Резервные копии
        std::vector<Backup> backups;
        
        // Статистика
        EditorStatistics statistics;
        
        // Дебаг
        DebugInfo debugInfo;
        
        // Приватные методы
        void renderMenuBar();
        void renderToolbar();
        void renderStatusBar();
        
        void renderSceneHierarchy();
        void renderInspector();
        void renderProjectBrowser();
        void renderConsole();
        void renderAnimationEditor();
        void renderParticleEditor();
        void renderMaterialEditor();
        void renderShaderEditor();
        void renderTerrainEditor();
        void renderLightingEditor();
        void renderPostProcessingEditor();
        void renderPhysicsEditor();
        void renderAudioEditor();
        void renderNavigationEditor();
        void renderUIEditor();
        void renderScriptEditor();
        void renderBuildSettings();
        void renderPlayerSettings();
        void renderPackageManager();
        void renderVersionControl();
        void renderProfiler();
        void renderMemoryProfiler();
        void renderFrameDebugger();
        void renderNetworkDebugger();
        void renderGraphicsSettings();
        void renderInputSettings();
        void renderAudioSettings();
        
        void handleShortcuts();
        void handleDragDrop();
        void handleContextMenu();
        
        void saveState();
        void loadState(const EditorState& state);
        
        void updateTimeline(float deltaTime);
        void updatePreview(float deltaTime);
        void updateStatistics(float deltaTime);
        
        void applyTool();
        void applyGizmo();
        
        void syncWithEngine();
        
        void showAssetPreview(const std::string& path);
        void hideAssetPreview();
        
        void logMessage(const std::string& message, MessageType type = MessageType::Info);
        void clearMessages();
        
        void setStatus(const std::string& status, float duration = 3.0f);
        
        void loadTheme(const std::string& theme);
        void loadLanguage(const std::string& language);
        
        void createDefaultLayout();
        void saveLayout();
        void loadLayout(const std::string& name);
        
        void backupProject();
        void restoreProject(const std::string& backupId);
        
        void exportProject(const std::string& path);
        void importProject(const std::string& path);
        
        void optimizeProject();
        
        void showWelcomeScreen();
        void hideWelcomeScreen();
        
        void checkForUpdates();
        void installUpdate();
        
        void showCredits();
        
        friend class EditorTool;
        friend class Gizmo;
        friend class PreviewWindow;
    };
}