#include "Editor.h"
#include "Engine.h"
#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <memory>
#include <filesystem>

// Prevent redefinition of ImVec2 and other imgui types
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "imgui.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"


namespace CmakeProject1 {

    Editor::Editor() : selectedGameObject(nullptr) {
        // Initialize editor window states
        showSceneHierarchyWindow = true;
        showInspectorWindow = true;
        showProjectBrowserWindow = true;
        showConsoleWindow = true;
    }

    Editor::~Editor() {
        shutdown();
    }

    bool Editor::initialize(SDL_Window* window, SDL_Renderer* renderer) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        // Временно отключаем docking, если версия ImGui старая
        // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();

        // Custom dark theme
        ImVec4* colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
        colors[ImGuiCol_Button] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
        colors[ImGuiCol_Header] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);

        // Setup Platform/Renderer bindings
        if (!ImGui_ImplSDL2_InitForOpenGL(window, nullptr)) {
            std::cerr << "Failed to initialize ImGui for SDL2" << std::endl;
            return false;
        }

        if (!ImGui_ImplOpenGL3_Init("#version 130")) {
            std::cerr << "Failed to initialize ImGui for OpenGL3" << std::endl;
            return false;
        }

        return true;
    }

    void Editor::beginFrame() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
    }

    void Editor::renderUI() {
        // Main menu bar
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Open Project")) {
                    // Handle project opening
                }
                if (ImGui::MenuItem("Save Project")) {
                    // TODO: Implement save
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Exit")) {
                    Engine::getInstance().shutdown();
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("GameObject")) {
                if (ImGui::MenuItem("Create Empty")) {
                    // TODO: Implement
                }
                if (ImGui::MenuItem("Sprite")) {
                    // TODO: Implement
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Window")) {
                ImGui::MenuItem("Scene Hierarchy", nullptr, &showSceneHierarchyWindow);
                ImGui::MenuItem("Inspector", nullptr, &showInspectorWindow);
                ImGui::MenuItem("Project Browser", nullptr, &showProjectBrowserWindow);
                ImGui::MenuItem("Console", nullptr, &showConsoleWindow);
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Help")) {
                ImGui::MenuItem("About", nullptr, &showSettingsWindow);
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }

        // Scene Hierarchy Window
        if (showSceneHierarchyWindow) {
            ImGui::Begin("Scene Hierarchy", &showSceneHierarchyWindow);
            ImGui::Text("Scene hierarchy will be here");
            ImGui::End();
        }

        // Inspector Window
        if (showInspectorWindow) {
            ImGui::Begin("Inspector", &showInspectorWindow);
            ImGui::Text("Inspector will be here");
            ImGui::End();
        }

        // Project Browser Window
        if (showProjectBrowserWindow) {
            ImGui::Begin("Project Browser", &showProjectBrowserWindow);
            ImGui::Text("Project browser will be here");
            ImGui::End();
        }

        // Console Window
        if (showConsoleWindow) {
            ImGui::Begin("Console", &showConsoleWindow);
            ImGui::Text("Console will be here");
            ImGui::End();
        }

        // Settings Window (About)
        if (showSettingsWindow) {
            ImGui::Begin("About 2D Game Engine", &showSettingsWindow, ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::Text("2D Game Engine v1.0.0");
            ImGui::End();
        }
    }



    void Editor::endFrame() {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void Editor::shutdown() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
    }

    void Editor::render() {
        renderUI();
    }

    void Editor::processEvent(const SDL_Event& event) {
        // Handle editor events
    }

    void Editor::showSceneHierarchy(bool show) {
        showSceneHierarchyWindow = show;
    }

    void Editor::showInspector(bool show) {
        showInspectorWindow = show;
    }

    void Editor::showProjectBrowser(bool show) {
        showProjectBrowserWindow = show;
    }

    void Editor::showConsole(bool show) {
        showConsoleWindow = show;
    }

    void Editor::showAnimationEditor(bool show) {
        showAnimationWindow = show;
    }

    void Editor::showParticleEditor(bool show) {
        showParticleWindow = show;
    }

    void Editor::showMaterialEditor(bool show) {
        showMaterialWindow = show;
    }

    void Editor::showShaderEditor(bool show) {
        showShaderWindow = show;
    }

    void Editor::showTerrainEditor(bool show) {
        showTerrainWindow = show;
    }

    void Editor::showLightingEditor(bool show) {
        showLightingWindow = show;
    }

    void Editor::showPostProcessingEditor(bool show) {
        showPostProcessingWindow = show;
    }

    void Editor::showPhysicsEditor(bool show) {
        showPhysicsWindow = show;
    }

    void Editor::showAudioEditor(bool show) {
        showAudioWindow = show;
    }

    void Editor::showNavigationEditor(bool show) {
        showNavigationWindow = show;
    }

    void Editor::showUIEditor(bool show) {
        showUIWindow = show;
    }

    void Editor::showScriptEditor(bool show) {
        showScriptWindow = show;
    }

    void Editor::showBuildSettings(bool show) {
        showBuildWindow = show;
    }

    void Editor::showPlayerSettings(bool show) {
        showSettingsWindow = show;
    }

    void Editor::showPackageManager(bool show) {
        showSettingsWindow = show;
    }

    void Editor::showVersionControl(bool show) {
        showSettingsWindow = show;
    }

    void Editor::showProfiler(bool show) {
        showProfilerWindow = show;
    }

    void Editor::showMemoryProfiler(bool show) {
        showProfilerWindow = show;
    }

    void Editor::showFrameDebugger(bool show) {
        showSettingsWindow = show;
    }

    void Editor::showNetworkDebugger(bool show) {
        showNetworkWindow = show;
    }

    void Editor::showGraphicsSettings(bool show) {
        showSettingsWindow = show;
    }

    void Editor::showInputSettings(bool show) {
        showSettingsWindow = show;
    }

    void Editor::showAudioSettings(bool show) {
        showSettingsWindow = show;
    }

    void Editor::selectTool(ToolType tool) {
        currentTool = tool;
    }

    void Editor::selectGameObject(GameObject* gameObject) {
        selectedGameObject = gameObject;
    }

    void Editor::deselectGameObject() {
        selectedGameObject = nullptr;
    }

    void Editor::selectMultiple(const std::vector<GameObject*>& gameObjects) {
        selectedGameObjects = gameObjects;
    }

    const std::vector<GameObject*>& Editor::getSelectedGameObjects() const {
        return selectedGameObjects;
    }

    void Editor::duplicateSelected() {
        // TODO: Implement duplication
    }

    void Editor::deleteSelected() {
        // TODO: Implement deletion
    }

    void Editor::groupSelected() {
        // TODO: Implement grouping
    }

    void Editor::ungroupSelected() {
        // TODO: Implement ungrouping
    }

    void Editor::copySelected() {
        // TODO: Implement copying
    }

    void Editor::paste() {
        // TODO: Implement pasting
    }

    void Editor::cut() {
        // TODO: Implement cutting
    }

    void Editor::undo() {
        // TODO: Implement undo
    }

    void Editor::redo() {
        // TODO: Implement redo
    }

    bool Editor::canUndo() const {
        return !undoStack.empty();
    }

    bool Editor::canRedo() const {
        return !redoStack.empty();
    }

    void Editor::focusOnSelected() {
        // TODO: Implement focusing
    }

    void Editor::frameSelected() {
        // TODO: Implement framing
    }

    void Editor::alignViewToSelected() {
        // TODO: Implement alignment
    }

    void Editor::alignSelectedToView() {
        // TODO: Implement alignment
    }

    void Editor::createLayer(const std::string& name) {
        layers.push_back(name);
    }

    void Editor::deleteLayer(const std::string& name) {
        auto it = std::find(layers.begin(), layers.end(), name);
        if (it != layers.end()) {
            layers.erase(it);
        }
    }

    void Editor::setActiveLayer(const std::string& name) {
        activeLayer = name;
    }

    std::string Editor::getActiveLayer() const {
        return activeLayer;
    }

    void Editor::takeSnapshot() {
        // TODO: Implement snapshot
    }

    void Editor::restoreSnapshot() {
        // TODO: Implement snapshot restoration
    }

    void Editor::clearSnapshots() {
        // TODO: Implement snapshot clearing
    }

    void Editor::createPrefab(GameObject* gameObject, const std::string& path) {
        // TODO: Implement prefab creation
    }

    void Editor::unpackPrefab(GameObject* gameObject) {
        // TODO: Implement prefab unpacking
    }

    void Editor::applyPrefab(GameObject* gameObject) {
        // TODO: Implement prefab application
    }

    void Editor::revertPrefab(GameObject* gameObject) {
        // TODO: Implement prefab reversion
    }

    void Editor::setTimelineTime(float time) {
        timelineTime = time;
    }

    void Editor::playTimeline() {
        timelinePlaying = true;
    }

    void Editor::pauseTimeline() {
        timelinePlaying = false;
    }

    void Editor::stopTimeline() {
        timelinePlaying = false;
        timelineTime = 0.0f;
    }

    bool Editor::isTimelinePlaying() const {
        return timelinePlaying;
    }

    void Editor::recordKeyframe(const std::string& property, const std::any& value) {
        // TODO: Implement keyframe recording
    }

    void Editor::deleteKeyframe(const std::string& property, float time) {
        // TODO: Implement keyframe deletion
    }

    void Editor::modifyKeyframe(const std::string& property, float time, const std::any& value) {
        // TODO: Implement keyframe modification
    }

    void Editor::createCurve(const std::string& name, const std::vector<ImVec2>& points) {
        curves[name] = points;
    }

    void Editor::editCurve(const std::string& name, const std::vector<ImVec2>& points) {
        curves[name] = points;
    }

    void Editor::deleteCurve(const std::string& name) {
        curves.erase(name);
    }

    void Editor::createNodeGraph(const std::string& name) {
        // TODO: Implement node graph creation
    }

    void Editor::addNode(const std::string& graph, const std::string& type, const ImVec2& position) {
        // TODO: Implement node addition
    }

    void Editor::connectNodes(const std::string& graph, int outputNode, int outputPin, 
                             int inputNode, int inputPin) {
        // TODO: Implement node connection
    }

    void Editor::deleteNode(const std::string& graph, int nodeId) {
        // TODO: Implement node deletion
    }

    void Editor::createShader(const std::string& name, ShaderType type) {
        // TODO: Implement shader creation
    }

    void Editor::compileShader(const std::string& name) {
        // TODO: Implement shader compilation
    }

    void Editor::hotReloadShader(const std::string& name) {
        // TODO: Implement shader hot reloading
    }

    void Editor::createMaterial(const std::string& name) {
        // TODO: Implement material creation
    }

    void Editor::editMaterial(const std::string& name, MaterialProperty property, 
                             const std::any& value) {
        // TODO: Implement material editing
    }

    void Editor::saveMaterial(const std::string& name, const std::string& path) {
        // TODO: Implement material saving
    }

    void Editor::loadMaterial(const std::string& path) {
        // TODO: Implement material loading
    }

    void Editor::importTexture(const std::string& path) {
        // TODO: Implement texture import
    }

    void Editor::editTexture(const std::string& name, TextureOperation operation) {
        // TODO: Implement texture editing
    }

    void Editor::generateTexture(const std::string& name, TextureType type, 
                                int width, int height) {
        // TODO: Implement texture generation
    }

    void Editor::createSprite(const std::string& name, const std::string& texturePath) {
        // TODO: Implement sprite creation
    }

    void Editor::editSprite(const std::string& name, SpriteEditOperation operation) {
        // TODO: Implement sprite editing
    }

    void Editor::sliceSprite(const std::string& name, int rows, int cols) {
        // TODO: Implement sprite slicing
    }

    void Editor::packSprites(const std::vector<std::string>& spritePaths, 
                            const std::string& outputPath) {
        // TODO: Implement sprite packing
    }

    void Editor::createAnimation(const std::string& name) {
        // TODO: Implement animation creation
    }

    void Editor::addAnimationFrame(const std::string& name, const std::string& spritePath, 
                                  float duration) {
        // TODO: Implement animation frame addition
    }

    void Editor::editAnimationCurve(const std::string& name, AnimationProperty property, 
                                   const std::vector<ImVec2>& points) {
        // TODO: Implement animation curve editing
    }

    void Editor::bakeAnimation(const std::string& name) {
        // TODO: Implement animation baking
    }

    void Editor::createParticleSystem(const std::string& name) {
        // TODO: Implement particle system creation
    }

    void Editor::editParticleSystem(const std::string& name, ParticleProperty property, 
                                   const std::any& value) {
        // TODO: Implement particle system editing
    }

    void Editor::previewParticleSystem(const std::string& name) {
        // TODO: Implement particle system preview
    }

    void Editor::exportParticleSystem(const std::string& name, const std::string& path) {
        // TODO: Implement particle system export
    }

    void Editor::createTerrain(const std::string& name, int width, int height, 
                              float heightScale) {
        // TODO: Implement terrain creation
    }

    void Editor::editTerrain(const std::string& name, TerrainTool tool, 
                            const Vector2& position, float strength) {
        // TODO: Implement terrain editing
    }

    void Editor::paintTerrain(const std::string& name, int textureLayer, 
                             const Vector2& position, float radius) {
        // TODO: Implement terrain painting
    }

    void Editor::generateTerrain(const std::string& name, TerrainGeneratorType type) {
        // TODO: Implement terrain generation
    }

    void Editor::createWater(const std::string& name, const Vector2& size) {
        // TODO: Implement water creation
    }

    void Editor::editWater(const std::string& name, WaterProperty property, 
                          const std::any& value) {
        // TODO: Implement water editing
    }

    void Editor::simulateWater(const std::string& name) {
        // TODO: Implement water simulation
    }

    void Editor::createLight(const std::string& name, LightType type) {
        // TODO: Implement light creation
    }

    void Editor::editLight(const std::string& name, LightProperty property, 
                          const std::any& value) {
        // TODO: Implement light editing
    }

    void Editor::bakeLighting() {
        // TODO: Implement lighting baking
    }

    void Editor::clearBakedLighting() {
        // TODO: Implement baked lighting clearing
    }

    void Editor::createPostProcessingProfile(const std::string& name) {
        // TODO: Implement post-processing profile creation
    }

    void Editor::editPostProcessing(const std::string& name, PostProcessingEffect effect, 
                                   const std::any& value) {
        // TODO: Implement post-processing editing
    }

    void Editor::applyPostProcessing(const std::string& name) {
        // TODO: Implement post-processing application
    }

    void Editor::createPhysicsMaterial(const std::string& name) {
        // TODO: Implement physics material creation
    }

    void Editor::editPhysicsMaterial(const std::string& name, PhysicsMaterialProperty property, 
                                    const std::any& value) {
        // TODO: Implement physics material editing
    }

    void Editor::simulatePhysics(bool simulate) {
        // TODO: Implement physics simulation
    }

    void Editor::stepPhysics() {
        // TODO: Implement physics stepping
    }

    void Editor::bakeNavMesh() {
        // TODO: Implement navmesh baking
    }

    void Editor::editNavMesh(NavMeshEditOperation operation, const Vector2& position, 
                            float radius) {
        // TODO: Implement navmesh editing
    }

    void Editor::testNavMesh(const Vector2& start, const Vector2& end) {
        // TODO: Implement navmesh testing
    }

    void Editor::createUICanvas(const std::string& name) {
        // TODO: Implement UI canvas creation
    }

    void Editor::createUIElement(const std::string& canvas, UIElementType type, 
                                const std::string& name) {
        // TODO: Implement UI element creation
    }

    void Editor::editUIElement(const std::string& canvas, const std::string& element, 
                              UIProperty property, const std::any& value) {
        // TODO: Implement UI element editing
    }

    void Editor::layoutUI(const std::string& canvas, UILayoutType type) {
        // TODO: Implement UI layout
    }

    void Editor::createScript(const std::string& name, ScriptLanguage language) {
        // TODO: Implement script creation
    }

    void Editor::editScript(const std::string& name) {
        // TODO: Implement script editing
    }

    void Editor::compileScript(const std::string& name) {
        // TODO: Implement script compilation
    }

    void Editor::debugScript(const std::string& name) {
        // TODO: Implement script debugging
    }

    void Editor::runScript(const std::string& name) {
        // TODO: Implement script running
    }

    void Editor::buildProject(const std::string& platform, const std::string& configuration) {
        // TODO: Implement project building
    }

    void Editor::runBuild() {
        // TODO: Implement build running
    }

    void Editor::deployBuild(const std::string& target) {
        // TODO: Implement build deployment
    }

    void Editor::installPackage(const std::string& name, const std::string& version) {
        // TODO: Implement package installation
    }

    void Editor::uninstallPackage(const std::string& name) {
        // TODO: Implement package uninstallation
    }

    void Editor::updatePackage(const std::string& name) {
        // TODO: Implement package updating
    }

    void Editor::listPackages() {
        // TODO: Implement package listing
    }

    void Editor::commitChanges(const std::string& message) {
        // TODO: Implement commit changes
    }

    void Editor::pushChanges() {
        // TODO: Implement push changes
    }

    void Editor::pullChanges() {
        // TODO: Implement pull changes
    }

    void Editor::showHistory() {
        // TODO: Implement show history
    }

    void Editor::revertChanges(const std::string& revision) {
        // TODO: Implement revert changes
    }

    void Editor::startProfiling() {
        // TODO: Implement profiling start
    }

    void Editor::stopProfiling() {
        // TODO: Implement profiling stop
    }

    void Editor::saveProfile(const std::string& path) {
        // TODO: Implement profile saving
    }

    void Editor::loadProfile(const std::string& path) {
        // TODO: Implement profile loading
    }

    void Editor::startNetworkServer() {
        // TODO: Implement network server start
    }

    void Editor::stopNetworkServer() {
        // TODO: Implement network server stop
    }

    void Editor::connectToServer(const std::string& address) {
        // TODO: Implement server connection
    }

    void Editor::disconnectFromServer() {
        // TODO: Implement server disconnection
    }

    void Editor::loadEditorSettings() {
        // TODO: Implement editor settings loading
    }

    void Editor::saveEditorSettings() {
        // TODO: Implement editor settings saving
    }

    void Editor::resetEditorSettings() {
        // TODO: Implement editor settings reset
    }

    void Editor::setLanguage(const std::string& language) {
        currentLanguage = language;
    }

    std::string Editor::getLanguage() const {
        return currentLanguage;
    }

    void Editor::setTheme(const std::string& theme) {
        currentTheme = theme;
    }

    std::string Editor::getTheme() const {
        return currentTheme;
    }

    void Editor::setHotkey(const std::string& action, const std::string& key) {
        hotkeys[action] = key;
    }

    std::string Editor::getHotkey(const std::string& action) const {
        auto it = hotkeys.find(action);
        return it != hotkeys.end() ? it->second : "";
    }

    void Editor::recordMacro() {
        recordingMacro = true;
    }

    void Editor::stopRecordingMacro() {
        recordingMacro = false;
    }

    void Editor::playMacro(const std::string& name) {
        // TODO: Implement macro playing
    }

    void Editor::saveMacro(const std::string& name, const std::string& path) {
        // TODO: Implement macro saving
    }

    void Editor::loadMacro(const std::string& path) {
        // TODO: Implement macro loading
    }

    void Editor::loadPlugin(const std::string& path) {
        // TODO: Implement plugin loading
    }

    void Editor::unloadPlugin(const std::string& name) {
        // TODO: Implement plugin unloading
    }

    void Editor::reloadPlugin(const std::string& name) {
        // TODO: Implement plugin reloading
    }

    void Editor::showTutorial(const std::string& tutorial) {
        // TODO: Implement tutorial showing
    }

    void Editor::showHelp() {
        // TODO: Implement help showing
    }

    void Editor::showAbout() {
        showSettingsWindow = true;
    }

    void Editor::showMessage(const std::string& title, const std::string& message, 
                            MessageType type) {
        // TODO: Implement message showing
    }

    void Editor::showError(const std::string& title, const std::string& message) {
        // TODO: Implement error showing
    }

    void Editor::showWarning(const std::string& title, const std::string& message) {
        // TODO: Implement warning showing
    }

    void Editor::showSuccess(const std::string& title, const std::string& message) {
        // TODO: Implement success showing
    }

    bool Editor::showConfirmDialog(const std::string& title, const std::string& message) {
        // TODO: Implement confirm dialog
        return false;
    }

    std::string Editor::showInputDialog(const std::string& title, const std::string& label, 
                                       const std::string& defaultValue) {
        // TODO: Implement input dialog
        return defaultValue;
    }

    std::string Editor::showFileDialog(const std::string& title, const std::string& filter, 
                                      FileDialogMode mode) {
        // TODO: Implement file dialog
        return "";
    }

    std::string Editor::showColorDialog(const std::string& title, const Color& defaultColor) {
        // TODO: Implement color dialog
        return "";
    }

    std::string Editor::showFontDialog(const std::string& title, const std::string& defaultFont) {
        // TODO: Implement font dialog
        return defaultFont;
    }

    void Editor::previewAsset(const std::string& path) {
        // TODO: Implement asset preview
    }

    void Editor::stopPreview() {
        // TODO: Implement preview stopping
    }

    void Editor::quickSearch() {
        // TODO: Implement quick search
    }

    void Editor::quickReplace() {
        // TODO: Implement quick replace
    }

    void Editor::quickOpen() {
        // TODO: Implement quick open
    }

    void Editor::quickRun() {
        // TODO: Implement quick run
    }

    void Editor::showRecentProjects() {
        // TODO: Implement recent projects showing
    }

    void Editor::clearRecentProjects() {
        // TODO: Implement recent projects clearing
    }

    void Editor::createBackup() {
        // TODO: Implement backup creation
    }

    void Editor::restoreBackup(const std::string& backupId) {
        // TODO: Implement backup restoration
    }

    void Editor::deleteBackup(const std::string& backupId) {
        // TODO: Implement backup deletion
    }

    void Editor::exportScene(const std::string& path, ExportFormat format) {
        // TODO: Implement scene export
    }

    void Editor::exportModel(const std::string& path, ExportFormat format) {
        // TODO: Implement model export
    }

    void Editor::exportAnimation(const std::string& path, ExportFormat format) {
        // TODO: Implement animation export
    }

    void Editor::exportTexture(const std::string& path, ExportFormat format) {
        // TODO: Implement texture export
    }

    void Editor::importAsset(const std::string& path) {
        // TODO: Implement asset import
    }

    void Editor::importScene(const std::string& path) {
        // TODO: Implement scene import
    }

    void Editor::importModel(const std::string& path) {
        // TODO: Implement model import
    }

    void Editor::optimizeScene() {
        // TODO: Implement scene optimization
    }

    void Editor::optimizeTextures() {
        // TODO: Implement texture optimization
    }

    void Editor::optimizeMeshes() {
        // TODO: Implement mesh optimization
    }

    void Editor::optimizeAnimations() {
        // TODO: Implement animation optimization
    }

    void Editor::showStatistics() {
        // TODO: Implement statistics showing
    }

    void Editor::showDebugInfo() {
        // TODO: Implement debug info showing
    }

    void Editor::showWireframe() {
        debugInfo.showWireframe = !debugInfo.showWireframe;
    }

    void Editor::showNormals() {
        debugInfo.showNormals = !debugInfo.showNormals;
    }

    void Editor::showBounds() {
        debugInfo.showBounds = !debugInfo.showBounds;
    }

    void Editor::showColliders() {
        debugInfo.showColliders = !debugInfo.showColliders;
    }

    void Editor::showLights() {
        // TODO: Implement lights showing
    }

    void Editor::showCameras() {
        // TODO: Implement cameras showing
    }

} // namespace CmakeProject1