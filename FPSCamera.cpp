#include "pch.h"

#include "FPSCamera.h"
#include "Window.h"
#include "IGame.h"

using namespace DirectX;
using namespace SimpleMath;

namespace
{
    const Vector3 START_POSITION = { 0.f, 1.5f, -5.5f};
    constexpr float ROTATION_GAIN = 0.004f;
    constexpr float MOVEMENT_GAIN = 0.07f;
}

BaseFramework::FPSCamera::FPSCamera(
    IGame* instance,
    std::unique_ptr<Keyboard>& keyboard,
    std::unique_ptr<Mouse>& mouse)
    :
    m_pitch(0),
    m_yaw(0),
    m_cameraPos(START_POSITION)
{
    this->instance = instance;
    this->keyboard = keyboard.get();
    this->mouse = mouse.get();

    auto hwnd = instance->GetWindow()->GetHwnd();
    mouse->SetWindow(hwnd);

    int width;
    int height;
    instance->GetWindow()->GetSize(width,height);
    proj = Matrix::CreatePerspectiveFieldOfView(
        XMConvertToRadians(70.f),
        float(width) / float(height), 0.01f, 100.f);
}

void BaseFramework::FPSCamera::Update(Matrix& m_view, Matrix& m_proj)
{
    auto mouse = this->mouse->GetState();

    if (mouse.positionMode == Mouse::MODE_RELATIVE)
    {
        Vector3 delta = Vector3(float(mouse.x), float(mouse.y), 0.f)
                        * ROTATION_GAIN;

        m_pitch -= delta.y;
        m_yaw -= delta.x;
    }

    this->mouse->SetMode(mouse.leftButton
        ? Mouse::MODE_RELATIVE : Mouse::MODE_ABSOLUTE);
    
    constexpr float limit = XM_PIDIV2 - 0.01f;
    m_pitch = std::max(-limit, m_pitch);
    m_pitch = std::min(+limit, m_pitch);

    auto kb = keyboard->GetState();

    if (kb.Home)
    {
        m_cameraPos = START_POSITION;
        m_pitch = m_yaw = 0;
    }

    Vector3 move = Vector3::Zero;

    if (kb.Up || kb.W)
        move.z += 1.f;

    if (kb.Down || kb.S)
        move.z -= 1.f;

    if (kb.Left || kb.A)
        move.x += 1.f;

    if (kb.Right || kb.D)
        move.x -= 1.f;

    if (kb.PageUp || kb.Space)
        move.y += 1.f;

    if (kb.PageDown || kb.X)
        move.y -= 1.f;

    Quaternion q = Quaternion::CreateFromYawPitchRoll(m_yaw, m_pitch, 0.f);

    move = Vector3::Transform(move, q);

    move *= MOVEMENT_GAIN;

    m_cameraPos += move;

    // keep longitude in sane range by wrapping
    if (m_yaw > XM_PI)
    {
        m_yaw -= XM_2PI;
    }
    else if (m_yaw < -XM_PI)
    {
        m_yaw += XM_2PI;
    }

    float y = sinf(m_pitch);
    float r = cosf(m_pitch);
    float z = r * cosf(m_yaw);
    float x = r * sinf(m_yaw);

    XMVECTOR lookAt = m_cameraPos + Vector3(x, y, z);

    view = XMMatrixLookAtRH(m_cameraPos, lookAt, Vector3::Up);

    m_view = view;
    m_proj = proj;
}
