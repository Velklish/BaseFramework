#pragma once
#include <SimpleMath.h>
#include <GeometricPrimitive.h>
#include <wrl/client.h>

#include "XYZGameComponent.h"

namespace BaseFramework
{
	class CubeComponent : public XYZGameComponent
	{
	public:
		// Inherited via XYZGameComponent
		CubeComponent(float size = 1.0f);
		void Initialize(IGame* game) override;
		void Update(DirectX::SimpleMath::Matrix m_world, DirectX::SimpleMath::Matrix m_view, DirectX::SimpleMath::Matrix m_proj) override;
		void Draw() override;
		void ClearResources() override;
		DirectX::SimpleMath::Matrix GetWorld() override { return m_world; }
		void Rotate(RotDirection direction, float angle) override;
		void Translate(float x, float y, float z) override;
	private:
		IGame* instance;
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

