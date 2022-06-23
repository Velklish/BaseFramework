#include "Framework/Graphics.h"
#include "Framework/pch.h"

using namespace Framework;

Graphics::Graphics(int width, int height, HWND hwnd)
{
    DXGI_SWAP_CHAIN_DESC sd;
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = 2;
    sd.OutputWindow = hwnd;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    sd.Flags = 0;

    UINT swapCreateFlags = 0u;
    swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;

    D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        D3D11_CREATE_DEVICE_DEBUG,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &sd,
        &pSwap,
        &pDevice,
        nullptr,
        &pContext);

    Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer = nullptr;
    pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer);
    pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pTarget);

    //z buffer
    D3D11_DEPTH_STENCIL_DESC dsDesc = {};
    dsDesc.DepthEnable = TRUE;
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDsState;
    pDevice->CreateDepthStencilState(&dsDesc, &pDsState);

    pContext->OMSetDepthStencilState(pDsState.Get(), 1u);

    //depth texture
    Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencil = {};
    D3D11_TEXTURE2D_DESC descDepth = {};
    descDepth.Width = width;
    descDepth.Height = height;
    descDepth.MipLevels = 1u;
    descDepth.ArraySize = 1u;
    descDepth.Format = DXGI_FORMAT_D32_FLOAT;
    descDepth.SampleDesc.Count = 1u;
    descDepth.SampleDesc.Quality = 0u;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil);

    //depth texture view
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
    descDSV.Format = DXGI_FORMAT_D32_FLOAT;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0u;

    pDevice->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, &pDepthStencilView);

    pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), pDepthStencilView.Get());

    D3D11_VIEWPORT viewport = {};
    viewport.Width = width;
    viewport.Height = height;
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.MinDepth = 0;
    viewport.MaxDepth = 1.0f;
    pContext->RSSetViewports(1u, &viewport);

    CD3D11_RASTERIZER_DESC rastDesc = {};
    rastDesc.CullMode = D3D11_CULL_NONE;
    rastDesc.FillMode = D3D11_FILL_SOLID;

    ID3D11RasterizerState* rastState;
    pDevice->CreateRasterizerState(&rastDesc, &rastState);
    D3D11_SAMPLER_DESC sampDesc;
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    pDevice->CreateSamplerState(&sampDesc, &pSamplerState);
    pContext->PSSetSamplers(0,1,pSamplerState.GetAddressOf());
    pContext->RSSetState(rastState);
}

void Graphics::Present()
{
    DX::ThrowIfFailed(pSwap->Present(1, 0));
}

void Graphics::ClearBuffer()
{
    const float color[] = { 0.145, 0.474, 0.831, 1 };
    //const float color[] = { 0, 0, 0, 1 };
    pContext->ClearRenderTargetView(pTarget.Get(), color);
    pContext->ClearDepthStencilView(pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
    pContext->OMSetRenderTargets(1, pTarget.GetAddressOf(), pDepthStencilView.Get());
    //pContext->PSSetSamplers(0,1,pSamplerState.GetAddressOf());
}
