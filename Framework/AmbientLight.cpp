#include "Forward.h"
#include "AmbientLight.h"

#include "Graphics.h"
#include "IGame.h"

namespace BaseFramework
{
    AmbientLight::AmbientLight(DirectX::SimpleMath::Vector3 color, float strength) : data(color, strength)
    {}

    void AmbientLight::Initialize(BaseFramework::IGame* instance)
    {
        this->graphics = instance->GetGfx();
        auto context = graphics->GetContext();
        auto device = graphics->GetDevice();
        
        pConstBuffer.Initialize(device.Get(), context.Get());
        pConstBuffer.data = data;
        pConstBuffer.ApplyChanges();
    }

    void AmbientLight::Update(DirectX::SimpleMath::Vector3 color, float strength)
    {
        this->data = ConstBuffers::PSAmbient(color, strength);
        pConstBuffer.data = data;
        pConstBuffer.ApplyChanges();
    }

    void AmbientLight::Draw()
    {
        auto context = graphics->GetContext();
        pConstBuffer.ApplyChanges();
        context->PSSetConstantBuffers(0,1u,pConstBuffer.GetAddressOf());
    }
}
