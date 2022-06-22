#pragma once
#include "Forward.h"
#include <Keyboard.h>
#include <Mouse.h>
#include <SimpleMath.h>

namespace Framework
{
    class FPSCamera
    {
    public:
        FPSCamera(IGame* instance,
            std::unique_ptr<DirectX::Keyboard>& keyboard,
            std::unique_ptr<DirectX::Mouse>& mouse);

        void Update(DirectX::SimpleMath::Matrix& m_view, DirectX::SimpleMath::Matrix& m_proj);

    private:
        IGame* instance;
        DirectX::Keyboard* keyboard;
        DirectX::Mouse* mouse;

        DirectX::SimpleMath::Matrix view;
        DirectX::SimpleMath::Matrix proj;

        float m_pitch;
        float m_yaw;
        DirectX::SimpleMath::Vector3 m_cameraPos;
    };
}

