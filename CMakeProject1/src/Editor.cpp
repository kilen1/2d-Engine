#include "../include/Editor.h"
#include "../include/Engine.h"
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

    Editor::Editor() : selectedGameObject(nullptr), isProjectBrowserOpen(true) {}

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
        if (isProjectBrowserOpen) {
            renderProjectBrowser();
            return;
        }

        // Main menu bar
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Open Project")) {
                    isProjectBrowserOpen = true;
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
                ImGui::MenuItem("Scene Hierarchy", nullptr, &showSceneHierarchy);
                ImGui::MenuItem("Inspector", nullptr, &showInspector);
                ImGui::MenuItem("Game View", nullptr, &showGameView);
                ImGui::MenuItem("Project Browser", nullptr, &showProjectBrowser);
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Help")) {
                ImGui::MenuItem("About", nullptr, &showAboutWindow);
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }

        // Scene Hierarchy Window
        if (showSceneHierarchy) {
            ImGui::Begin("Scene Hierarchy", &showSceneHierarchy);
            ImGui::Text("Scene hierarchy will be here");
            ImGui::End();
        }

        // Inspector Window
        if (showInspector) {
            ImGui::Begin("Inspector", &showInspector);
            ImGui::Text("Inspector will be here");
            ImGui::End();
        }

        // Project Browser Window
        if (showProjectBrowser) {
            ImGui::Begin("Project Browser", &showProjectBrowser);
            ImGui::Text("Project browser will be here");
            ImGui::End();
        }

        // Game View Window
        if (showGameView) {
            ImGui::Begin("Game View", &showGameView, ImGuiWindowFlags_NoScrollbar);
            ImGui::Text("Game view will be here");
            ImGui::End();
        }

        // About Window
        if (showAboutWindow) {
            ImGui::Begin("About 2D Game Engine", &showAboutWindow, ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::Text("2D Game Engine v1.0.0");
            ImGui::End();
        }
    }

    void Editor::renderProjectBrowser() {
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        ImGui::Begin("Project Browser", nullptr,
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

        // Center content
        ImVec2 windowSize = ImGui::GetWindowSize();
        ImVec2 centerPos = ImVec2((windowSize.x - 400) * 0.5f, (windowSize.y - 300) * 0.5f);
        ImGui::SetCursorPos(centerPos);

        ImGui::BeginChild("ProjectBrowserChild", ImVec2(400, 300), true);

        // Title
        ImGui::SetCursorPosX((400 - ImGui::CalcTextSize("2D Game Engine").x) * 0.5f);
        ImGui::TextColored(ImVec4(0.0f, 0.7f, 1.0f, 1.0f), "2D GAME ENGINE");
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // Create New Project
        static char newProjectName[128] = "";
        ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "CREATE NEW PROJECT");
        ImGui::Spacing();
        ImGui::InputText("##ProjectName", newProjectName, IM_ARRAYSIZE(newProjectName));
        ImGui::Spacing();

        if (ImGui::Button("Create Project", ImVec2(380, 40))) {
            if (strlen(newProjectName) > 0) {
                // TODO: Implement project creation
                isProjectBrowserOpen = false;
                strcpy(newProjectName, "");
            }
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // Skip to editor button
        ImGui::Spacing();
        if (ImGui::Button("Skip to Editor", ImVec2(380, 40))) {
            isProjectBrowserOpen = false;
        }

        ImGui::EndChild();

        ImGui::End();
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

} // namespace CmakeProject1