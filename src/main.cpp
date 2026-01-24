#include "../include/EngineCore.h"
#include "../include/BrushSystem.h"
#include <windows.h>

// Global pointer to engine for input handling
EngineCore* g_pEngine = nullptr;
BrushSystem* g_pBrushSystem = nullptr;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow) {
    EngineCore engine;
    g_pEngine = &engine;
    
    if (!engine.Initialize(hInstance, nCmdShow)) {
        return 1;
    }
    
    // Initialize brush system
    auto renderer = engine.GetRenderer();
    auto inputManager = engine.GetInputManager();
    
    BrushSystem brushSystem;
    g_pBrushSystem = &brushSystem;
    
    if (!brushSystem.Initialize()) {
        return 1;
    }
    
    // Register input callbacks for pressure-sensitive drawing
    inputManager->RegisterMouseCallback([](float x, float y, int button, bool isDown) {
        if (button == 0) { // Left mouse button
            if (isDown) {
                // On mouse down, start a stroke with medium pressure
                g_pBrushSystem->StartStroke(x, y, 0.7f);
            } else {
                // On mouse up, end the stroke
                g_pBrushSystem->EndStroke();
            }
        }
    });
    
    // Register tablet callback for pressure-sensitive input
    inputManager->RegisterTabletCallback([](const TabletData& tabletData) {
        if (tabletData.isPenDown) {
            // Update brush with actual pressure data from tablet
            if (g_pBrushSystem) {
                // Start or continue stroke based on pressure
                if (!g_pBrushSystem->GetCurrentBrush()) return;
                
                // For demo purposes, use the pressure value if available, otherwise default
                float pressure = tabletData.pressure > 0 ? tabletData.pressure : 0.5f;
                
                if (g_pBrushSystem->GetCurrentBrush()) {
                    g_pBrushSystem->ContinueStroke(tabletData.x, tabletData.y, pressure);
                }
            }
        }
    });
    
    engine.Run();
    engine.Shutdown();
    
    return 0;
}