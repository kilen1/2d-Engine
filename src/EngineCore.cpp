#include "../include/EngineCore.h"
#include <commctrl.h>

EngineCore::EngineCore() : m_hInstance(nullptr), m_hwnd(nullptr), m_bRunning(false) {
    // Initialize COM for Windows tablet support
    CoInitialize(NULL);
}

EngineCore::~EngineCore() {
    CoUninitialize();
}

bool EngineCore::Initialize(HINSTANCE hInstance, int nCmdShow) {
    m_hInstance = hInstance;

    // Register window class
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = EngineCore::WindowProc;
    wc.hInstance = m_hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = L"Direct2DEngineClass";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClassEx(&wc);

    // Create window
    m_hwnd = CreateWindow(
        L"Direct2DEngineClass",
        L"Direct2D Game Engine",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720,
        NULL, NULL, m_hInstance, this
    );

    if (!m_hwnd) {
        return false;
    }

    // Initialize subsystems
    m_pGraphicsDevice = std::make_unique<GraphicsDevice>();
    if (!m_pGraphicsDevice->Initialize(m_hwnd)) {
        return false;
    }

    m_pRenderer = std::make_unique<Renderer>(m_pGraphicsDevice.get());
    if (!m_pRenderer->Initialize()) {
        return false;
    }

    m_pInputManager = std::make_unique<InputManager>();

    ShowWindow(m_hwnd, nCmdShow);
    UpdateWindow(m_hwnd);

    m_bRunning = true;
    return true;
}

void EngineCore::Run() {
    MSG msg = {};

    while (m_bRunning) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            // Main game loop
            m_pGraphicsDevice->BeginFrame(0.1f, 0.1f, 0.1f, 1.0f);
            
            // Render here
            m_pRenderer->Flush();
            
            m_pGraphicsDevice->EndFrame();
        }
    }
}

void EngineCore::Shutdown() {
    if (m_pRenderer) {
        m_pRenderer->Cleanup();
        m_pRenderer.reset();
    }

    if (m_pGraphicsDevice) {
        m_pGraphicsDevice->Cleanup();
        m_pGraphicsDevice.reset();
    }

    if (m_hwnd) {
        DestroyWindow(m_hwnd);
        m_hwnd = nullptr;
    }
}

LRESULT CALLBACK EngineCore::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    EngineCore* pEngine = nullptr;

    if (uMsg == WM_CREATE) {
        CREATESTRUCT* pCS = reinterpret_cast<CREATESTRUCT*>(lParam);
        pEngine = static_cast<EngineCore*>(pCS->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pEngine));
    } else {
        pEngine = reinterpret_cast<EngineCore*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    if (pEngine) {
        // Pass message to input manager
        pEngine->GetInputManager()->ProcessMessage(uMsg, wParam, lParam);

        switch (uMsg) {
        case WM_DESTROY:
            pEngine->m_bRunning = false;
            PostQuitMessage(0);
            return 0;

        case WM_SIZE:
            if (pEngine->GetGraphicsDevice()) {
                UINT width = LOWORD(lParam);
                UINT height = HIWORD(lParam);
                pEngine->GetGraphicsDevice()->Resize(width, height);
            }
            return 0;
        }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}