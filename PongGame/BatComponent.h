#pragma once
#include "Framework/GameComponent.h"
#include "SimpleMath.h"
#include "d3d11.h"
#include "wrl.h"

namespace PongGame
{
	class BatComponent : public Framework::GameComponent
	{
	public:
		// Inherited via GameComponent
		BatComponent(float sizeX, float sizeY, float posX, float posY);
		void Initialize(Framework::IGame* game) override;
		void Update() override;
		void Draw() override;
		void Translate(float x, float y);
		void ClearResources() override;

		DirectX::SimpleMath::Vector2 position = {};
		DirectX::SimpleMath::Rectangle collision;
	private:
		DirectX::SimpleMath::Vector2 velocity = {0.02f,0.02f};
		DirectX::SimpleMath::Vector2 size {};
		Framework::IGame* instance;
		std::vector<DirectX::SimpleMath::Vector2> vertices{};
		std::vector<int> indices{};

		DirectX::SimpleMath::Matrix transform;

		int screenWidth;
		int screenHeight;
		
		Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer = nullptr;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader = nullptr;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader = nullptr;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout = nullptr;
		Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer = nullptr;
		//Microsoft::WRL::ComPtr<ID3D11Buffer> pConstBuffer = nullptr;
	};
}

