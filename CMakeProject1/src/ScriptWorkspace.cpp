#include "Editor.h"
#include "Engine.h"
#include <fstream>

namespace CmakeProject1 {

    void Editor::renderScriptWorkspace() {
        if (!m_showScriptWorkspace) return;

        ImGui::Begin("Script Workspace", &m_showScriptWorkspace,
            ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse);

        // Menu bar
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("New Script")) {
                    auto name = showInputDialog("New Script", "Script name:");
                    if (!name.empty()) {
                        createScript(name);
                    }
                }
                if (ImGui::MenuItem("Open...", "Ctrl+O")) {
                    auto path = showFileDialog("Open Script",
                        "Lua scripts (*.lua)\0*.lua\0All files (*.*)\0*.*\0");
                    if (!path.empty()) {
                        editScript(path);
                    }
                }
                if (ImGui::MenuItem("Save", "Ctrl+S")) {
                    saveCurrentScript();
                }
                if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) {
                    auto path = showFileDialog("Save Script As",
                        "Lua scripts (*.lua)\0*.lua\0");
                    if (!path.empty()) {
                        saveScriptAs(path);
                    }
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Close", "Ctrl+W")) {
                    m_showScriptWorkspace = false;
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Edit")) {
                if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
                if (ImGui::MenuItem("Redo", "Ctrl+Y")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Cut", "Ctrl+X")) {}
                if (ImGui::MenuItem("Copy", "Ctrl+C")) {}
                if (ImGui::MenuItem("Paste", "Ctrl+V")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Find", "Ctrl+F")) {}
                if (ImGui::MenuItem("Replace", "Ctrl+H")) {}
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Script")) {
                if (ImGui::MenuItem("Compile", "F5")) {
                    if (!m_currentScriptPath.empty()) {
                        compileScript(m_currentScriptPath);
                    }
                }
                if (ImGui::MenuItem("Run", "F6")) {
                    if (!m_currentScriptPath.empty()) {
                        runScript(m_currentScriptPath);
                    }
                }
                if (ImGui::MenuItem("Debug", "F7")) {
                    if (!m_currentScriptPath.empty()) {
                        debugScript(m_currentScriptPath);
                    }
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Attach to GameObject")) {
                    attachScriptToGameObject();
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("View")) {
                ImGui::MenuItem("Line Numbers", nullptr, &m_showLineNumbers);
                ImGui::MenuItem("Syntax Highlighting", nullptr, &m_syntaxHighlighting);
                ImGui::MenuItem("Word Wrap", nullptr, &m_wordWrap);
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Help")) {
                if (ImGui::MenuItem("API Reference")) {
                    showHelp();
                }
                if (ImGui::MenuItem("Examples")) {
                    showExamples();
                }
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        // Script list
        ImGui::BeginChild("ScriptList", ImVec2(200, 0), true);

        auto project = Engine::getInstance().getProjectManager()->getCurrentProject();
        if (project) {
            auto scripts = project->getScripts();
            for (const auto& script : scripts) {
                std::string name = std::filesystem::path(script).filename().string();
                if (ImGui::Selectable(name.c_str(), m_currentScriptPath == script)) {
                    editScript(script);
                }

                // Context menu
                if (ImGui::BeginPopupContextItem()) {
                    if (ImGui::MenuItem("Open")) {
                        editScript(script);
                    }
                    if (ImGui::MenuItem("Rename")) {
                        renameScript(script);
                    }
                    if (ImGui::MenuItem("Delete")) {
                        deleteScript(script);
                    }
                    ImGui::EndPopup();
                }
            }
        }

        ImGui::EndChild();

        ImGui::SameLine();

        // Editor
        ImGui::BeginChild("Editor", ImVec2(0, 0), true);

        if (!m_currentScriptPath.empty()) {
            auto& state = m_scriptEditors[m_currentScriptPath];

            // Toolbar
            if (ImGui::Button("Compile (F5)")) {
                compileScript(m_currentScriptPath);
            }
            ImGui::SameLine();
            if (ImGui::Button("Run (F6)")) {
                runScript(m_currentScriptPath);
            }
            ImGui::SameLine();
            if (ImGui::Button("Debug (F7)")) {
                debugScript(m_currentScriptPath);
            }
            ImGui::SameLine();
            ImGui::Text(state.modified ? "*" : " ");

            ImGui::Separator();

            // Text editor
            ImGui::InputTextMultiline("##source", &state.content,
                ImVec2(-1, -1),
                ImGuiInputTextFlags_AllowTabInput |
                (m_wordWrap ? 0 : ImGuiInputTextFlags_NoHorizontalScroll));

            // Status bar
            ImGui::Separator();
            ImGui::Text("Line: %d, Column: %d | %s",
                state.cursorLine, state.cursorColumn,
                state.modified ? "Modified" : "Saved");
        }
        else {
            ImGui::Text("No script opened. Create a new script or open an existing one.");
            if (ImGui::Button("New Script")) {
                auto name = showInputDialog("New Script", "Script name:");
                if (!name.empty()) {
                    createScript(name);
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Open Script")) {
                auto path = showFileDialog("Open Script",
                    "Lua scripts (*.lua)\0*.lua\0All files (*.*)\0*.*\0");
                if (!path.empty()) {
                    editScript(path);
                }
            }
        }

        ImGui::EndChild();

        ImGui::End();
    }

    void Editor::createScript(const std::string& name, const std::string& language) {
        auto project = Engine::getInstance().getProjectManager()->getCurrentProject();
        if (!project) return;

        std::string scriptsPath = project->getPath() + "/Scripts";
        std::string scriptPath = scriptsPath + "/" + name + ".lua";

        // Create default script content
        std::string content = R"(--- 
-- )" + name + R"(.lua
-- Auto-generated script
--

local )" + name + R"( = {}

function )" + name + R"(:Start()
    -- Called when the script starts
    self.transform = self:GetComponent("Transform")
    self.sprite = self:GetComponent("SpriteRenderer")
end

function )" + name + R"(:Update(deltaTime)
    -- Called every frame
end

function )" + name + R"(:OnCollisionEnter(collision)
    -- Called when collision starts
end

function )" + name + R"(:OnCollisionExit(collision)
    -- Called when collision ends
end

-- Public properties (visible in inspector)
)" + name + R"(.speed = 5.0
)" + name + R"(.jumpForce = 10.0
)" + name + R"(.health = 100

return )" + name;

        // Save file
        std::ofstream file(scriptPath);
        if (file.is_open()) {
            file << content;
            file.close();

            // Add to project
            project->addRecentFile(scriptPath);

            // Open in editor
            editScript(scriptPath);

            showMessage("Success", "Script created: " + name);
        }
        else {
            showError("Error", "Failed to create script: " + scriptPath);
        }
    }

    void Editor::editScript(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            showError("Error", "Failed to open script: " + path);
            return;
        }

        std::string content((std::istreambuf_iterator<char>(file)),
            std::istreambuf_iterator<char>());
        file.close();

        m_scriptEditors[path] = { path, content, false, 0, 0 };
        m_currentScriptPath = path;
        m_showScriptWorkspace = true;
    }

    void Editor::compileScript(const std::string& path) {
        auto& engine = Engine::getInstance();
        auto scriptEngine = engine.getScriptEngine();

        if (scriptEngine->compileScript(path)) {
            showMessage("Success", "Script compiled successfully: " +
                std::filesystem::path(path).filename().string());

            // Hot reload if game is running
            if (engine.isPlaying()) {
                scriptEngine->hotReload(path);
            }
        }
        else {
            showError("Compilation Failed", "Failed to compile script: " + path);
        }
    }

    void Editor::runScript(const std::string& path) {
        auto& engine = Engine::getInstance();
        auto scriptEngine = engine.getScriptEngine();

        if (scriptEngine->runScript(path)) {
            showMessage("Success", "Script executed successfully");
        }
        else {
            showError("Execution Failed", "Failed to execute script: " + path);
        }
    }

    void Editor::saveCurrentScript() {
        if (m_currentScriptPath.empty()) return;

        auto it = m_scriptEditors.find(m_currentScriptPath);
        if (it == m_scriptEditors.end()) return;

        auto& state = it->second;

        std::ofstream file(m_currentScriptPath);
        if (file.is_open()) {
            file << state.content;
            file.close();

            state.modified = false;
            showMessage("Success", "Script saved: " +
                std::filesystem::path(m_currentScriptPath).filename().string());
        }
        else {
            showError("Error", "Failed to save script: " + m_currentScriptPath);
        }
    }

    void Editor::attachScriptToGameObject() {
        if (m_currentScriptPath.empty()) return;

        auto selected = getSelectedGameObject();
        if (!selected) {
            showWarning("No Selection", "Please select a GameObject first");
            return;
        }

        auto scriptComp = selected->addComponent<ScriptComponent>();
        scriptComp->setScript(m_currentScriptPath);

        showMessage("Success", "Script attached to " + selected->getName());
    }

} // namespace CmakeProject1