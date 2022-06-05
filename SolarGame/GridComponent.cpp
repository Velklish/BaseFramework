#include "GridComponent.h"

#include "Framework/Graphics.h"
#include <DirectXHelpers.h>
#include "Framework/IGame.h"
#include "Framework/Window.h"

using namespace DirectX;
using namespace SimpleMath;

void SolarGame::GridComponent::Initialize(BaseFramework::IGame* game, float size)
{
    this->instance = game;
    this->size = size;

    auto device = this->instance->GetGfx()->GetDevice();
    m_world = Matrix::CreateTranslation(0.0f,-0.5f,0.0f);

    m_states = std::make_unique<CommonStates>(device.Get());

    m_effect = std::make_unique<BasicEffect>(device.Get());
    m_effect->SetVertexColorEnabled(true);
    
        CreateInputLayoutFromEffect<VertexPositionColor>(device.Get(), m_effect.get(),
            m_inputLayout.ReleaseAndGetAddressOf());

    auto context = this->instance->GetGfx()->GetContext();
    m_batch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(context.Get());

    int width;
    int height;
    this->instance->GetWindow()->GetSize(width,height);

    m_effect->SetView(m_view);
    m_effect->SetProjection(m_proj);
}

void SolarGame::GridComponent::Update(Matrix m_view,
    Matrix m_proj)
{
    this->m_view = m_view;
    this->m_proj = m_proj;

    m_effect->SetView(m_view);
    m_effect->SetProjection(m_proj);
}

void SolarGame::GridComponent::Draw()
{
    auto context = this->instance->GetGfx()->GetContext();
    context->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
    context->RSSetState(m_states->CullNone());

    m_effect->SetWorld(m_world);

    m_effect->Apply(context.Get());

    context->IASetInputLayout(m_inputLayout.Get());

    m_batch->Begin();

    Vector3 xaxis(size, 0.f, 0.f);
    Vector3 yaxis(0.f, 0.f, size);
    Vector3 origin = Vector3::Zero;

    constexpr size_t divisions = 20;

    for (size_t i = 0; i <= divisions; ++i)
    {
        float fPercent = float(i) / float(divisions);
        fPercent = (fPercent * 2.0f) - 1.0f;

        Vector3 scale = xaxis * fPercent + origin;

        VertexPositionColor v1(scale - yaxis, Colors::White);
        VertexPositionColor v2(scale + yaxis, Colors::White);
        m_batch->DrawLine(v1, v2);
    }

    for (size_t i = 0; i <= divisions; i++)
    {
        float fPercent = float(i) / float(divisions);
        fPercent = (fPercent * 2.0f) - 1.0f;

        Vector3 scale = yaxis * fPercent + origin;

        VertexPositionColor v1(scale - xaxis, Colors::White);
        VertexPositionColor v2(scale + xaxis, Colors::White);
        m_batch->DrawLine(v1, v2);
    }

    m_batch->End();
}

void SolarGame::GridComponent::ClearResources()
{
}