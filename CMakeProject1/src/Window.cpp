#include "Window.h"
#include <SDL2/SDL.h>
#include <iostream>

namespace CmakeProject1 {

    Window::Window() = default;

    Window::~Window() {
        shutdown();
    }

    bool Window::initialize(const std::string& title, int width, int height) {
        m_width = width;
        m_height = height;

        // Настройка атрибутов OpenGL контекста (Core Profile 3.3)
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

        // Создание окна с поддержкой OpenGL
        m_window = SDL_CreateWindow(
            title.c_str(),
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            width, height,
            SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN
        );

        if (!m_window) {
            std::cerr << "Failed to create SDL window: " << SDL_GetError() << std::endl;
            return false;
        }

        // 🔥 Создание OpenGL-контекста
        m_glContext = SDL_GL_CreateContext(m_window);
        if (!m_glContext) {
            std::cerr << "Failed to create OpenGL context: " << SDL_GetError() << std::endl;
            SDL_DestroyWindow(m_window);
            m_window = nullptr;
            return false;
        }

        // Сделать контекст текущим для текущего потока
        if (SDL_GL_MakeCurrent(m_window, m_glContext) != 0) {
            std::cerr << "Failed to make OpenGL context current: " << SDL_GetError() << std::endl;
            SDL_GL_DeleteContext(m_glContext);
            SDL_DestroyWindow(m_window);
            m_glContext = nullptr;
            m_window = nullptr;
            return false;
        }

        // Включить VSync (опционально: 1 = вкл, 0 = выкл, -1 = адаптивный)
        SDL_GL_SetSwapInterval(1);

        return true;
    }

    void Window::shutdown() {
        if (m_glContext) {
            SDL_GL_DeleteContext(m_glContext);
            m_glContext = nullptr;
        }
        if (m_window) {
            SDL_DestroyWindow(m_window);
            m_window = nullptr;
        }
        m_width = 800;
        m_height = 600;
        m_fullscreen = false;
    }

    void Window::setFullscreen(bool fullscreen) {
        m_fullscreen = fullscreen;
        Uint32 flag = fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0;
        SDL_SetWindowFullscreen(m_window, flag);
    }

    void Window::swapBuffers() {
        if (m_window && m_glContext) {
            SDL_GL_SwapWindow(m_window);
        }
    }

    void Window::pollEvents() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            // Обработка событий может быть делегирована движку или InputManager'у
            // Здесь оставляем как заглушку или уберите метод, если не используется
        }
    }

} // namespace CmakeProject1