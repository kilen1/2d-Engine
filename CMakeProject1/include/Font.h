#pragma once
#include "Types.h"
#include <string>
#include <memory>

namespace CmakeProject1 {

    class Font {
    public:
        Font() = default;
        ~Font() = default;

        std::string path;
        int size = 12;
        bool loaded = false;
        // TTF_Font* font; // SDL_ttf handle — можно добавить позже
    };

} // namespace CmakeProject1