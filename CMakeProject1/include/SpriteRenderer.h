#pragma once

#include "Component.h"
#include <SDL2/SDL.h>
#include <string>

namespace CmakeProject1 {

    class SpriteRenderer : public Component {
    public:
        SpriteRenderer();
        virtual ~SpriteRenderer();

        void setTexture(const std::string& filepath);
        void setColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);

        virtual void render(SDL_Renderer* renderer) override;

    private:
        SDL_Texture* texture = nullptr;
        std::string texturePath;
        SDL_Color color = { 255, 255, 255, 255 };
        SDL_Rect sourceRect;
        SDL_Rect destRect;
    };

} // namespace CmakeProject1