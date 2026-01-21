#include "../../include/Core/Window.h"
#include <iostream>

namespace CmakeProject1 {

    Window::Window(const std::string& title, int width, int height)
        : title(title), width(width), height(height) {
        
        window = SDL_CreateWindow(
            title.c_str(),
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            width, height,
            SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
        );
        
        if (!window) {
            std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
            return;
        }
        
        renderer = SDL_CreateRenderer(window, -1, 
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        
        if (!renderer) {
            std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
            SDL_DestroyWindow(window);
            window = nullptr;
            return;
        }
    }

    Window::~Window() {
        if (renderer) {
            SDL_DestroyRenderer(renderer);
        }
        if (window) {
            SDL_DestroyWindow(window);
        }
    }

    void Window::setTitle(const std::string& title) {
        this->title = title;
        if (window) {
            SDL_SetWindowTitle(window, title.c_str());
        }
    }

    void Window::setSize(int width, int height) {
        this->width = width;
        this->height = height;
        if (window) {
            SDL_SetWindowSize(window, width, height);
        }
    }

    void Window::swapBuffers() {
        if (renderer) {
            SDL_RenderPresent(renderer);
        }
    }

    void Window::pollEvents() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quitRequested = true;
            }
        }
    }

} // namespace CmakeProject1