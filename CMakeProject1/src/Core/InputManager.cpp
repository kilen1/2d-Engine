#include "../../include/Core/InputManager.h"

namespace CmakeProject1 {

    InputManager::InputManager() {
        keyboardState = SDL_GetKeyboardState(nullptr);
    }

    InputManager::~InputManager() {
        if (textInputActive) {
            SDL_StopTextInput();
        }
    }

    void InputManager::update() {
        // Store previous states
        prevKeyStates = keyStates;
        prevMouseState = currentMouseState;
        
        // Reset current mouse wheel values
        currentMouseState.wheelX = 0;
        currentMouseState.wheelY = 0;

        // Process events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_KEYDOWN:
                    handleKeyDown(event);
                    break;
                case SDL_KEYUP:
                    handleKeyUp(event);
                    break;
                case SDL_MOUSEMOTION:
                    handleMouseMotion(event);
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    handleMouseButton(event);
                    break;
                case SDL_MOUSEBUTTONUP:
                    handleMouseButton(event);
                    break;
                case SDL_MOUSEWHEEL:
                    handleMouseWheel(event);
                    break;
                case SDL_TEXTINPUT:
                    handleTextInput(event);
                    break;
                default:
                    break;
            }
        }

        // Update held keys
        for (int i = 0; i < SDL_NUM_SCANCODES; ++i) {
            if (keyboardState[i]) {
                if (keyStates[static_cast<SDL_Scancode>(i)] == KeyState::RELEASED) {
                    keyStates[static_cast<SDL_Scancode>(i)] = KeyState::PRESSED;
                } else if (keyStates[static_cast<SDL_Scancode>(i)] == KeyState::PRESSED) {
                    keyStates[static_cast<SDL_Scancode>(i)] = KeyState::HELD;
                }
            } else {
                if (keyStates[static_cast<SDL_Scancode>(i)] == KeyState::PRESSED || 
                    keyStates[static_cast<SDL_Scancode>(i)] == KeyState::HELD) {
                    keyStates[static_cast<SDL_Scancode>(i)] = KeyState::RELEASED;
                }
            }
        }
    }

    void InputManager::handleKeyDown(const SDL_Event& event) {
        SDL_Scancode scancode = event.key.keysym.scancode;
        keyStates[scancode] = KeyState::PRESSED;
        
        auto it = keyCallbacks.find(scancode);
        if (it != keyCallbacks.end() && it->second.first) {
            it->second.first();
        }
    }

    void InputManager::handleKeyUp(const SDL_Event& event) {
        SDL_Scancode scancode = event.key.keysym.scancode;
        keyStates[scancode] = KeyState::RELEASED;
        
        auto it = keyCallbacks.find(scancode);
        if (it != keyCallbacks.end() && it->second.second) {
            it->second.second();
        }
    }

    void InputManager::handleMouseMotion(const SDL_Event& event) {
        currentMouseState.x = event.motion.x;
        currentMouseState.y = event.motion.y;
    }

    void InputManager::handleMouseButton(const SDL_Event& event) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            currentMouseState.leftButton = (event.type == SDL_MOUSEBUTTONDOWN);
        } else if (event.button.button == SDL_BUTTON_RIGHT) {
            currentMouseState.rightButton = (event.type == SDL_MOUSEBUTTONDOWN);
        } else if (event.button.button == SDL_BUTTON_MIDDLE) {
            currentMouseState.middleButton = (event.type == SDL_MOUSEBUTTONDOWN);
        }

        auto it = mouseCallbacks.find(event.button.button);
        if (it != mouseCallbacks.end()) {
            if (event.type == SDL_MOUSEBUTTONDOWN && it->second.first) {
                it->second.first();
            } else if (event.type == SDL_MOUSEBUTTONUP && it->second.second) {
                it->second.second();
            }
        }
    }

    void InputManager::handleMouseWheel(const SDL_Event& event) {
        currentMouseState.wheelX = event.wheel.x;
        currentMouseState.wheelY = event.wheel.y;
    }

    void InputManager::handleTextInput(const SDL_Event& event) {
        if (textInputActive) {
            currentText = event.text.text;
        }
    }

    KeyState InputManager::getKeyState(SDL_Scancode scancode) const {
        auto it = keyStates.find(scancode);
        return (it != keyStates.end()) ? it->second : KeyState::RELEASED;
    }

    KeyState InputManager::getKeyState(SDL_Keycode keycode) const {
        SDL_Scancode scancode = SDL_GetScancodeFromKey(keycode);
        return getKeyState(scancode);
    }

    bool InputManager::isKeyDown(SDL_Scancode scancode) const {
        return keyStates.count(scancode) && 
               (keyStates.at(scancode) == KeyState::PRESSED || keyStates.at(scancode) == KeyState::HELD);
    }

    bool InputManager::isKeyPressed(SDL_Scancode scancode) const {
        return keyStates.count(scancode) && keyStates.at(scancode) == KeyState::PRESSED &&
               prevKeyStates.count(scancode) && prevKeyStates.at(scancode) == KeyState::RELEASED;
    }

    bool InputManager::isKeyReleased(SDL_Scancode scancode) const {
        return keyStates.count(scancode) && keyStates.at(scancode) == KeyState::RELEASED &&
               prevKeyStates.count(scancode) && prevKeyStates.at(scancode) == KeyState::HELD;
    }

    MouseState InputManager::getMouseState() const {
        return currentMouseState;
    }

    bool InputManager::isMouseButtonDown(Uint8 button) const {
        switch (button) {
            case SDL_BUTTON_LEFT:
                return currentMouseState.leftButton;
            case SDL_BUTTON_RIGHT:
                return currentMouseState.rightButton;
            case SDL_BUTTON_MIDDLE:
                return currentMouseState.middleButton;
            default:
                return false;
        }
    }

    bool InputManager::isMouseButtonPressed(Uint8 button) const {
        bool current = isMouseButtonDown(button);
        bool previous = false;
        
        switch (button) {
            case SDL_BUTTON_LEFT:
                previous = prevMouseState.leftButton;
                break;
            case SDL_BUTTON_RIGHT:
                previous = prevMouseState.rightButton;
                break;
            case SDL_BUTTON_MIDDLE:
                previous = prevMouseState.middleButton;
                break;
            default:
                return false;
        }
        
        return current && !previous;
    }

    bool InputManager::isMouseButtonReleased(Uint8 button) const {
        bool current = !isMouseButtonDown(button);
        bool previous = false;
        
        switch (button) {
            case SDL_BUTTON_LEFT:
                previous = prevMouseState.leftButton;
                break;
            case SDL_BUTTON_RIGHT:
                previous = prevMouseState.rightButton;
                break;
            case SDL_BUTTON_MIDDLE:
                previous = prevMouseState.middleButton;
                break;
            default:
                return false;
        }
        
        return current && previous;
    }

    void InputManager::setTextInput(bool enabled) {
        if (enabled && !textInputActive) {
            SDL_StartTextInput();
            textInputActive = true;
        } else if (!enabled && textInputActive) {
            SDL_StopTextInput();
            textInputActive = false;
        }
    }

    bool InputManager::isTextInputActive() const {
        return textInputActive;
    }

    const std::string& InputManager::getText() const {
        return currentText;
    }

    void InputManager::registerKeyCallback(SDL_Scancode key, std::function<void()> onPress,
                                         std::function<void()> onRelease) {
        keyCallbacks[key] = std::make_pair(onPress, onRelease);
    }

    void InputManager::registerMouseButtonCallback(Uint8 button, std::function<void()> onPress,
                                                 std::function<void()> onRelease) {
        mouseCallbacks[button] = std::make_pair(onPress, onRelease);
    }

} // namespace CmakeProject1