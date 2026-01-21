#pragma once

#include <SDL2/SDL.h>
#include <string>

namespace CmakeProject1 {

    class Window {
    public:
        Window(const std::string& title, int width, int height);
        ~Window();

        bool isValid() const { return window != nullptr; }
        
        SDL_Window* getSDLWindow() const { return window; }
        SDL_Renderer* getSDLRenderer() const { return renderer; }
        
        void setTitle(const std::string& title);
        void setSize(int width, int height);
        int getWidth() const { return width; }
        int getHeight() const { return height; }
        
        void swapBuffers();
        void pollEvents();
        bool shouldClose() const { return quitRequested; }

    private:
        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;
        int width, height;
        bool quitRequested = false;
        std::string title;
    };

} // namespace CmakeProject1