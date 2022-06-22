#pragma once
#include "Forward.h"
#include <Keyboard.h>
#include <Mouse.h>
#include <SimpleMath.h>

namespace Framework
{
    class OrbitalCamera
    {
    public:
        OrbitalCamera(IGame* instance,
            std::unique_ptr<DirectX::Keyboard>& keyboard,
            std::unique_ptr<DirectX::Mouse>& mouse);

        void Update(DirectX::SimpleMath::Matrix& m_view,
            DirectX::SimpleMath::Matrix& m_proj);
    private:
        IGame* instance;
        DirectX::Keyboard* keyboard;
        DirectX::Mouse* mouse;

        DirectX::SimpleMath::Matrix view;
        DirectX::SimpleMath::Matrix proj;

        float m_theta;
        float m_phi;
        float m_radius;
    };
}
