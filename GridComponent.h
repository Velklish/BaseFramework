#pragma once
#include <SimpleMath.h>
#include <GeometricPrimitive.h>
#include <wrl.h>
#include "CommonStates.h"
#include "Effects.h"
#include "PrimitiveBatch.h"

#include "XYZGameComponent.h"

namespace BaseFramework
{
    class GridComponent
    {
    public:
        void Initialize(IGame* game, float size = 4.0f);
        void Update(DirectX::SimpleMath::Matrix m_view,
            DirectX::SimpleMath::Matrix m_proj);
        void Draw();
        void ClearResources();
    private:
        IGame* instance;
        float size;
		
        std::unique_ptr<DirectX::CommonStates> m_states;
        std::unique_ptr<DirectX::BasicEffect> m_effect;
        std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_batch;

        Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

        DirectX::SimpleMath::Matrix m_world;
        DirectX::SimpleMath::Matrix m_view;
        DirectX::SimpleMath::Matrix m_proj;
    };
}
