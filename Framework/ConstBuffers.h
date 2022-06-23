#pragma once
#include <SimpleMath.h>

namespace ConstBuffers
{
    struct PSAmbient
    {
        DirectX::XMFLOAT3 color;
        float lightStrength;

    };

    struct VSVertex
    {
        DirectX::SimpleMath::Matrix wvpMatrix;
        DirectX::SimpleMath::Matrix worldMatrix;
    };

    struct PSDynamicLight
    {
        DirectX::XMFLOAT3 color;
        float lightStrength;
        DirectX::SimpleMath::Vector3 position;
        DirectX::SimpleMath::Vector3 viewPosition; 
    };
}
