#include "../include/PressureBrush.h"
#include <algorithm>
using std::min;
using std::max;

PressureBrush::PressureBrush(const std::string& name, float minSize, float maxSize) :
    m_Name(name),
    m_MinSize(minSize),
    m_MaxSize(maxSize),
    m_CurrentSize(minSize),
    m_Hardness(0.5f),
    m_Spacing(1.0f),
    m_Flow(1.0f),
    m_Type(BrushType::STANDARD),
    m_LastX(-1),
    m_LastY(-1) {
}

PressureBrush::~PressureBrush() {
}

void PressureBrush::SetHardness(float hardness) {
    m_Hardness = max(0.0f, min(1.0f, hardness));
}

void PressureBrush::SetSpacing(float spacing) {
    m_Spacing = max(0.1f, spacing);
}

void PressureBrush::SetFlow(float flow) {
    m_Flow = max(0.0f, min(1.0f, flow));
}

void PressureBrush::UpdateWithPressure(float pressure) {
    // Adjust brush size based on pressure
    float pressureFactor = max(0.0f, min(1.0f, pressure));
    m_CurrentSize = m_MinSize + (m_MaxSize - m_MinSize) * pressureFactor;
}

void PressureBrush::ApplyStroke(float x, float y, float pressure, ID3D11DeviceContext* pContext) {
    // Determine if we should draw at this position based on spacing
    if (m_LastX != -1 && m_LastY != -1) {
        float dist = sqrt((x - m_LastX) * (x - m_LastX) + (y - m_LastY) * (y - m_LastY));
        if (dist < m_Spacing) {
            return; // Skip drawing if too close to last point
        }
    }
    
    // Update brush properties based on pressure
    UpdateWithPressure(pressure);
    
    // Store current position for next spacing check
    m_LastX = x;
    m_LastY = y;
    
    // In a real implementation, we would render the brush mark to the current render target
    // For now, we'll just log that the stroke was applied
}