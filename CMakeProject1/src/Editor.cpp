#include "Editor.h"
#include "Engine.h"
#include <GameObject.h>
#include <Transform.h>
#include <Scene.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_mixer.h>
#include <SDL_opengl.h>
#include <Window.h>


namespace CmakeProject1 {
#ifndef SDL_GLContext
    typedef void* SDL_GLContext;
#endif

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
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Поддержка Viewports (если включены)
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            // НЕ нужно восстанавливать фокус вручную — ImGui делает это сам
        }
    }

    void Editor::render() {
        // Create and setup docking layout
        static bool dockBuilder = false;
        static ImGuiID dockspace_id = 0;
        
        if (!dockBuilder) {
            // Create dockspace
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | 
                           ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

            // Begin main window
            ImGui::Begin("MainWindow", nullptr, window_flags);
            ImGui::PopStyleVar(2);

            // Create dockspace
            dockspace_id = ImGui::GetID("EngineDockspace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

            // Setup default layout
            ImGui::DockBuilderRemoveNode(dockspace_id); // Clear out existing layout
            ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_None); // Add empty node
            ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

            // Split into main area and bottom
            ImGuiID dock_main_id = dockspace_id;
            ImGuiID dock_bottom_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.25f, nullptr, &dock_main_id);

            // Split main area into left and right
            ImGuiID dock_left_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.25f, nullptr, &dock_main_id);
            ImGuiID dock_right_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.30f, nullptr, &dock_main_id);

            // Assign windows to dock spaces
            ImGui::DockBuilderDockWindow("Scene Hierarchy", dock_left_id);
            ImGui::DockBuilderDockWindow("Inspector", dock_right_id);
            ImGui::DockBuilderDockWindow("Project Browser", dock_bottom_id);
            ImGui::DockBuilderDockWindow("Game View", dock_main_id);
            
            dockBuilder = true;
        } else {
            // Main menu bar
            if (ImGui::BeginMainMenuBar()) {
                if (ImGui::BeginMenu("View")) {
                    ImGui::MenuItem("Scene Hierarchy", nullptr, &m_showSceneHierarchy);
                    ImGui::MenuItem("Inspector", nullptr, &m_showInspector);
                    ImGui::MenuItem("Project Browser", nullptr, &m_showProjectBrowser);
                    ImGui::MenuItem("Game View", nullptr, &m_showGameView);
                    ImGui::MenuItem("Scene View", nullptr, &m_showSceneView);
                    ImGui::MenuItem("Console", nullptr, &m_showConsole);
                    ImGui::EndMenu();
                }
                ImGui::EndMainMenuBar();
            }

            // Render docked windows
            if (m_showSceneHierarchy) {
                renderSceneHierarchy();
            }
            
            if (m_showInspector) {
                renderInspector();
            }
            
            if (m_showProjectBrowser) {
                renderProjectBrowser();
            }
            
            if (m_showGameView) {
                renderGameView();
            }
            
            if (m_showSceneView) {
                renderSceneView();
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
            
            ImGui::End(); // End MainWindow
        }
        
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
        
        // Toolbar for scene hierarchy
        if (ImGui::Button("Create Empty")) {
            // Create empty game object
        }
        ImGui::SameLine();
        if (ImGui::Button("Search")) {
            // Show search field
        }
        
        ImGui::Separator();
        
        // Scene hierarchy content
        auto engine = &Engine::getInstance();
        if (engine->getCurrentScene()) {
            auto gameObjects = engine->getCurrentScene()->getRootGameObjects();
            for (auto& go : gameObjects) {
                // Generate a unique ID for the tree node
                ImGui::PushID(go.get());
                
                if (ImGui::TreeNode(go->getName().c_str())) {
                    // Show child objects recursively
                    ImGui::Text("Position: (%.2f, %.2f)", 
                               go->getTransform()->getPosition().x, 
                               go->getTransform()->getPosition().y);
                    ImGui::Text("Rotation: %.2f", go->getTransform()->getRotation());
                    
                    // Check if this object was clicked
                    if (ImGui::IsItemClicked()) {
                        selectGameObject(go);
                    }
                    
                    ImGui::TreePop();
                } else {
                    // Check if the item without children was clicked
                    if (ImGui::IsItemClicked()) {
                        selectGameObject(go);
                    }
                }
                
                ImGui::PopID();
            }
        } else {
            ImGui::Text("No scene loaded");
        }
        
        // Right-click context menu
        if (ImGui::BeginPopupContextWindow()) {
            if (ImGui::MenuItem("Create Empty")) {
                // Create empty game object
            }
            if (ImGui::MenuItem("Duplicate")) {
                duplicateSelected();
            }
            if (ImGui::MenuItem("Delete")) {
                deleteSelected();
            }
            ImGui::EndPopup();
        }
        
        ImGui::End();
    }

    void Editor::renderInspector() {
        ImGui::Begin("Inspector", &m_showInspector);
        
        // Toolbar for inspector
        if (ImGui::Button("Add Component")) {
            // Show component selection popup
        }
        ImGui::SameLine();
        if (ImGui::Button("Reset")) {
            // Reset selected object to defaults
        }
        
        ImGui::Separator();
        
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
            
            // Add components section
            ImGui::Separator();
            ImGui::Text("Components");
            
            if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
                // Transform component fields are already shown above
            }
            
            if (ImGui::CollapsingHeader("Sprite Renderer")) {
                ImGui::Text("Sprite renderer properties would appear here");
            }
            
            if (ImGui::CollapsingHeader("Collider")) {
                ImGui::Text("Collider properties would appear here");
            }
            
            if (ImGui::CollapsingHeader("Script")) {
                ImGui::Text("Script properties would appear here");
            }
        } else {
            ImGui::Text("No object selected");
        }
        
        ImGui::End();
    }

    void Editor::renderProjectBrowser() {
        ImGui::Begin("Project Browser", &m_showProjectBrowser);
        
        // Toolbar for project browser
        if (ImGui::Button("Refresh")) {
            // Refresh project assets
        }
        ImGui::SameLine();
        if (ImGui::Button("New Folder")) {
            // Create new folder
        }
        ImGui::SameLine();
        if (ImGui::Button("Import")) {
            // Import asset
        }
        
        ImGui::Separator();
        
        // File tree view
        static ImGuiTextFilter filter;
        filter.Draw("Filter", -100.0f);
        
        ImGui::BeginChild("Project Files", ImVec2(0, 0), true);
        
        // Simple example of showing project structure
        if (ImGui::TreeNode("Assets")) {
            if (ImGui::TreeNode("Scenes")) {
                ImGui::BulletText("MainScene.scene");
                ImGui::BulletText("Level1.scene");
                ImGui::TreePop();
            }
            
            if (ImGui::TreeNode("Sprites")) {
                ImGui::BulletText("Player.png");
                ImGui::BulletText("Enemy.png");
                ImGui::BulletText("Background.png");
                ImGui::TreePop();
            }
            
            if (ImGui::TreeNode("Scripts")) {
                ImGui::BulletText("PlayerController.lua");
                ImGui::BulletText("GameManager.lua");
                ImGui::TreePop();
            }
            
            if (ImGui::TreeNode("Prefabs")) {
                ImGui::BulletText("Player.prefab");
                ImGui::BulletText("Enemy.prefab");
                ImGui::TreePop();
            }
            
            ImGui::TreePop();
        }
        
        if (ImGui::TreeNode("Resources")) {
            ImGui::BulletText("Fonts");
            ImGui::BulletText("Materials");
            ImGui::BulletText("Shaders");
            ImGui::TreePop();
        }
        
        ImGui::EndChild();
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
        
        // Toolbar for game view
        if (ImGui::Button(m_playMode ? "||" : ">")) {
            // Toggle play/pause
            auto engine = &Engine::getInstance();
            engine->togglePlayMode();
            m_playMode = !m_playMode;
        }
        ImGui::SameLine();
        if (ImGui::Button("O")) {
            // Stop button
            auto engine = &Engine::getInstance();
            engine->stop();
            m_playMode = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("[]")) {
            // Step button (when paused)
            auto engine = &Engine::getInstance();
        }
        
        ImGui::SameLine();
        ImGui::Checkbox("Gizmos", &m_showGizmos);
        
        ImGui::SameLine();
        ImGui::Checkbox("Stats", &m_showStats);
        
        ImGui::SameLine();
        {
            static const char* resolutionItems[] = { "50%", "75%", "100%", "125%", "150%" };
            static int resolutionIdx = 2; // 100%
            ImGui::Combo("##Resolution", &resolutionIdx, resolutionItems, IM_ARRAYSIZE(resolutionItems));
        }
        
        ImGui::Separator();
        
        // Get the available size for the game view
        ImVec2 availSize = ImGui::GetContentRegionAvail();
        
        // Calculate aspect ratio preserving size (keeping 16:9 aspect ratio as example)
        float aspectRatio = 16.0f / 9.0f;
        float width = availSize.x;
        float height = width / aspectRatio;
        
        if (height > availSize.y) {
            height = availSize.y;
            width = height * aspectRatio;
        }
        
        // Draw the game view area with black border
        ImVec2 startPos = ImGui::GetCursorScreenPos();
        startPos.x += (availSize.x - width) * 0.5f;  // Center horizontally
        
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        ImVec2 p1 = startPos;
        ImVec2 p2 = ImVec2(p1.x + width, p1.y + height);
        
        // Draw black border
        draw_list->AddRect(p1, p2, IM_COL32(0, 0, 0, 255));
        
        // Add placeholder text inside the game view
        const char* placeholderText = m_playMode ? "GAME RUNNING" : "GAME VIEW";
        ImVec2 textSize = ImGui::CalcTextSize(placeholderText);
        ImVec2 textPos = ImVec2(
            p1.x + (width - textSize.x) * 0.5f,
            p1.y + (height - textSize.y) * 0.5f
        );
        draw_list->AddText(textPos, IM_COL32(255, 255, 255, 255), placeholderText);
        
        // Make the area clickable
        ImGui::SetCursorScreenPos(p1);
        ImGui::InvisibleButton("game_view_area", ImVec2(width, height));
        
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