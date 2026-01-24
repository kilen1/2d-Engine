#include "../include/GraphicsDevice.h"
#include <dxgi1_4.h>
#include <d3dcompiler.h>

GraphicsDevice::GraphicsDevice() : 
    m_hwnd(nullptr), 
    m_width(1280), 
    m_height(720) {
}

GraphicsDevice::~GraphicsDevice() {
    Cleanup();
}

bool GraphicsDevice::Initialize(HWND hwnd, UINT width, UINT height) {
    m_hwnd = hwnd;
    m_width = width;
    m_height = height;

    // Create D3D11 device and context
    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferCount = 1;
    sd.BufferDesc.Width = m_width;
    sd.BufferDesc.Height = m_height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = m_hwnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL featureLevel;
    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        createDeviceFlags,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &sd,
        m_pSwapChain.ReleaseAndGetAddressOf(),
        m_pDevice.ReleaseAndGetAddressOf(),
        &featureLevel,
        m_pDeviceContext.ReleaseAndGetAddressOf()
    );

    if (FAILED(hr)) {
        return false;
    }

    // Create render target view
    if (!CreateRenderTargetView()) {
        return false;
    }

    // Create depth stencil view
    if (!CreateDepthStencilView()) {
        return false;
    }

    // Create rasterizer state
    if (!CreateRasterizerState()) {
        return false;
    }

    return true;
}

void GraphicsDevice::Cleanup() {
    m_pRenderTargetView.Reset();
    m_pDepthStencilView.Reset();
    m_pRasterizerState.Reset();
    m_pSwapChain.Reset();
    m_pDeviceContext.Reset();
    m_pDevice.Reset();
}

bool GraphicsDevice::CreateRenderTargetView() {
    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
    HRESULT hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
    
    if (FAILED(hr)) {
        return false;
    }

    hr = m_pDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, m_pRenderTargetView.ReleaseAndGetAddressOf());
    
    if (FAILED(hr)) {
        return false;
    }

    return true;
}

bool GraphicsDevice::CreateDepthStencilView() {
    Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer;
    D3D11_TEXTURE2D_DESC depthStencilDesc = {};
    depthStencilDesc.Width = m_width;
    depthStencilDesc.Height = m_height;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.SampleDesc.Quality = 0;
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    HRESULT hr = m_pDevice->CreateTexture2D(&depthStencilDesc, nullptr, depthStencilBuffer.GetAddressOf());
    
    if (FAILED(hr)) {
        return false;
    }

    hr = m_pDevice->CreateDepthStencilView(depthStencilBuffer.Get(), nullptr, m_pDepthStencilView.ReleaseAndGetAddressOf());
    
    if (FAILED(hr)) {
        return false;
    }

    return true;
}

bool GraphicsDevice::CreateRasterizerState() {
    D3D11_RASTERIZER_DESC rasterDesc = {};
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.CullMode = D3D11_CULL_NONE;
    rasterDesc.FrontCounterClockwise = FALSE;
    rasterDesc.DepthBias = 0;
    rasterDesc.DepthBiasClamp = 0.0f;
    rasterDesc.SlopeScaledDepthBias = 0.0f;
    rasterDesc.DepthClipEnable = TRUE;
    rasterDesc.ScissorEnable = FALSE;
    rasterDesc.MultisampleEnable = FALSE;
    rasterDesc.AntialiasedLineEnable = FALSE;

    HRESULT hr = m_pDevice->CreateRasterizerState(&rasterDesc, m_pRasterizerState.ReleaseAndGetAddressOf());
    
    if (FAILED(hr)) {
        return false;
    }

    m_pDeviceContext->RSSetState(m_pRasterizerState.Get());

    return true;
}

void GraphicsDevice::BeginFrame(float r, float g, float b, float a) {
    float color[4] = { r, g, b, a };
    m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), color);
    m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
    m_pDeviceContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
}

void GraphicsDevice::EndFrame() {
    m_pSwapChain->Present(1, 0);
}

void GraphicsDevice::Resize(UINT width, UINT height) {
    if (m_pDevice && m_pSwapChain) {
        m_pDeviceContext->OMSetRenderTargets(0, 0, 0);
        
        m_pRenderTargetView.Reset();
        m_pDepthStencilView.Reset();
        
        HRESULT hr = m_pSwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
        if (SUCCEEDED(hr)) {
            m_width = width;
            m_height = height;
            CreateRenderTargetView();
            CreateDepthStencilView();
        }
    }
}