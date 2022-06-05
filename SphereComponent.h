#pragma once
#include <d3d11.h>
#include <SimpleMath.h>
#include <GeometricPrimitive.h>
#include "XYZGameComponent.h"

namespace BaseFramework 
{
	class SphereComponent : public XYZGameComponent
	{
	public:
		// Inherited via XYZGameComponent
		void Initialize(IGame* game) override;
		void Update(DirectX::SimpleMath::Matrix m_world, DirectX::SimpleMath::Matrix m_view, DirectX::SimpleMath::Matrix m_proj) override;
		void Draw() override;
		void ClearResources() override;
		DirectX::SimpleMath::Matrix GetWorld() override { return m_world; };
		void Rotate(RotDirection direction, float angle) override;
		void Translate(float x, float y, float z) override;
	private:
		IGame* instance;

		DirectX::SimpleMath::Matrix tempMatrix = DirectX::SimpleMath::Matrix::Identity;
		
		DirectX::SimpleMath::Matrix m_world;
		DirectX::SimpleMath::Matrix m_view;
		DirectX::SimpleMath::Matrix m_proj;
		std::unique_ptr<DirectX::GeometricPrimitive> m_shape;
		//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	};
}


