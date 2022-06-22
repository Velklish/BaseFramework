#pragma once
#include <SimpleMath.h>
#include <GeometricPrimitive.h>
#include <wrl/client.h>

#include "Framework/XYZGameComponent.h"

namespace SolarGame
{
	class CubeComponent : public Framework::XYZGameComponent
	{
	public:
		// Inherited via XYZGameComponent
		CubeComponent(float size = 1.0f);
		void Initialize(Framework::IGame* game) override;
		void Update(Framework::Transform transform) override;
		void Draw() override;
		void ClearResources() override;
		DirectX::SimpleMath::Matrix GetWorld() override { return m_world; }
		void Rotate(RotDirection direction, float angle) override;
		void Translate(float x, float y, float z) override;
	private:
		Framework::IGame* instance;
		float size = 1.0f;
		
		DirectX::SimpleMath::Matrix tempMatrix = DirectX::SimpleMath::Matrix::Identity;
		
		DirectX::SimpleMath::Matrix m_world = DirectX::SimpleMath::Matrix::Identity;
		DirectX::SimpleMath::Matrix m_view = DirectX::SimpleMath::Matrix::Identity;
		DirectX::SimpleMath::Matrix m_proj = DirectX::SimpleMath::Matrix::Identity;

		std::vector<unsigned short> indices;
        
		Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer = nullptr;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader = nullptr;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader = nullptr;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout = nullptr;
		Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer = nullptr;
		Microsoft::WRL::ComPtr<ID3D11Buffer> pColorConstBuffer = nullptr;
	};
}

