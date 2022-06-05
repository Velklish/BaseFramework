#include "SphereComponent.h"

#include "Graphics.h"
#include "IGame.h"
#include "Window.h"

using namespace DirectX::SimpleMath;

void BaseFramework::SphereComponent::Initialize(IGame* game)
{
	this->instance = game;
    m_shape = DirectX::GeometricPrimitive::CreateSphere(instance->GetGfx()->GetContext().Get(), 0.5f);

    m_world = DirectX::SimpleMath::Matrix::Identity;

    int width, height;
    instance->GetWindow()->GetSize(width,height);
    m_view = Matrix::CreateLookAt(DirectX::SimpleMath::Vector3(2.f, 2.f, 2.f),
        Vector3::Zero, DirectX::SimpleMath::Vector3::UnitY);
    m_proj = Matrix::CreatePerspectiveFieldOfView(DirectX::XM_PI / 4.f,
        float(width) / float(height), 0.1f, 10.f);
}

void BaseFramework::SphereComponent::Update(DirectX::SimpleMath::Matrix m_world, DirectX::SimpleMath::Matrix m_view, DirectX::SimpleMath::Matrix m_proj)
{
    this->m_view = m_view;
    this->m_proj = m_proj;
    this->m_world = tempMatrix * m_world;
    tempMatrix = Matrix::Identity;
}

void BaseFramework::SphereComponent::Draw()
{
    m_shape->Draw(m_world, m_view, m_proj);
}

void BaseFramework::SphereComponent::ClearResources()
{
}

void BaseFramework::SphereComponent::Rotate(RotDirection direction, float angle)
{
    switch (direction)
    {
    case X: tempMatrix*=Matrix::CreateRotationX(DirectX::XMConvertToRadians(angle));
    case Y: tempMatrix*=Matrix::CreateRotationY(DirectX::XMConvertToRadians(angle));
    case Z: tempMatrix*=Matrix::CreateRotationZ(DirectX::XMConvertToRadians(angle)); 
    }
}

void BaseFramework::SphereComponent::Translate(float x, float y, float z)
{
    tempMatrix *= Matrix::CreateTranslation(x, y, z);
}
