#include "../include/Sprite.h"
#include <wincodec.h>
#include <wrl/client.h>

Sprite::Sprite() : m_Width(0.0f), m_Height(0.0f) {
}

Sprite::~Sprite() {
    // Resources will be automatically released through ComPtr
}

bool Sprite::LoadFromFile(const std::wstring& filename) {
    m_Filename = filename;
    
    // For now, we'll just set basic dimensions
    // In a real implementation, we would load the image and create the texture
    m_Width = 64.0f;
    m_Height = 64.0f;
    
    // This is a simplified version - in a real implementation, 
    // we would use WIC (Windows Imaging Component) to load the image
    // and create a texture from it
    
    return CreateTextureView();
}

bool Sprite::CreateFromMemory(unsigned char* pData, size_t size) {
    // Similar to loading from file, but from memory
    m_Width = 64.0f;
    m_Height = 64.0f;
    
    return CreateTextureView();
}

void Sprite::Render(float x, float y, float scaleX, float scaleY, float rotation) {
    // Rendering is handled by the Renderer class
    // This method would be called by the renderer when drawing sprites
}

bool Sprite::CreateTextureView() {
    // For this simplified implementation, we'll just set the dimensions
    // A full implementation would create the actual texture from image data
    m_Width = 64.0f;
    m_Height = 64.0f;
    
    // In a real implementation, we would create the texture using the graphics device
    // passed from outside since Sprite doesn't own the graphics device
    
    return true;
}