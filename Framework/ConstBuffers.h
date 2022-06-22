#pragma once
#include <DirectXCollision.h>
#include <SimpleMath.h>

namespace ConstBuffers
{
    struct PSAmbient
    {
        DirectX::SimpleMath::Vector3 color;
        float lightStrength;
    };
}
