#pragma once

#include <SDL2/SDL.h>
#include <unordered_map>
#include <functional>
#include <vector>

namespace CmakeProject1 {

    enum class KeyState {
        PRESSED,
        RELEASED,
        HELD
    };

    struct MouseState {
        int x = 0;
        int y = 0;
        bool leftButton = false;
        bool rightButton = false;
        bool middleButton = false;
        int wheelX = 0;
        int wheelY = 0;
    };

    class InputManager {
    public:
        InputManager();
        ~InputManager();

        void update();

        KeyState getKeyState(SDL_Scancode scancode) const;
        KeyState getKeyState(SDL_Keycode keycode) const;
        bool isKeyDown(SDL_Scancode scancode) const;
        bool isKeyPressed(SDL_Scancode scancode) const;
        bool isKeyReleased(SDL_Scancode scancode) const;

        MouseState getMouseState() const;
        bool isMouseButtonDown(Uint8 button) const;
        bool isMouseButtonPressed(Uint8 button) const;
        bool isMouseButtonReleased(Uint8 button) const;

        void setTextInput(bool enabled);
        bool isTextInputActive() const;
        const std::string& getText() const;

        void registerKeyCallback(SDL_Scancode key, std::function<void()> onPress, 
                                std::function<void()> onRelease = nullptr);
        void registerMouseButtonCallback(Uint8 button, std::function<void()> onPress,
                                        std::function<void()> onRelease = nullptr);

    private:
        void handleKeyDown(const SDL_Event& event);
        void handleKeyUp(const SDL_Event& event);
        void handleMouseMotion(const SDL_Event& event);
        void handleMouseButton(const SDL_Event& event);
        void handleMouseWheel(const SDL_Event& event);
        void handleTextInput(const SDL_Event& event);

        const Uint8* keyboardState = nullptr;
        Uint8 mouseState = 0;
        
        std::unordered_map<SDL_Scancode, KeyState> keyStates;
        std::unordered_map<SDL_Scancode, KeyState> prevKeyStates;
        
        MouseState currentMouseState;
        MouseState prevMouseState;
        
        std::unordered_map<SDL_Scancode, std::pair<std::function<void()>, std::function<void()>>> keyCallbacks;
        std::unordered_map<Uint8, std::pair<std::function<void()>, std::function<void()>>> mouseCallbacks;
        
        bool textInputActive = false;
        std::string currentText;
    };

} // namespace CmakeProject1