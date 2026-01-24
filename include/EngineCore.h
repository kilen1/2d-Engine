#pragma once
#include <windows.h>
#include <wrl/client.h>
#include "GraphicsDevice.h"
#include "Renderer.h"
#include "InputManager.h"

class EngineCore {
public:
    EngineCore();
    ~EngineCore();

    bool Initialize(HINSTANCE hInstance, int nCmdShow);
    void Run();
    void Shutdown();

    // Getters for subsystems
    GraphicsDevice* GetGraphicsDevice() { return m_pGraphicsDevice.get(); }
    Renderer* GetRenderer() { return m_pRenderer.get(); }
    InputManager* GetInputManager() { return m_pInputManager.get(); }

private:
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    std::unique_ptr<GraphicsDevice> m_pGraphicsDevice;
    std::unique_ptr<Renderer> m_pRenderer;
    std::unique_ptr<InputManager> m_pInputManager;

    HINSTANCE m_hInstance;
    HWND m_hwnd;
    bool m_bRunning;
};