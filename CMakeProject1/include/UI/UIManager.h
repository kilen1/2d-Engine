#pragma once

#include "../Core/Types.h"
#include <glm/glm.hpp>
#include <string>

struct SDL_Rect;

namespace CmakeProject1 {
    class UIManager {
    public:
        virtual ~UIManager() = default;
        virtual bool initialize() = 0;
        virtual void update() = 0;
        virtual void shutdown() = 0;

        virtual void createUIElement(const std::string& type, const std::string& name, const Vector2& position) = 0;
        virtual void showMessageBox(const std::string& title, const std::string& message) = 0;
        virtual void render() = 0;
        virtual void handleInput() = 0;
    };
}