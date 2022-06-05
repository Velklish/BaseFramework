﻿#include "NewCube.h"

#include <d3dcompiler.h>
#include <iterator>

#include "Graphics.h"
#include "IGame.h"

using namespace DirectX::SimpleMath;
using namespace BaseFramework;

void BaseFramework::NewCube::Initialize(IGame* game)
{
    this->instance = game;

    auto graphics = instance->GetGfx();
    auto device = graphics->GetDevice();
    auto context = graphics->GetContext();
    auto target = graphics->GetTarget();
    auto dsv = graphics->GetDSV();

    const Vector3 vertices[] =
    {
        {-0.5f, -0.5f, -0.5f},
        {0.5f, -0.5f, -0.5f},
        {-0.5f, 0.5f, -0.5f},
        {0.5f, 0.5f, -0.5f},
        {-0.5f, -0.5f, 0.5f},
        {0.5f, -0.5f, 0.5f},
        {-0.5f, 0.5f, 0.5f},
        {0.5f, 0.5f, 0.5f},
    };

    D3D11_BUFFER_DESC bDesc;
    bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bDesc.Usage = D3D11_USAGE_DEFAULT;
    bDesc.CPUAccessFlags = 0u;
    bDesc.MiscFlags = 0u;
    bDesc.ByteWidth = sizeof vertices;
    bDesc.StructureByteStride = sizeof Vector3;

    D3D11_SUBRESOURCE_DATA sd = {};
    sd.pSysMem = vertices;
    sd.SysMemPitch = 0;
    sd.SysMemSlicePitch = 0;

    device->CreateBuffer(&bDesc, &sd, &pVertexBuffer);

    indices =
    {
        0,2,1, 2,3,1,
        1,3,5, 3,7,5,
        2,6,3, 3,6,7,
        4,5,7, 4,7,6,
        0,4,2, 2,4,6,
        0,1,4, 1,5,4
        };
    
    D3D11_BUFFER_DESC ibd = {};
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.Usage = D3D11_USAGE_DEFAULT;
    ibd.CPUAccessFlags = 0u;
    ibd.ByteWidth = sizeof(unsigned short) * std::size(indices);
    ibd.StructureByteStride = sizeof(unsigned short);
    D3D11_SUBRESOURCE_DATA isd;
    isd.pSysMem = indices.data();
    isd.SysMemPitch = 0;
    isd.SysMemSlicePitch = 0;

    device->CreateBuffer(&ibd, &isd, &pIndexBuffer);

    //Pixe Constant Buffer
    Vector4 colors[6]{
        {1.0f,0.0f,1.0f,1.0f},
        {1.0f,0.0f,0.0f,1.0f},
        {0.0f,1.0f,0.0f,1.0f},
        {0.0f,0.0f,1.0f,1.0f},
        {1.0f,1.0f,0.0f,1.0f},
        {0.0f,1.0f,1.0f,1.0f},
    };
    
    D3D11_BUFFER_DESC cbd2 = {};
    cbd2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbd2.Usage = D3D11_USAGE_DYNAMIC;
    cbd2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    cbd2.MiscFlags = 0u;
    cbd2.ByteWidth = sizeof colors;
    cbd2.StructureByteStride = 0u;
    D3D11_SUBRESOURCE_DATA csd2;
    csd2.pSysMem = &colors;
    device->CreateBuffer(&cbd2, &csd2, &pColorConstBuffer);
    
    const D3D11_INPUT_ELEMENT_DESC inputElements[] = {
        {"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };
    
    Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
    
    D3DReadFileToBlob(L"PixelShader.cso", &pBlob);
    device->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);

    D3DReadFileToBlob(L"VertexShader.cso", &pBlob);
    device->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);

    device->CreateInputLayout(inputElements, std::size(inputElements), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout);
}

void BaseFramework::NewCube::Draw()
{
    auto graphics = instance->GetGfx();
    auto device = graphics->GetDevice();
    auto context = graphics->GetContext();
    auto target = graphics->GetTarget();
    auto dsv = graphics->GetDSV();
    
    const UINT stride = sizeof Vector3;
    const UINT offset = 0u;
    context->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
    context->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);

    Matrix cb =
    {
            (m_world * m_view * m_proj).Transpose()
    };

    Microsoft::WRL::ComPtr<ID3D11Buffer> pConstBuffer;
    D3D11_BUFFER_DESC cbd = {};
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbd.Usage = D3D11_USAGE_DYNAMIC;
    cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    cbd.MiscFlags = 0u;
    cbd.ByteWidth = sizeof cb;
    cbd.StructureByteStride = 0u;
    D3D11_SUBRESOURCE_DATA csd;
    csd.pSysMem = &cb;

    device->CreateBuffer(&cbd, &csd, &pConstBuffer);
    context->VSSetConstantBuffers(0, 1u, pConstBuffer.GetAddressOf());
    context->PSSetConstantBuffers(0, 1u, pColorConstBuffer.GetAddressOf());
    context->VSSetShader(pVertexShader.Get(), nullptr, 0u);
    context->PSSetShader(pPixelShader.Get(), nullptr, 0u);
    context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    context->IASetInputLayout(pInputLayout.Get());
    context->DrawIndexed((UINT)std::size(indices), 0u, 0u);
}

void BaseFramework::NewCube::Update(Matrix m_world, Matrix m_view, Matrix m_proj)
{
    this->m_world =  m_world;
    this->m_view = m_view;
    this->m_proj = m_proj;
}
