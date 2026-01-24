#pragma once
#include <string>
#include <d3d11_4.h>
#include <wrl/client.h>

enum class BrushType {
    STANDARD,
    TEXTURED,
    SMUDGE,
    ERASER
};

class PressureBrush {
public:
    PressureBrush(const std::string& name, float minSize, float maxSize);
    ~PressureBrush();

    // Getters
    const std::string& GetName() const { return m_Name; }
    float GetMinSize() const { return m_MinSize; }
    float GetMaxSize() const { return m_MaxSize; }
    float GetCurrentSize() const { return m_CurrentSize; }
    BrushType GetType() const { return m_Type; }
    
    // Setters
    void SetName(const std::string& name) { m_Name = name; }
    void SetType(BrushType type) { m_Type = type; }
    void SetHardness(float hardness);  // 0.0 (soft) to 1.0 (hard)
    void SetSpacing(float spacing);    // Distance between brush marks
    void SetFlow(float flow);          // Opacity multiplier based on pressure
    
    // Update brush based on pressure
    void UpdateWithPressure(float pressure);
    
    // Apply brush stroke
    void ApplyStroke(float x, float y, float pressure, ID3D11DeviceContext* pContext);

private:
    std::string m_Name;
    float m_MinSize;
    float m_MaxSize;
    float m_CurrentSize;
    float m_Hardness;
    float m_Spacing;
    float m_Flow;
    BrushType m_Type;
    
    // Brush texture (for textured brushes)
    Microsoft::WRL::ComPtr<ID3D11Texture2D> m_pBrushTexture;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pBrushTextureSRV;
    
    // Last position for spacing calculation
    float m_LastX, m_LastY;
};