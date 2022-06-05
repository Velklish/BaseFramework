#include "pch.h"

#include "OrbitalCamera.h"
#include "Window.h"
#include "IGame.h"

using namespace DirectX;
using namespace SimpleMath;

namespace
{
    constexpr float ROTATION_GAIN = 0.004f;
    constexpr float MOVEMENT_GAIN = 0.07f;

    constexpr float c_defaultPhi = XM_2PI / 6.0f;
    constexpr float c_defaultRadius = 3.3f;
    constexpr float c_minRadius = 0.1f;
    constexpr float c_maxRadius = 5.f;
}

BaseFramework::OrbitalCamera::OrbitalCamera(
    IGame* instance,
    std::unique_ptr<Keyboard>& keyboard,
    std::unique_ptr<Mouse>& mouse)
    :   m_theta(0.f),
        m_phi(c_defaultPhi),
        m_radius(c_defaultRadius)
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

void BaseFramework::OrbitalCamera::Update(Matrix& m_view, Matrix& m_proj)
{
    auto mouse = this->mouse->GetState();
    
    m_radius -= float(mouse.scrollWheelValue) * ROTATION_GAIN;

    m_radius = std::max(c_minRadius, std::min(c_maxRadius, m_radius));

    if (mouse.positionMode == Mouse::MODE_RELATIVE)
    {
        Vector3 delta = Vector3(float(mouse.x), float(mouse.y), 0.f)
            * ROTATION_GAIN;

        m_phi -= delta.y;
        m_theta -= delta.x;
    }

    this->mouse->SetMode(mouse.leftButton ?
      Mouse::MODE_RELATIVE : Mouse::MODE_ABSOLUTE);

    auto kb = keyboard->GetState();

    if (kb.Home)
    {
        m_theta = 0.f;
        m_phi = c_defaultPhi;
        m_radius = c_defaultRadius;
    }

    Vector3 move = Vector3::Zero;

    if (kb.Up || kb.W)
        move.y += 1.f;

    if (kb.Down || kb.S)
        move.y -= 1.f;

    if (kb.Left || kb.A)
        move.x -= 1.f;

    if (kb.Right || kb.D)
        move.x += 1.f;

    if (kb.PageUp || kb.Space)
        move.z += 1.f;

    if (kb.PageDown || kb.X)
        move.z -= 1.f;

    move *= MOVEMENT_GAIN;

    m_phi -= move.y;
    m_theta -= move.x;
    m_radius += move.z;

    // limit pitch to straight up or straight down
    constexpr float limit = XM_PIDIV2 - 0.01f;
    m_phi = std::max(1e-2f, std::min(limit, m_phi));

    if (m_theta > XM_PI)
    {
        m_theta -= XM_2PI;
    }
    else if (m_theta < -XM_PI)
    {
        m_theta += XM_2PI;
    }

    XMVECTOR lookFrom = XMVectorSet(
        m_radius * sinf(m_phi) * cosf(m_theta),
        m_radius * cosf(m_phi),
        m_radius * sinf(m_phi) * sinf(m_theta),
        0);

    view = XMMatrixLookAtRH(lookFrom, g_XMZero, Vector3::Up);

    m_view = view;
    m_proj = proj;
}
