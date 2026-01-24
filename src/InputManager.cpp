#include "../include/InputManager.h"
#include <windowsx.h>
#include <tabletapi.h>

InputManager::InputManager() : 
    m_MouseX(0.0f), 
    m_MouseY(0.0f),
    m_bTabletActive(false),
    m_hTabletContext(nullptr) {
    
    // Initialize mouse buttons and keyboard state
    for (int i = 0; i < 5; i++) {
        m_MouseButtons[i] = false;
    }
    
    for (int i = 0; i < 256; i++) {
        m_KeyboardState[i] = false;
    }
    
    // Initialize tablet data
    m_TabletData.x = 0.0f;
    m_TabletData.y = 0.0f;
    m_TabletData.pressure = 0.0f;
    m_TabletData.tiltX = 0.0f;
    m_TabletData.tiltY = 0.0f;
    m_TabletData.isPenDown = false;
    m_TabletData.isEraser = false;
}

InputManager::~InputManager() {
    // Clean up tablet context if active
    if (m_hTabletContext) {
        DeleteTabletContext(m_hTabletContext);
    }
}

void InputManager::ProcessMessage(UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
        case WM_MOUSEMOVE:
        case WM_MOUSEWHEEL:
            // Handle mouse events
            switch (msg) {
                case WM_LBUTTONDOWN:
                    m_MouseButtons[0] = true;
                    m_MouseX = GET_X_LPARAM(lParam);
                    m_MouseY = GET_Y_LPARAM(lParam);
                    break;
                case WM_LBUTTONUP:
                    m_MouseButtons[0] = false;
                    break;
                case WM_RBUTTONDOWN:
                    m_MouseButtons[1] = true;
                    m_MouseX = GET_X_LPARAM(lParam);
                    m_MouseY = GET_Y_LPARAM(lParam);
                    break;
                case WM_RBUTTONUP:
                    m_MouseButtons[1] = false;
                    break;
                case WM_MBUTTONDOWN:
                    m_MouseButtons[2] = true;
                    m_MouseX = GET_X_LPARAM(lParam);
                    m_MouseY = GET_Y_LPARAM(lParam);
                    break;
                case WM_MBUTTONUP:
                    m_MouseButtons[2] = false;
                    break;
                case WM_MOUSEMOVE:
                    m_MouseX = GET_X_LPARAM(lParam);
                    m_MouseY = GET_Y_LPARAM(lParam);
                    break;
                case WM_MOUSEWHEEL:
                    // Handle scroll wheel
                    break;
            }
            
            // Trigger mouse callbacks
            for (auto& callback : m_MouseCallbacks) {
                callback((float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam), 
                         GET_KEYSTATE_WPARAM(wParam), 
                         (msg == WM_LBUTTONDOWN || msg == WM_RBUTTONDOWN || msg == WM_MBUTTONDOWN));
            }
            break;
            
        case WM_KEYDOWN:
        case WM_KEYUP:
            {
                int key = (int)wParam;
                bool isDown = (msg == WM_KEYDOWN);
                m_KeyboardState[key] = isDown;
                
                // Trigger keyboard callbacks
                for (auto& callback : m_KeyboardCallbacks) {
                    callback(key, isDown);
                }
            }
            break;
            
        default:
            // Check if it's a tablet message
            ProcessTabletMessage(msg, wParam, lParam);
            break;
    }
}

bool InputManager::ProcessTabletMessage(UINT msg, WPARAM wParam, LPARAM lParam) {
    // Check if this is a Windows XP Tablet PC or newer pen message
    if (msg == WM_TABLET_QUERYSYSTEMGESTURESTATUS) {
        // Disable system gestures to allow our application to handle pen input
        return TRUE;
    }
    
    // Handle packet messages for pressure-sensitive input
    // We'll look for standard pen/stylus messages
    if (msg >= WM_POINTERDEVICECHANGE && msg <= WM_POINTERUPDATE) {
        // This is a simplified approach - in reality, we'd need to use the RealTimeStylus API
        // or Windows Ink APIs to get pressure data
        
        // For now, simulate pressure based on other input
        m_bTabletActive = true;
        m_TabletData.pressure = 0.5f; // Default pressure
        m_TabletData.x = (float)GET_X_LPARAM(lParam);
        m_TabletData.y = (float)GET_Y_LPARAM(lParam);
        
        // Trigger tablet callbacks
        for (auto& callback : m_TabletCallbacks) {
            callback(m_TabletData);
        }
        
        return true;
    }
    
    return false;
}

void InputManager::RegisterMouseCallback(std::function<void(float, float, int, bool)> callback) {
    m_MouseCallbacks.push_back(callback);
}

void InputManager::RegisterKeyboardCallback(std::function<void(int, bool)> callback) {
    m_KeyboardCallbacks.push_back(callback);
}

void InputManager::RegisterTabletCallback(std::function<void(const TabletData&)> callback) {
    m_TabletCallbacks.push_back(callback);
}