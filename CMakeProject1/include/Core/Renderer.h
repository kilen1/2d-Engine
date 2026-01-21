#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <unordered_map>
#include <memory>

namespace CmakeProject1 {

    struct Color {
        Uint8 r, g, b, a;
        Color(Uint8 r = 255, Uint8 g = 255, Uint8 b = 255, Uint8 a = 255) 
            : r(r), g(g), b(b), a(a) {}
    };

    class Texture {
    public:
        Texture(SDL_Texture* tex, int width, int height) 
            : texture(tex), width(width), height(height) {}
        ~Texture();
        
        SDL_Texture* getSDLTexture() const { return texture; }
        int getWidth() const { return width; }
        int getHeight() const { return height; }

    private:
        SDL_Texture* texture;
        int width, height;
    };

    class Renderer {
    public:
        Renderer(SDL_Renderer* sdlRenderer);
        ~Renderer();

        bool loadTexture(const std::string& path, const std::string& id);
        void drawTexture(const std::string& id, int x, int y, int w = 0, int h = 0);
        void drawTextureEx(const std::string& id, SDL_Rect* src, SDL_Rect* dst, double angle = 0.0, 
                          SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE);
        
        void clear();
        void present();
        
        void setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
        void setDrawColor(const Color& color);

        void drawPoint(int x, int y);
        void drawLine(int x1, int y1, int x2, int y2);
        void drawRect(const SDL_Rect& rect);
        void fillRect(const SDL_Rect& rect);
        
        SDL_Renderer* getSDLRenderer() const { return renderer; }

    private:
        SDL_Renderer* renderer;
        std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
    };

} // namespace CmakeProject1