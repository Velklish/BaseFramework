#pragma once
#include <SimpleMath.h>

namespace Framework
{
    struct Transform
    {
        DirectX::SimpleMath::Matrix view;
        DirectX::SimpleMath::Matrix proj;
        DirectX::SimpleMath::Matrix world;
        DirectX::SimpleMath::Vector3 cameraPosition;
    };
}
