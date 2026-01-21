#include "../../include/Core/Renderer.h"
#include <SDL2/SDL_image.h>
#include <iostream>

namespace CmakeProject1 {

    // Texture implementation
    Texture::~Texture() {
        if (texture) {
            SDL_DestroyTexture(texture);
        }
    }

    // Renderer implementation
    Renderer::Renderer(SDL_Renderer* sdlRenderer) : renderer(sdlRenderer) {}

    Renderer::~Renderer() {
        textures.clear();
    }

    bool Renderer::loadTexture(const std::string& path, const std::string& id) {
        SDL_Surface* tempSurface = IMG_Load(path.c_str());
        if (!tempSurface) {
            std::cerr << "Failed to load texture: " << path << " - " << IMG_GetError() << std::endl;
            return false;
        }

        SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, tempSurface);
        if (!tex) {
            std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
            SDL_FreeSurface(tempSurface);
            return false;
        }

        int width = tempSurface->w;
        int height = tempSurface->h;
        SDL_FreeSurface(tempSurface);

        textures[id] = std::make_shared<Texture>(tex, width, height);
        return true;
    }

    void Renderer::drawTexture(const std::string& id, int x, int y, int w, int h) {
        auto it = textures.find(id);
        if (it != textures.end()) {
            SDL_Rect destRect = {x, y, w ? w : it->second->getWidth(), h ? h : it->second->getHeight()};
            SDL_RenderCopy(renderer, it->second->getSDLTexture(), nullptr, &destRect);
        }
    }

    void Renderer::drawTextureEx(const std::string& id, SDL_Rect* src, SDL_Rect* dst, double angle, 
                                SDL_Point* center, SDL_RendererFlip flip) {
        auto it = textures.find(id);
        if (it != textures.end()) {
            SDL_RenderCopyEx(renderer, it->second->getSDLTexture(), src, dst, angle, center, flip);
        }
    }

    void Renderer::clear() {
        SDL_RenderClear(renderer);
    }

    void Renderer::present() {
        SDL_RenderPresent(renderer);
    }

    void Renderer::setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
        SDL_SetRenderDrawColor(renderer, r, g, b, a);
    }

    void Renderer::setDrawColor(const Color& color) {
        setDrawColor(color.r, color.g, color.b, color.a);
    }

    void Renderer::drawPoint(int x, int y) {
        SDL_RenderDrawPoint(renderer, x, y);
    }

    void Renderer::drawLine(int x1, int y1, int x2, int y2) {
        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    }

    void Renderer::drawRect(const SDL_Rect& rect) {
        SDL_RenderDrawRect(renderer, &rect);
    }

    void Renderer::fillRect(const SDL_Rect& rect) {
        SDL_RenderFillRect(renderer, &rect);
    }

} // namespace CmakeProject1