#include "SpriteRenderer.h"
#include "Transform.h"
#include "Engine.h"
#include "GameObject.h"
#include <iostream>

namespace CmakeProject1 {

    SpriteRenderer::SpriteRenderer() {}

    SpriteRenderer::~SpriteRenderer() {
        if (texture) {
            SDL_DestroyTexture(texture);
            texture = nullptr;
        }
    }

    void SpriteRenderer::setTexture(const std::string& filepath) {
        if (texture) {
            SDL_DestroyTexture(texture);
            texture = nullptr;
        }

        SDL_Surface* loadedSurface = IMG_Load(filepath.c_str());
        if (!loadedSurface) {
            std::cerr << "Unable to load image " << filepath << "! SDL_image Error: " << IMG_GetError() << std::endl;
            return;
        }

        // Create texture from surface
        auto engineRenderer = Engine::getInstance().getRenderer();
        if (!engineRenderer) {
            std::cerr << "Engine renderer not available!" << std::endl;
            SDL_FreeSurface(loadedSurface);
            return;
        }

        texture = SDL_CreateTextureFromSurface(engineRenderer->getSDLRenderer(), loadedSurface);
        if (!texture) {
            std::cerr << "Unable to create texture from " << filepath << "! SDL Error: " << SDL_GetError() << std::endl;
        }
        else {
            // Set the source rectangle dimensions based on the loaded surface
            sourceRect.x = 0;
            sourceRect.y = 0;
            sourceRect.w = loadedSurface->w;
            sourceRect.h = loadedSurface->h;
        }

        SDL_FreeSurface(loadedSurface);
        texturePath = filepath;
    }

    void SpriteRenderer::setColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
        color.r = r;
        color.g = g;
        color.b = b;
        color.a = a;
    }

    void SpriteRenderer::render(SDL_Renderer* renderer) {
        if (!texture || !enabled) return;

        auto ownerPtr = getOwner();
        if (!ownerPtr) return;

        auto transform = ownerPtr->getTransform();
        if (!transform) return;

        // Update destination rectangle based on transform
        SDL_Rect destRect = transform->getBounds();

        // Set color modulation
        SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
        SDL_SetTextureAlphaMod(texture, color.a);

        // Render the texture
        SDL_RenderCopy(renderer, texture, &sourceRect, &destRect);
    }

} // namespace CmakeProject1