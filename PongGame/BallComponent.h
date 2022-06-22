#pragma once
#include "Framework/GameComponent.h"
#include "SimpleMath.h"
#include "d3d11.h"
#include "wrl.h"

namespace PongGame
{
    class BallComponent : public Framework::GameComponent
    {
    public:
        enum BounceDirection
        {
            Vertical,
            Horizontal
        };
        
        BallComponent(float sizeX, float sizeY, float posX, float posY);
        void Initialize(Framework::IGame* game) override;
        void Update() override;
        void Draw() override;

        void ClearResources() override;
        void Bounce(BounceDirection dir);
        void Launch();

        DirectX::SimpleMath::Vector2 position = {};
        DirectX::SimpleMath::Rectangle collision;
 
        
    private:
        void Reset();
        
        std::vector<DirectX::SimpleMath::Vector2> vertices{};
        std::vector<int> indices{};
        DirectX::SimpleMath::Matrix transform;
        DirectX::SimpleMath::Vector2 velocity = {};
        DirectX::SimpleMath::Vector2 size {};

        int screenWidth;
        int screenHeight;

        Framework::IGame* instance;
        
        Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer = nullptr;
        Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader = nullptr;
        Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader = nullptr;
        Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout = nullptr;
        Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer = nullptr;
    };
}
