#include "InputManager.h"
#include <SDL2/SDL.h>
#include <iostream>

namespace CmakeProject1 {

    InputManager::InputManager() {}

    InputManager::~InputManager() {}

    void InputManager::update() {
        // Clear press/release states
        for (auto& [key, state] : m_keyDown) state = false;
        for (auto& [key, state] : m_keyUp) state = false;
        for (auto& [button, state] : m_mouseDown) state = false;
        for (auto& [button, state] : m_mouseUp) state = false;

        // Update mouse delta
        static Vector2 lastMousePos{ 0, 0 };
        m_mouseDelta = m_mousePos - lastMousePos;
        lastMousePos = m_mousePos;
    }

    void InputManager::processEvent(const SDL_Event& event) {
        switch (event.type) {
        case SDL_KEYDOWN:
            m_keyStates[event.key.keysym.sym] = true;
            m_keyDown[event.key.keysym.sym] = true;
            if (m_hotkeys.find(event.key.keysym.sym) != m_hotkeys.end()) {
                m_hotkeys[event.key.keysym.sym]();
            }
            break;
        case SDL_KEYUP:
            m_keyStates[event.key.keysym.sym] = false;
            m_keyUp[event.key.keysym.sym] = true;
            break;
        case SDL_MOUSEBUTTONDOWN:
            m_mouseButtons[event.button.button] = true;
            m_mouseDown[event.button.button] = true;
            break;
        case SDL_MOUSEBUTTONUP:
            m_mouseButtons[event.button.button] = false;
            m_mouseUp[event.button.button] = true;
            break;
        case SDL_MOUSEMOTION:
            m_mousePos.x = static_cast<float>(event.motion.x);
            m_mousePos.y = static_cast<float>(event.motion.y);
            break;
        default:
            break;
        }
    }

    bool InputManager::isKeyDown(SDL_Keycode key) const {
        auto it = m_keyStates.find(key);
        return it != m_keyStates.end() && it->second;
    }

    bool InputManager::isKeyPressed(SDL_Keycode key) const {
        auto it = m_keyDown.find(key);
        return it != m_keyDown.end() && it->second;
    }

    bool InputManager::isKeyReleased(SDL_Keycode key) const {
        auto it = m_keyUp.find(key);
        return it != m_keyUp.end() && it->second;
    }

    bool InputManager::isMouseButtonDown(int button) const {
        auto it = m_mouseButtons.find(button);
        return it != m_mouseButtons.end() && it->second;
    }

    bool InputManager::isMouseButtonPressed(int button) const {
        auto it = m_mouseDown.find(button);
        return it != m_mouseDown.end() && it->second;
    }

    bool InputManager::isMouseButtonReleased(int button) const {
        auto it = m_mouseUp.find(button);
        return it != m_mouseUp.end() && it->second;
    }

    Vector2 InputManager::getMousePosition() const {
        return m_mousePos;
    }

    Vector2 InputManager::getMouseDelta() const {
        return m_mouseDelta;
    }

    bool InputManager::isGamepadButtonDown(int gamepadId, int button) const {
        // Placeholder
        return false;
    }

    float InputManager::getGamepadAxis(int gamepadId, int axis) const {
        // Placeholder
        return 0.0f;
    }

    void InputManager::bindHotkey(SDL_Keycode key, std::function<void()> callback) {
        m_hotkeys[key] = callback;
    }

    void InputManager::unbindHotkey(SDL_Keycode key) {
        m_hotkeys.erase(key);
    }

    void InputManager::resetState() {
        m_keyStates.clear();
        m_keyDown.clear();
        m_keyUp.clear();
        m_mouseButtons.clear();
        m_mouseDown.clear();
        m_mouseUp.clear();
        m_mousePos = { 0, 0 };
        m_mouseDelta = { 0, 0 };
    }

} // namespace CmakeProject1