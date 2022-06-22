#pragma once

#include <SimpleMath.h>

class Vertex
{
public:
    Vertex() {}
    Vertex(DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector2 texCoord, DirectX::SimpleMath::Vector3 normal)
        : position(pos), texCoord(texCoord), normal(normal) {} 
    DirectX::SimpleMath::Vector3 position;
    //DirectX::SimpleMath::Vector3 normal;
    DirectX::SimpleMath::Vector2 texCoord;
    DirectX::SimpleMath::Vector3 normal;
};
