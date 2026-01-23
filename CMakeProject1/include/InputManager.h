#pragma once
#include "Types.h"
#include <SDL2/SDL.h>
#include <unordered_map>
#include <vector>
#include <functional>

namespace CmakeProject1 {

    class InputManager {
    public:
        InputManager();
        ~InputManager();

        void update();
        void processEvent(const SDL_Event& event);

        // Keyboard
        bool isKeyDown(SDL_Keycode key) const;
        bool isKeyPressed(SDL_Keycode key) const;
        bool isKeyReleased(SDL_Keycode key) const;

        // Mouse
        bool isMouseButtonDown(int button) const;
        bool isMouseButtonPressed(int button) const;
        bool isMouseButtonReleased(int button) const;
        Vector2 getMousePosition() const;
        Vector2 getMouseDelta() const;

        // Joystick/Gamepad
        bool isGamepadButtonDown(int gamepadId, int button) const;
        float getGamepadAxis(int gamepadId, int axis) const;

        // Hotkeys
        void bindHotkey(SDL_Keycode key, std::function<void()> callback);
        void unbindHotkey(SDL_Keycode key);

        // Utility
        void resetState();

    private:
        std::unordered_map<SDL_Keycode, bool> m_keyStates;
        std::unordered_map<SDL_Keycode, bool> m_keyDown;
        std::unordered_map<SDL_Keycode, bool> m_keyUp;

        std::unordered_map<int, bool> m_mouseButtons;
        std::unordered_map<int, bool> m_mouseDown;
        std::unordered_map<int, bool> m_mouseUp;

        Vector2 m_mousePos{ 0, 0 };
        Vector2 m_mouseDelta{ 0, 0 };

        std::unordered_map<SDL_Keycode, std::function<void()>> m_hotkeys;
    };

} // namespace CmakeProject1