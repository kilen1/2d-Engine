#include "../include/Editor.h"
#include "../include/Engine.h"
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

namespace CmakeProject1 {

    Editor::Editor() {
        // Initialize editor with default settings
    }

    Editor::~Editor() {
        shutdown();
    }

    bool Editor::initialize(SDL_Window* window, SDL_GLContext context) {
        // Initialize ImGui
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        
        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        
        // Setup Platform/Renderer bindings
        ImGui_ImplSDL2_InitForOpenGL(window, context);
        ImGui_ImplOpenGL3_Init("#version 330");
        
        return true;
    }

    void Editor::shutdown() {
        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
    }

    void Editor::beginFrame() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
    }

    void Editor::endFrame() {
        ImGui::Render();
    }

    void Editor::render() {
        // Render various editor windows
        if (m_showSceneView) {
            renderSceneView();
        }
        
        if (m_showGameView) {
            renderGameView();
        }
        
        if (m_showSceneHierarchy) {
            renderSceneHierarchy();
        }
        
        if (m_showInspector) {
            renderInspector();
        }
        
        if (m_showProjectBrowser) {
            renderProjectBrowser();
        }
        
        if (m_showConsole) {
            renderConsole();
        }
        
        if (m_showDrawingSpace) {
            renderDrawingSpace();
        }
        
        if (m_showSpriteEditor) {
            renderSpriteEditor();
        }
        
        if (m_showScriptWorkspace) {
            renderScriptWorkspace();
        }
        
        // Render all other windows as needed...
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void Editor::processEvent(const SDL_Event& event) {
        ImGui_ImplSDL2_ProcessEvent(&event);
    }

    // Window visibility methods
    void Editor::showSceneHierarchy(bool show) { m_showSceneHierarchy = show; }
    void Editor::showInspector(bool show) { m_showInspector = show; }
    void Editor::showProjectBrowser(bool show) { m_showProjectBrowser = show; }
    void Editor::showConsole(bool show) { m_showConsole = show; }
    void Editor::showSceneView(bool show) { m_showSceneView = show; }
    void Editor::showDrawingSpace(bool show) { m_showDrawingSpace = show; }
    void Editor::showSpriteEditor(bool show) { m_showSpriteEditor = show; }
    void Editor::showScriptWorkspace(bool show) { m_showScriptWorkspace = show; }

    // Rendering implementations
    void Editor::renderSceneHierarchy() {
        ImGui::Begin("Scene Hierarchy", &m_showSceneHierarchy);
        
        // Render scene hierarchy content
        auto engine = &Engine::getInstance();
        if (engine->getCurrentScene()) {
            auto gameObjects = engine->getCurrentScene()->getRootGameObjects();
            for (auto& go : gameObjects) {
                if (ImGui::TreeNode(go->getName().c_str())) {
                    // Show child objects recursively
                    ImGui::Text("Position: (%.2f, %.2f)", 
                               go->getTransform()->getPosition().x, 
                               go->getTransform()->getPosition().y);
                    ImGui::Text("Rotation: %.2f", go->getTransform()->getRotation());
                    ImGui::TreePop();
                }
            }
        }
        
        ImGui::End();
    }

    void Editor::renderInspector() {
        ImGui::Begin("Inspector", &m_showInspector);
        
        // Render selected game object properties
        if (auto selectedGO = m_selectedGameObject.lock()) {
            char nameBuffer[256];
            strcpy(nameBuffer, selectedGO->getName().c_str());
            
            if (ImGui::InputText("Name", nameBuffer, sizeof(nameBuffer))) {
                selectedGO->setName(nameBuffer);
            }
            
            ImGui::Separator();
            
            // Show transform properties
            auto transform = selectedGO->getTransform();
            if (transform) {
                Vector2 pos = transform->getPosition();
                float posX = pos.x, posY = pos.y;
                
                if (ImGui::DragFloat2("Position", &posX)) {
                    transform->setPosition(posX, posY);
                }
                
                float rotation = transform->getRotation();
                if (ImGui::DragFloat("Rotation", &rotation)) {
                    transform->setRotation(rotation);
                }
                
                Vector2 scale = transform->getScale();
                float scaleX = scale.x, scaleY = scale.y;
                if (ImGui::DragFloat2("Scale", &scaleX)) {
                    transform->setScale(scaleX, scaleY);
                }
            }
        }
        
        ImGui::End();
    }

    void Editor::renderProjectBrowser() {
        ImGui::Begin("Project Browser", &m_showProjectBrowser);
        ImGui::Text("Project assets and files would appear here");
        ImGui::End();
    }

    void Editor::renderConsole() {
        ImGui::Begin("Console", &m_showConsole);
        ImGui::Text("Log messages would appear here");
        ImGui::End();
    }

    void Editor::renderSceneView() {
        ImGui::Begin("Scene View", &m_showSceneView);
        ImGui::Text("Scene view content would appear here");
        ImGui::End();
    }

    void Editor::renderGameView() {
        ImGui::Begin("Game View", &m_showGameView);
        ImGui::Text("Game view content would appear here");
        ImGui::End();
    }

    void Editor::renderDrawingSpace() {
        ImGui::Begin("Drawing Space", &m_showDrawingSpace);
        ImGui::Text("Drawing tools and canvas would appear here");
        ImGui::End();
    }

    void Editor::renderSpriteEditor() {
        ImGui::Begin("Sprite Editor", &m_showSpriteEditor);
        ImGui::Text("Sprite editing tools would appear here");
        ImGui::End();
    }

    void Editor::renderScriptWorkspace() {
        ImGui::Begin("Script Workspace", &m_showScriptWorkspace);
        ImGui::Text("Script editing interface would appear here");
        ImGui::End();
    }

    void Editor::selectGameObject(std::shared_ptr<GameObject> gameObject) {
        m_selectedGameObject = gameObject;
    }

    void Editor::deselectGameObject() {
        m_selectedGameObject.reset();
    }

    std::vector<std::shared_ptr<GameObject>> Editor::getSelectedGameObjects() const {
        std::vector<std::shared_ptr<GameObject>> result;
        for (const auto& weakGO : m_selectedGameObjects) {
            if (auto go = weakGO.lock()) {
                result.push_back(go);
            }
        }
        return result;
    }

    void Editor::duplicateSelected() {
        if (auto selected = m_selectedGameObject.lock()) {
            auto engine = &Engine::getInstance();
            auto duplicated = engine->getCurrentScene()->instantiate(selected);
            if (duplicated) {
                duplicated->getTransform()->translate(Vector2(1.0f, 0.0f)); // Offset slightly
                selectGameObject(duplicated);
            }
        }
    }

    void Editor::deleteSelected() {
        if (auto selected = m_selectedGameObject.lock()) {
            auto engine = &Engine::getInstance();
            if (engine->getCurrentScene()) {
                engine->getCurrentScene()->destroy(selected);
                deselectGameObject();
            }
        }
    }

    void Editor::undo() {
        if (!m_undoStack.empty()) {
            // Pop from undo stack and push to redo stack
            m_redoStack.push_back(m_undoStack.back());
            m_undoStack.pop_back();
        }
    }

    void Editor::redo() {
        if (!m_redoStack.empty()) {
            // Pop from redo stack and push to undo stack
            m_undoStack.push_back(m_redoStack.back());
            m_redoStack.pop_back();
        }
    }

    bool Editor::canUndo() const {
        return !m_undoStack.empty();
    }

    bool Editor::canRedo() const {
        return !m_redoStack.empty();
    }

    void Editor::createLayer(const std::string& name) {
        auto it = std::find(m_layers.begin(), m_layers.end(), name);
        if (it == m_layers.end()) {
            m_layers.push_back(name);
        }
    }

    void Editor::deleteLayer(const std::string& name) {
        m_layers.erase(std::remove(m_layers.begin(), m_layers.end(), name), m_layers.end());
    }

    void Editor::setActiveLayer(const std::string& name) {
        auto it = std::find(m_layers.begin(), m_layers.end(), name);
        if (it != m_layers.end()) {
            m_activeLayer = name;
        }
    }

    std::string Editor::getActiveLayer() const {
        return m_activeLayer;
    }

    void Editor::loadEditorSettings() {
        // Load editor preferences from file
    }

    void Editor::saveEditorSettings() {
        // Save editor preferences to file
    }

    void Editor::setTheme(const std::string& theme) {
        m_theme = theme;
        // Apply theme changes
    }

    void Editor::setLanguage(const std::string& language) {
        m_language = language;
        // Apply language changes
    }

    void Editor::setHotkey(const std::string& action, const std::string& key) {
        m_hotkeys[action] = key;
    }

    std::string Editor::getHotkey(const std::string& action) const {
        auto it = m_hotkeys.find(action);
        if (it != m_hotkeys.end()) {
            return it->second;
        }
        return "";
    }

} // namespace CmakeProject1