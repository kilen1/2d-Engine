#pragma once
#include <d3d11_4.h>
#include <wrl/client.h>
#include "Sprite.h"
#include <vector>
#include <memory>

struct Vertex {
    float x, y, z;
    float u, v;
    float r, g, b, a;
};

class Renderer {
public:
    Renderer(GraphicsDevice* pGraphicsDevice);
    ~Renderer();

    bool Initialize();
    void Cleanup();

    // Sprite rendering
    void DrawSprite(Sprite* pSprite);
    void DrawLine(float x1, float y1, float x2, float y2, float thickness, float r, float g, float b, float a = 1.0f);
    void DrawCircle(float centerX, float centerY, float radius, float r, float g, float b, float a = 1.0f);
    
    // Batch rendering
    void Flush();

private:
    bool CreateShaders();
    bool CreateInputLayout();
    bool CreateVertexBuffer();
    bool CreateIndexBuffer();
    bool CreateConstantBuffers();
    bool CreateBlendStates();

    GraphicsDevice* m_pGraphicsDevice;
    
    // Shaders
    Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
    
    // Buffers
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer;
    
    // Blend states for transparency
    Microsoft::WRL::ComPtr<ID3D11BlendState> m_pBlendState;
    
    // Rendering data
    std::vector<Vertex> m_Vertices;
    std::vector<UINT> m_Indices;
    UINT m_VertexCount;
    UINT m_IndexCount;
};