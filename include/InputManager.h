#pragma once
#include <windows.h>
#include <vector>
#include <functional>
#include <objbase.h>

// Structure to hold pressure-sensitive tablet input data
struct TabletData {
    float x, y;           // Position
    float pressure;       // Pressure level (0.0 to 1.0)
    float tiltX, tiltY;   // Tilt angles
    bool isPenDown;       // Whether pen is touching surface
    bool isEraser;        // Whether eraser is being used
};

class InputManager {
public:
    InputManager();
    ~InputManager();

    void ProcessMessage(UINT msg, WPARAM wParam, LPARAM lParam);
    
    // Getters for current input state
    float GetMouseX() const { return m_MouseX; }
    float GetMouseY() const { return m_MouseY; }
    bool IsMouseDown(int button) const { return m_MouseButtons[button]; }
    bool IsKeyDown(int key) const { return m_KeyboardState[key]; }
    
    // Tablet-specific getters
    const TabletData& GetTabletData() const { return m_TabletData; }
    bool IsTabletActive() const { return m_bTabletActive; }
    
    // Register callbacks for input events
    void RegisterMouseCallback(std::function<void(float, float, int, bool)> callback);
    void RegisterKeyboardCallback(std::function<void(int, bool)> callback);
    void RegisterTabletCallback(std::function<void(const TabletData&)> callback);

    // Handle Windows tablet messages
    bool ProcessTabletMessage(UINT msg, WPARAM wParam, LPARAM lParam);

private:
    float m_MouseX, m_MouseY;
    bool m_MouseButtons[5];  // Left, Right, Middle, X1, X2
    bool m_KeyboardState[256];
    TabletData m_TabletData;
    bool m_bTabletActive;
    
    std::vector<std::function<void(float, float, int, bool)>> m_MouseCallbacks;
    std::vector<std::function<void(int, bool)>> m_KeyboardCallbacks;
    std::vector<std::function<void(const TabletData&)>> m_TabletCallbacks;
    
    // Windows tablet API structures
    HCTX m_hTabletContext;
};