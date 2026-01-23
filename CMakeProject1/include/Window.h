#pragma once

#include "Types.h"
#include <SDL2/SDL.h>
#include <string>

namespace CmakeProject1 {

    class Window {
    public:
        Window();
        ~Window();

        // Инициализирует окно и OpenGL-контекст
        bool initialize(const std::string& title, int width, int height);
        void shutdown();

        // Получение указателей для ImGui и рендерера
        SDL_Window* getSDLWindow() const { return m_window; }
        SDL_GLContext getGLContext() const { return m_glContext; }

        // Размеры окна
        int getWidth() const { return m_width; }
        int getHeight() const { return m_height; }

        // Управление полноэкранным режимом
        void setFullscreen(bool fullscreen);
        bool isFullscreen() const { return m_fullscreen; }

        // Обмен буферами (для OpenGL)
        void swapBuffers();

        // Опрос событий SDL (опционально — может делегироваться движку)
        void pollEvents();

    private:
        SDL_Window* m_window = nullptr;
        SDL_GLContext m_glContext = nullptr;

        int m_width = 800;
        int m_height = 600;
        bool m_fullscreen = false;
    };

} // namespace CmakeProject1