#pragma once

#include "Model.h"
#include "SimpleMath.h"

namespace Framework
{
    class PointLight
    {
    public:
        PointLight(DirectX::SimpleMath::Vector3 color, float strength, DirectX::SimpleMath::Vector3 position);
        void Initialize(IGame* instance);
        void Update(DirectX::SimpleMath::Vector3 color, float strength, DirectX::SimpleMath::Vector3 position);
        void Update();
        void Draw();

        ConstBuffers::PSDynamicLight data;
    private:
        ConstantBuffer<ConstBuffers::PSDynamicLight> pConstBuffer;
        IGame* game;
        Model* model;
    };
}
