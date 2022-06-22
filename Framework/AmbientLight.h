#pragma once
#include "Forward.h"
#include "ConstantBuffer.h"
#include "ConstBuffers.h"
#include "SimpleMath.h"

namespace Framework
{
    class AmbientLight
    {
    public:
        AmbientLight(DirectX::XMFLOAT3 color, float strength);
        void Initialize(Framework::IGame* instance);
        void Update(DirectX::XMFLOAT3 color, float strength);
        void Draw();

        ConstBuffers::PSAmbient data;
    private:
        ConstantBuffer<ConstBuffers::PSAmbient> pConstBuffer;
        Framework::Graphics* graphics;
    };
}
