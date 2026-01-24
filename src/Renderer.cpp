#include "../include/Renderer.h"
#include <d3dcompiler.h>
#include <DirectXMath.h>
using namespace DirectX;

// Simple vertex shader
static const char* g_szVertexShader =
"cbuffer MatrixBuffer : register(b0) \
{ \
    matrix worldViewProjection; \
}; \
\
struct VS_INPUT \
{ \
    float3 pos : POSITION; \
    float2 tex : TEXCOORD0; \
    float4 col : COLOR0; \
}; \
\
struct PS_INPUT \
{ \
    float4 pos : SV_POSITION; \
    float2 tex : TEXCOORD0; \
    float4 col : COLOR0; \
}; \
\
PS_INPUT main(VS_INPUT input) \
{ \
    PS_INPUT output; \
    output.pos = mul(worldViewProjection, float4(input.pos, 1.0f)); \
    output.tex = input.tex; \
    output.col = input.col; \
    return output; \
}";

// Simple pixel shader
static const char* g_szPixelShader =
"struct PS_INPUT \
{ \
    float4 pos : SV_POSITION; \
    float2 tex : TEXCOORD0; \
    float4 col : COLOR0; \
}; \
\
Texture2D tex : register(t0); \
SamplerState sam : register(s0); \
\
float4 main(PS_INPUT input) : SV_TARGET \
{ \
    float4 textureColor = tex.Sample(sam, input.tex); \
    return textureColor * input.col; \
}";

Renderer::Renderer(GraphicsDevice* pGraphicsDevice) :
    m_pGraphicsDevice(pGraphicsDevice),
    m_VertexCount(0),
    m_IndexCount(0) {
}

Renderer::~Renderer() {
    Cleanup();
}

bool Renderer::Initialize() {
    if (!CreateShaders()) {
        return false;
    }

    if (!CreateInputLayout()) {
        return false;
    }

    if (!CreateVertexBuffer()) {
        return false;
    }

    if (!CreateIndexBuffer()) {
        return false;
    }

    if (!CreateConstantBuffers()) {
        return false;
    }

    if (!CreateBlendStates()) {
        return false;
    }

    return true;
}

void Renderer::Cleanup() {
    m_pVertexShader.Reset();
    m_pPixelShader.Reset();
    m_pInputLayout.Reset();
    m_pVertexBuffer.Reset();
    m_pIndexBuffer.Reset();
    m_pConstantBuffer.Reset();
    m_pBlendState.Reset();
}

bool Renderer::CreateShaders() {
    // Compile vertex shader
    Microsoft::WRL::ComPtr<ID3DBlob> pVSBlob = nullptr;
    HRESULT hr = D3DCompile(g_szVertexShader, strlen(g_szVertexShader), nullptr, nullptr, nullptr,
        "main", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, pVSBlob.GetAddressOf(), nullptr);

    if (FAILED(hr)) {
        return false;
    }

    hr = m_pGraphicsDevice->GetDevice()->CreateVertexShader(
        pVSBlob->GetBufferPointer(),
        pVSBlob->GetBufferSize(),
        nullptr,
        m_pVertexShader.ReleaseAndGetAddressOf()
    );

    if (FAILED(hr)) {
        return false;
    }

    // Compile pixel shader
    Microsoft::WRL::ComPtr<ID3DBlob> pPSBlob = nullptr;
    hr = D3DCompile(g_szPixelShader, strlen(g_szPixelShader), nullptr, nullptr, nullptr,
        "main", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, pPSBlob.GetAddressOf(), nullptr);

    if (FAILED(hr)) {
        return false;
    }

    hr = m_pGraphicsDevice->GetDevice()->CreatePixelShader(
        pPSBlob->GetBufferPointer(),
        pPSBlob->GetBufferSize(),
        nullptr,
        m_pPixelShader.ReleaseAndGetAddressOf()
    );

    if (FAILED(hr)) {
        return false;
    }

    return true;
}

bool Renderer::CreateInputLayout() {
    D3D11_INPUT_ELEMENT_DESC layout[] = {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    Microsoft::WRL::ComPtr<ID3DBlob> pVSBlob = nullptr;
    HRESULT hr = D3DCompile(g_szVertexShader, strlen(g_szVertexShader), nullptr, nullptr, nullptr,
        "main", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, pVSBlob.GetAddressOf(), nullptr);

    if (FAILED(hr)) {
        return false;
    }

    hr = m_pGraphicsDevice->GetDevice()->CreateInputLayout(
        layout, 3,
        pVSBlob->GetBufferPointer(),
        pVSBlob->GetBufferSize(),
        m_pInputLayout.ReleaseAndGetAddressOf()
    );

    if (FAILED(hr)) {
        return false;
    }

    return true;
}

bool Renderer::CreateVertexBuffer() {
    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(Vertex) * 10000; // Max vertices
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    HRESULT hr = m_pGraphicsDevice->GetDevice()->CreateBuffer(&bd, nullptr, m_pVertexBuffer.ReleaseAndGetAddressOf());

    return SUCCEEDED(hr);
}

bool Renderer::CreateIndexBuffer() {
    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(UINT) * 30000; // Max indices
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    HRESULT hr = m_pGraphicsDevice->GetDevice()->CreateBuffer(&bd, nullptr, m_pIndexBuffer.ReleaseAndGetAddressOf());

    return SUCCEEDED(hr);
}

bool Renderer::CreateConstantBuffers() {
    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(XMMATRIX);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    HRESULT hr = m_pGraphicsDevice->GetDevice()->CreateBuffer(&bd, nullptr, m_pConstantBuffer.ReleaseAndGetAddressOf());

    return SUCCEEDED(hr);
}

bool Renderer::CreateBlendStates() {
    D3D11_BLEND_DESC blendDesc = {};
    blendDesc.AlphaToCoverageEnable = false;
    blendDesc.RenderTarget[0].BlendEnable = true;
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    HRESULT hr = m_pGraphicsDevice->GetDevice()->CreateBlendState(&blendDesc, m_pBlendState.ReleaseAndGetAddressOf());

    return SUCCEEDED(hr);
}

void Renderer::DrawSprite(Sprite* pSprite) {
    if (!pSprite) return;

    // Calculate sprite vertices for a quad
    float halfWidth = pSprite->GetWidth() * 0.5f;
    float halfHeight = pSprite->GetHeight() * 0.5f;

    // Create vertices for the sprite quad
    Vertex vertices[4] = {
        {-halfWidth, -halfHeight, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f}, // Bottom-left
        { halfWidth, -halfHeight, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f}, // Bottom-right
        {-halfWidth,  halfHeight, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f}, // Top-left
        { halfWidth,  halfHeight, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f}  // Top-right
    };

    // Create indices for two triangles
    UINT indices[6] = { 0, 1, 2, 1, 3, 2 };

    // Add to batch
    for (int i = 0; i < 4; i++) {
        m_Vertices.push_back(vertices[i]);
    }
    for (int i = 0; i < 6; i++) {
        m_Indices.push_back(m_VertexCount + indices[i]);
    }
    m_VertexCount += 4;
    m_IndexCount += 6;
}

void Renderer::DrawLine(float x1, float y1, float x2, float y2, float thickness, float r, float g, float b, float a) {
    // Calculate direction vector
    float dx = x2 - x1;
    float dy = y2 - y1;
    float length = sqrt(dx * dx + dy * dy);
    
    if (length == 0.0f) return;
    
    // Normalize direction
    dx /= length;
    dy /= length;
    
    // Calculate perpendicular vector for line thickness
    float perpX = -dy * thickness * 0.5f;
    float perpY = dx * thickness * 0.5f;
    
    // Create vertices for a quad representing the line
    Vertex vertices[4] = {
        {x1 + perpX, y1 + perpY, 0.0f, 0.0f, 0.0f, r, g, b, a}, // Top-left
        {x1 - perpX, y1 - perpY, 0.0f, 0.0f, 1.0f, r, g, b, a}, // Bottom-left
        {x2 + perpX, y2 + perpY, 0.0f, 1.0f, 0.0f, r, g, b, a}, // Top-right
        {x2 - perpX, y2 - perpY, 0.0f, 1.0f, 1.0f, r, g, b, a}  // Bottom-right
    };
    
    // Indices for two triangles
    UINT indices[6] = {0, 1, 2, 1, 3, 2};
    
    // Add to batch
    for (int i = 0; i < 4; i++) {
        m_Vertices.push_back(vertices[i]);
    }
    for (int i = 0; i < 6; i++) {
        m_Indices.push_back(m_VertexCount + indices[i]);
    }
    m_VertexCount += 4;
    m_IndexCount += 6;
}

void Renderer::DrawCircle(float centerX, float centerY, float radius, float r, float g, float b, float a) {
    const int segments = 32;
    const float angleStep = 2.0f * 3.14159f / segments;
    
    // Create vertices for the circle (fan-style)
    Vertex centerVertex = {centerX, centerY, 0.0f, 0.5f, 0.5f, r, g, b, a};
    m_Vertices.push_back(centerVertex);
    
    // Add vertices around the circle
    for (int i = 0; i <= segments; i++) {
        float angle = i * angleStep;
        float x = centerX + cos(angle) * radius;
        float y = centerY + sin(angle) * radius;
        
        Vertex v = {x, y, 0.0f, 0.0f, 0.0f, r, g, b, a};
        m_Vertices.push_back(v);
    }
    
    // Create indices for triangle fan
    for (int i = 0; i < segments; i++) {
        m_Indices.push_back(m_VertexCount); // Center vertex
        m_Indices.push_back(m_VertexCount + i + 1);
        m_Indices.push_back(m_VertexCount + i + 2);
    }
    
    m_VertexCount += segments + 2;
    m_IndexCount += segments * 3;
}

void Renderer::Flush() {
    if (m_Vertices.empty() || m_Indices.empty()) {
        return;
    }

    // Map vertex buffer and copy data
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    HRESULT hr = m_pGraphicsDevice->GetDeviceContext()->Map(m_pVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    
    if (SUCCEEDED(hr)) {
        memcpy(mappedResource.pData, m_Vertices.data(), sizeof(Vertex) * m_Vertices.size());
        m_pGraphicsDevice->GetDeviceContext()->Unmap(m_pVertexBuffer.Get(), 0);
    }

    // Map index buffer and copy data
    hr = m_pGraphicsDevice->GetDeviceContext()->Map(m_pIndexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    
    if (SUCCEEDED(hr)) {
        memcpy(mappedResource.pData, m_Indices.data(), sizeof(UINT) * m_Indices.size());
        m_pGraphicsDevice->GetDeviceContext()->Unmap(m_pIndexBuffer.Get(), 0);
    }

    // Set up rendering pipeline
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    
    m_pGraphicsDevice->GetDeviceContext()->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);
    m_pGraphicsDevice->GetDeviceContext()->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    m_pGraphicsDevice->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_pGraphicsDevice->GetDeviceContext()->IASetInputLayout(m_pInputLayout.Get());
    
    // Set shaders
    m_pGraphicsDevice->GetDeviceContext()->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
    m_pGraphicsDevice->GetDeviceContext()->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
    
    // Set blend state for transparency
    float blendFactor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    m_pGraphicsDevice->GetDeviceContext()->OMSetBlendState(m_pBlendState.Get(), blendFactor, 0xFFFFFFFF);
    
    // Draw indexed
    m_pGraphicsDevice->GetDeviceContext()->DrawIndexed(m_IndexCount, 0, 0);
    
    // Clear batch data
    m_Vertices.clear();
    m_Indices.clear();
    m_VertexCount = 0;
    m_IndexCount = 0;
}