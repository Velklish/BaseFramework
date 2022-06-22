#include "BatComponent.h"
#include "Framework/IGame.h"
#include "Framework/Graphics.h"
#include "Framework/Window.h"
#include <d3dcompiler.h>

#include "Framework/pch.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

using namespace DirectX::SimpleMath;
using namespace Framework;

PongGame::BatComponent::BatComponent(float sizeX, float sizeY, float posX, float posY)
	: position(posX, posY),
	size(sizeX, sizeY)
{
}

void PongGame::BatComponent::Initialize(IGame* game)
{
    this->instance = game;
	
	auto graphics = instance->GetGfx();
	auto device = graphics->GetDevice();
	auto context = graphics->GetContext();
	auto target = graphics->GetTarget();
	auto dsv = graphics->GetDSV();

	const Vector2 vertices[] =
	{
		Vector2(-(size.x/2), -(size.y/2)),
		Vector2(-(size.x/2), (size.y/2)),
		Vector2((size.x/2), -(size.y/2)),
		Vector2((size.x/2), (size.y/2)),
	};

	D3D11_BUFFER_DESC bDesc;
	bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bDesc.Usage = D3D11_USAGE_DEFAULT;
	bDesc.CPUAccessFlags = 0u;
	bDesc.MiscFlags = 0u;
	bDesc.ByteWidth = sizeof vertices;
	bDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices;
	sd.SysMemPitch = 0;
	sd.SysMemSlicePitch = 0;

	device->CreateBuffer(&bDesc, &sd, &pVertexBuffer);

	indices = { 0,1,2, 1,3,2 };
	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.ByteWidth = sizeof(int) * std::size(indices);
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA isd;
	isd.pSysMem = indices.data();
	isd.SysMemPitch = 0;
	isd.SysMemSlicePitch = 0;

	device->CreateBuffer(&ibd, &isd, &pIndexBuffer);
	
	const D3D11_INPUT_ELEMENT_DESC inputElements[] = {
		{"Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
    
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;

	D3DReadFileToBlob(L"PixelShader.cso", &pBlob);
	device->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);

	D3DReadFileToBlob(L"VertexShader.cso", &pBlob);
	device->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);
	
	device->CreateInputLayout(inputElements, std::size(inputElements), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout);
	
	instance->GetWindow()->GetSize(screenWidth, screenHeight);
	
	collision.x = (position.x - size.x/2) * screenWidth/2 ;
	collision.y = (position.y - size.y/2) * screenHeight/2;
	collision.width = size.x * (screenWidth / 2);
	collision.height = size.y * screenHeight / 2;
}

void PongGame::BatComponent::Update()
{
    transform = Matrix::CreateTranslation(Vector3(position.x, position.y, 0)).Transpose();
}

void PongGame::BatComponent::Draw()
{
    auto graphics = instance->GetGfx();
    auto device = graphics->GetDevice();
    auto context = graphics->GetContext();
    auto target = graphics->GetTarget();
    auto dsv = graphics->GetDSV();
	
    //bind vertex buffer to pipeline
    const UINT stride = sizeof Vector2;
    const UINT offset = 0u;
    context->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
    context->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0u);
 
    //Vertex Constant Buffer
    Matrix cb = transform;

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

	context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    context->OMSetRenderTargets(1u, target.GetAddressOf(), dsv.Get());
	context->PSSetShader(pPixelShader.Get(), nullptr, 0u);
	context->VSSetShader(pVertexShader.Get(), nullptr, 0u);
	context->IASetInputLayout(pInputLayout.Get());
    context->DrawIndexed((UINT)std::size(indices), 0u, 0u);
}

void PongGame::BatComponent::Translate(float x, float y)
{
	auto newPosX = position.x + x * velocity.x;
	auto newPosY = position.y + y * velocity.y;
	
	auto newColisX = (newPosX - size.x/2) * (screenWidth / 2);
	auto newColisY = (newPosY - size.y/2) * (screenHeight / 2);
	
	if(abs(newColisX + collision.width) > screenWidth/2
		|| abs(newColisX) > screenWidth / 2
		|| abs(newPosX) > 1.0f)
	{
		return;
	}
	
	position.x = newPosX;
	position.y = newPosY;
	
	collision.x = newColisX;
	collision.y = newColisY;
}

void PongGame::BatComponent::ClearResources()
{
}
