#pragma once
#include "PressureBrush.h"
#include <vector>
#include <memory>
#include <map>
#include <string>

class BrushSystem {
public:
    BrushSystem();
    ~BrushSystem();

    bool Initialize();
    void Cleanup();

    // Brush management
    PressureBrush* CreateBrush(const std::string& name, float minSize, float maxSize);
    PressureBrush* GetBrush(const std::string& name);
    void SetCurrentBrush(const std::string& name);
    PressureBrush* GetCurrentBrush() { return m_pCurrentBrush; }

    // Drawing operations
    void StartStroke(float x, float y, float pressure);
    void ContinueStroke(float x, float y, float pressure);
    void EndStroke();
    
    // Configuration
    void SetColor(float r, float g, float b, float a = 1.0f);
    void SetOpacity(float opacity) { m_Opacity = opacity; }
    float GetOpacity() const { return m_Opacity; }

private:
    std::vector<std::unique_ptr<PressureBrush>> m_Brushes;
    std::map<std::string, PressureBrush*> m_BrushMap;
    PressureBrush* m_pCurrentBrush;
    
    float m_LastX, m_LastY;
    bool m_bDrawing;
    
    // Current drawing properties
    float m_ColorR, m_ColorG, m_ColorB, m_ColorA;
    float m_Opacity;
};