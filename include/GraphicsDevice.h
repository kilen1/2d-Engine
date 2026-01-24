#pragma once
#include <dxgi1_4.h>
#include <d3d11_4.h>
#include <wrl/client.h>

class GraphicsDevice {
public:
    GraphicsDevice();
    ~GraphicsDevice();

    bool Initialize(HWND hwnd, UINT width = 1280, UINT height = 720);
    void Cleanup();

    // Getters
    ID3D11Device* GetDevice() const { return m_pDevice.Get(); }
    ID3D11DeviceContext* GetDeviceContext() const { return m_pDeviceContext.Get(); }
    IDXGISwapChain* GetSwapChain() const { return m_pSwapChain.Get(); }
    ID3D11RenderTargetView* GetRenderTargetView() const { return m_pRenderTargetView.Get(); }
    ID3D11DepthStencilView* GetDepthStencilView() const { return m_pDepthStencilView.Get(); }

    // Rendering methods
    void BeginFrame(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);
    void EndFrame();
    void Resize(UINT width, UINT height);

private:
    bool CreateRenderTargetView();
    bool CreateDepthStencilView();
    bool CreateRasterizerState();

    Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;
    Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_pRasterizerState;

    HWND m_hwnd;
    UINT m_width;
    UINT m_height;
};