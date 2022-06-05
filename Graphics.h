#pragma once
#include "Windows.h"
#include <wrl.h>
#include <d3d11.h>
#include <d3d11_1.h>
namespace BaseFramework
{
    class Graphics
    {
    public:
        Graphics(int width, int height, HWND hwnd);
        void Present();
        void ClearBuffer();
        Microsoft::WRL::ComPtr<ID3D11Device> GetDevice() { return pDevice; }
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetContext() { return pContext; }
        Microsoft::WRL::ComPtr<IDXGISwapChain> GetSwapChain() { return pSwap; }
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView> GetTarget() { return pTarget; }
        Microsoft::WRL::ComPtr<ID3D11DepthStencilView> GetDSV() { return pDepthStencilView; }
    private:
        Microsoft::WRL::ComPtr<ID3D11Device> pDevice = nullptr;
        Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap = nullptr;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext = nullptr;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext1> pContext1 = nullptr;
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget = nullptr;
        Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDepthStencilView = nullptr;
    };
}

