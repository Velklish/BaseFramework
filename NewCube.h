#pragma once

#include <d3d11.h>

#include "Forward.h"
#include <SimpleMath.h>
#include <wrl/client.h>

namespace BaseFramework
{
    class NewCube
    {
    public:
        void Initialize(IGame* game);
        void Draw();
        void Update(DirectX::SimpleMath::Matrix m_world, DirectX::SimpleMath::Matrix m_view, DirectX::SimpleMath::Matrix m_proj);
        
        //Microsoft::WRL::ComPtr<ID3D11Buffer> constBuffer;
        DirectX::SimpleMath::Vector3 position = DirectX::SimpleMath::Vector3(0,-6,0);
        IGame* instance;
        DirectX::SimpleMath::Matrix m_world;
        DirectX::SimpleMath::Matrix m_view;
        DirectX::SimpleMath::Matrix m_proj;

    private:
        std::vector<unsigned short> indices;
        
        Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer = nullptr;
        Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader = nullptr;
        Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader = nullptr;
        Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout = nullptr;
        Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer = nullptr;
        Microsoft::WRL::ComPtr<ID3D11Buffer> pColorConstBuffer;
    };
}
