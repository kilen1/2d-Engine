#pragma once
#include <d3d11_4.h>
#include <wrl/client.h>
#include <string>

class Sprite {
public:
    Sprite();
    ~Sprite();

    bool LoadFromFile(const std::wstring& filename);
    bool CreateFromMemory(unsigned char* pData, size_t size);
    void Render(float x, float y, float scaleX = 1.0f, float scaleY = 1.0f, float rotation = 0.0f);
    
    // Getters
    float GetWidth() const { return m_Width; }
    float GetHeight() const { return m_Height; }

private:
    bool CreateTextureView();

    float m_Width;
    float m_Height;
    std::wstring m_Filename;
};