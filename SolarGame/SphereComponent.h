#pragma once
#include <d3d11.h>
#include <SimpleMath.h>
#include <GeometricPrimitive.h>
#include "Framework/XYZGameComponent.h"

namespace SolarGame
{
	class SphereComponent : public Framework::XYZGameComponent
	{
	public:
		// Inherited via XYZGameComponent
		void Initialize(Framework::IGame* game) override;
		void Update(Framework::Transform transform) override;
		void Draw() override;
		void ClearResources() override;
		DirectX::SimpleMath::Matrix GetWorld() override { return m_world; };
		void Rotate(RotDirection direction, float angle) override;
		void Translate(float x, float y, float z) override;
	private:
		Framework::IGame* instance;

		DirectX::SimpleMath::Matrix tempMatrix = DirectX::SimpleMath::Matrix::Identity;
		
		DirectX::SimpleMath::Matrix m_world;
		DirectX::SimpleMath::Matrix m_view;
		DirectX::SimpleMath::Matrix m_proj;
		std::unique_ptr<DirectX::GeometricPrimitive> m_shape;
		//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	};
}


