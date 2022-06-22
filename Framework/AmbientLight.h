#pragma once
#include "Forward.h"
#include "Color.h"
#include "ConstantBuffer.h"
#include "ConstBuffers.h"
#include "SimpleMath.h"

namespace BaseFramework
{
    class AmbientLight
    {
    public:
        AmbientLight(DirectX::SimpleMath::Vector3 color, float strength);
        void Initialize(BaseFramework::IGame* instance);
        void Update(DirectX::SimpleMath::Vector3 color, float strength);
        void Draw();

        ConstBuffers::PSAmbient data;
    private:
        ConstantBuffer<ConstBuffers::PSAmbient> pConstBuffer;
        BaseFramework::Graphics* graphics;
    };
}
