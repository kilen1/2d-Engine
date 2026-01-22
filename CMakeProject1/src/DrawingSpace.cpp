#include "Engine.h"
#include "Editor.h"
#include "Texture.h"
#include <GL/glew.h>
#include "imgui.h"

namespace CmakeProject1 {

    void Editor::renderDrawingSpace() {
        if (!m_showDrawingSpace) return;

        ImGui::Begin("Drawing Space", &m_showDrawingSpace, ImGuiWindowFlags_MenuBar);

        // Menu bar
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("New")) {
                    clearDrawing();
                }
                if (ImGui::MenuItem("Open...")) {
                    auto path = showFileDialog("Open Drawing", "Image files (*.png;*.jpg;*.bmp)\0*.png;*.jpg;*.bmp\0");
                    if (!path.empty()) {
                        loadDrawing(path);
                    }
                }
                if (ImGui::MenuItem("Save", "Ctrl+S")) {
                    auto path = showFileDialog("Save Drawing", "PNG Image (*.png)\0*.png\0");
                    if (!path.empty()) {
                        saveDrawing(path);
                    }
                }
                if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) {
                    auto path = showFileDialog("Save Drawing As", "PNG Image (*.png)\0*.png\0");
                    if (!path.empty()) {
                        saveDrawing(path);
                    }
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Create Sprite")) {
                    createSpriteFromDrawing();
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Exit")) {
                    m_showDrawingSpace = false;
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Edit")) {
                if (ImGui::MenuItem("Undo", "Ctrl+Z", false, m_drawingHistoryIndex > 0)) {
                    if (m_drawingHistoryIndex > 0) {
                        m_drawingHistoryIndex--;
                        m_drawingCanvas = m_drawingHistory[m_drawingHistoryIndex];
                    }
                }
                if (ImGui::MenuItem("Redo", "Ctrl+Y", false,
                    m_drawingHistoryIndex < (int)m_drawingHistory.size() - 1)) {
                    if (m_drawingHistoryIndex < (int)m_drawingHistory.size() - 1) {
                        m_drawingHistoryIndex++;
                        m_drawingCanvas = m_drawingHistory[m_drawingHistoryIndex];
                    }
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Clear")) {
                    clearDrawing();
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Tools")) {
                if (ImGui::MenuItem("Brush", nullptr, m_drawingTool == 0)) m_drawingTool = 0;
                if (ImGui::MenuItem("Eraser", nullptr, m_drawingTool == 1)) m_drawingTool = 1;
                if (ImGui::MenuItem("Fill", nullptr, m_drawingTool == 2)) m_drawingTool = 2;
                if (ImGui::MenuItem("Line", nullptr, m_drawingTool == 3)) m_drawingTool = 3;
                if (ImGui::MenuItem("Rectangle", nullptr, m_drawingTool == 4)) m_drawingTool = 4;
                if (ImGui::MenuItem("Circle", nullptr, m_drawingTool == 5)) m_drawingTool = 5;
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("View")) {
                ImGui::MenuItem("Grid", nullptr, &m_showGrid);
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        // Toolbar
        ImGui::BeginChild("Toolbar", ImVec2(80, 0), true);

        // Tools
        if (ImGui::Selectable("Brush", m_drawingTool == 0)) m_drawingTool = 0;
        if (ImGui::Selectable("Eraser", m_drawingTool == 1)) m_drawingTool = 1;
        if (ImGui::Selectable("Fill", m_drawingTool == 2)) m_drawingTool = 2;
        if (ImGui::Selectable("Line", m_drawingTool == 3)) m_drawingTool = 3;
        if (ImGui::Selectable("Rectangle", m_drawingTool == 4)) m_drawingTool = 4;
        if (ImGui::Selectable("Circle", m_drawingTool == 5)) m_drawingTool = 5;

        ImGui::Separator();

        // Color picker
        float col[3] = { m_drawingColor.r, m_drawingColor.g, m_drawingColor.b };
        if (ImGui::ColorEdit3("Color", col, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel)) {
            m_drawingColor = Color(col[0], col[1], col[2], 1.0f);
        }

        // Brush size
        ImGui::Text("Size:");
        ImGui::SliderFloat("##size", &m_drawingSize, 1.0f, 100.0f, "%.0f");

        // Opacity
        static float opacity = 1.0f;
        ImGui::Text("Opacity:");
        ImGui::SliderFloat("##opacity", &opacity, 0.0f, 1.0f, "%.2f");

        ImGui::EndChild();

        ImGui::SameLine();

        // Canvas
        ImGui::BeginChild("Canvas", ImVec2(0, 0), true,
            ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoMove);

        // Get canvas region
        ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
        ImVec2 canvas_size = ImGui::GetContentRegionAvail();

        // Create canvas if needed
        if (!m_drawingCanvas) {
            m_drawingCanvas = std::make_shared<Texture>(1024, 1024);
            m_drawingCanvas->clear(Color(0, 0, 0, 0));
            m_drawingHistory.clear();
            m_drawingHistory.push_back(m_drawingCanvas);
            m_drawingHistoryIndex = 0;
        }

        // Draw canvas
        ImGui::Image((void*)(intptr_t)m_drawingCanvas->getID(),
            ImVec2(m_drawingCanvas->getWidth(), m_drawingCanvas->getHeight()));

        // Handle drawing
        if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
            ImVec2 mouse_pos = ImGui::GetMousePos();
            Vector2 local_pos = Vector2(
                (mouse_pos.x - canvas_pos.x) / canvas_size.x * m_drawingCanvas->getWidth(),
                (mouse_pos.y - canvas_pos.y) / canvas_size.y * m_drawingCanvas->getHeight()
            );

            applyDrawingTool(local_pos, true);
        }

        ImGui::EndChild();

        ImGui::End();
    }

    void Editor::applyDrawingTool(const Vector2& position, bool pressed) {
        if (!m_drawingCanvas) return;

        static Vector2 last_position;
        static bool drawing = false;

        if (pressed && !drawing) {
            // Start new drawing
            drawing = true;
            last_position = position;

            // Save state for undo
            auto newCanvas = std::make_shared<Texture>(*m_drawingCanvas);
            m_drawingHistory.resize(m_drawingHistoryIndex + 1);
            m_drawingHistory.push_back(newCanvas);
            m_drawingHistoryIndex++;
            m_drawingCanvas = newCanvas;
        }

        if (!pressed && drawing) {
            drawing = false;
            return;
        }

        if (!drawing) return;

        // Apply tool
        switch (m_drawingTool) {
        case 0: // Brush
            drawLine(last_position, position, m_drawingColor, m_drawingSize);
            break;
        case 1: // Eraser
            drawLine(last_position, position, Color(0, 0, 0, 0), m_drawingSize);
            break;
        case 2: // Fill
            floodFill(position, m_drawingColor);
            break;
        case 3: // Line
            // Preview line
            break;
        case 4: // Rectangle
            // Preview rectangle
            break;
        case 5: // Circle
            // Preview circle
            break;
        }

        last_position = position;
    }

    void Editor::drawLine(const Vector2& start, const Vector2& end, const Color& color, float size) {
        if (!m_drawingCanvas) return;

        // Bresenham's line algorithm
        int x0 = (int)start.x;
        int y0 = (int)start.y;
        int x1 = (int)end.x;
        int y1 = (int)end.y;

        int dx = abs(x1 - x0);
        int dy = abs(y1 - y0);
        int sx = x0 < x1 ? 1 : -1;
        int sy = y0 < y1 ? 1 : -1;
        int err = dx - dy;

        int radius = (int)(size / 2);

        while (true) {
            // Draw circle at point
            for (int y = -radius; y <= radius; y++) {
                for (int x = -radius; x <= radius; x++) {
                    if (x * x + y * y <= radius * radius) {
                        int px = x0 + x;
                        int py = y0 + y;

                        if (px >= 0 && px < m_drawingCanvas->getWidth() &&
                            py >= 0 && py < m_drawingCanvas->getHeight()) {
                            m_drawingCanvas->setPixel(px, py, color);
                        }
                    }
                }
            }

            if (x0 == x1 && y0 == y1) break;

            int e2 = 2 * err;
            if (e2 > -dy) {
                err -= dy;
                x0 += sx;
            }
            if (e2 < dx) {
                err += dx;
                y0 += sy;
            }
        }

        m_drawingCanvas->updateTexture();
    }

    void Editor::floodFill(const Vector2& position, const Color& color) {
        if (!m_drawingCanvas) return;

        int x = (int)position.x;
        int y = (int)position.y;

        if (x < 0 || x >= m_drawingCanvas->getWidth() ||
            y < 0 || y >= m_drawingCanvas->getHeight()) {
            return;
        }

        Color targetColor = m_drawingCanvas->getPixel(x, y);
        if (targetColor == color) return;

        std::vector<std::pair<int, int>> stack;
        stack.push_back({ x, y });

        while (!stack.empty()) {
            auto [cx, cy] = stack.back();
            stack.pop_back();

            if (cx < 0 || cx >= m_drawingCanvas->getWidth() ||
                cy < 0 || cy >= m_drawingCanvas->getHeight() ||
                m_drawingCanvas->getPixel(cx, cy) != targetColor) {
                continue;
            }

            m_drawingCanvas->setPixel(cx, cy, color);

            stack.push_back({ cx + 1, cy });
            stack.push_back({ cx - 1, cy });
            stack.push_back({ cx, cy + 1 });
            stack.push_back({ cx, cy - 1 });
        }

        m_drawingCanvas->updateTexture();
    }

    void Editor::createSpriteFromDrawing() {
        if (!m_drawingCanvas) return;

        auto sprite = Engine::getInstance().getAssetManager()->createSprite(
            "DrawingSprite", m_drawingCanvas, Rect(0, 0, m_drawingCanvas->getWidth(), m_drawingCanvas->getHeight()));

        // Create game object with sprite
        auto scene = Engine::getInstance().getCurrentScene();
        if (scene) {
            auto go = scene->createGameObject("DrawingSprite");
            auto renderer = go->addComponent<SpriteRenderer>();
            renderer->setSprite(sprite);

            selectGameObject(go);
        }

        showMessage("Success", "Sprite created from drawing!");
    }

} // namespace CmakeProject1