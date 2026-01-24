#include "../include/BrushSystem.h"
#include <algorithm>
using std::min;
using std::max;

BrushSystem::BrushSystem() :
    m_pCurrentBrush(nullptr),
    m_LastX(-1),
    m_LastY(-1),
    m_bDrawing(false),
    m_ColorR(0.0f),
    m_ColorG(0.0f),
    m_ColorB(0.0f),
    m_ColorA(1.0f),
    m_Opacity(1.0f) {
}

BrushSystem::~BrushSystem() {
    Cleanup();
}

bool BrushSystem::Initialize() {
    // Create default brushes
    PressureBrush* defaultBrush = CreateBrush("Default", 2.0f, 20.0f);
    if (!defaultBrush) return false;
    
    defaultBrush->SetHardness(0.8f);
    defaultBrush->SetSpacing(2.0f);
    defaultBrush->SetFlow(1.0f);
    
    m_pCurrentBrush = defaultBrush;
    
    return true;
}

void BrushSystem::Cleanup() {
    m_Brushes.clear();
    m_BrushMap.clear();
    m_pCurrentBrush = nullptr;
}

PressureBrush* BrushSystem::CreateBrush(const std::string& name, float minSize, float maxSize) {
    auto brush = std::make_unique<PressureBrush>(name, minSize, maxSize);
    PressureBrush* rawPtr = brush.get();
    
    m_Brushes.push_back(std::move(brush));
    m_BrushMap[name] = rawPtr;
    
    return rawPtr;
}

PressureBrush* BrushSystem::GetBrush(const std::string& name) {
    auto it = m_BrushMap.find(name);
    if (it != m_BrushMap.end()) {
        return it->second;
    }
    return nullptr;
}

void BrushSystem::SetCurrentBrush(const std::string& name) {
    auto it = m_BrushMap.find(name);
    if (it != m_BrushMap.end()) {
        m_pCurrentBrush = it->second;
    }
}

void BrushSystem::StartStroke(float x, float y, float pressure) {
    if (!m_pCurrentBrush) return;
    
    m_LastX = x;
    m_LastY = y;
    m_bDrawing = true;
    
    // Apply initial brush stroke
    m_pCurrentBrush->ApplyStroke(x, y, pressure, nullptr);
}

void BrushSystem::ContinueStroke(float x, float y, float pressure) {
    if (!m_pCurrentBrush || !m_bDrawing) return;
    
    // Draw a line from the last point to the current point
    // In a real implementation, we would render this line segment with pressure-sensitive properties
    // For now, just apply the stroke at the current position
    m_pCurrentBrush->ApplyStroke(x, y, pressure, nullptr);
    
    m_LastX = x;
    m_LastY = y;
}

void BrushSystem::EndStroke() {
    m_bDrawing = false;
    m_LastX = -1;
    m_LastY = -1;
}

void BrushSystem::SetColor(float r, float g, float b, float a) {
    m_ColorR = max(0.0f, min(1.0f, r));
    m_ColorG = max(0.0f, min(1.0f, g));
    m_ColorB = max(0.0f, min(1.0f, b));
    m_ColorA = max(0.0f, min(1.0f, a));
}