
#include "PointLight.h"

Framework::PointLight::PointLight(
    DirectX::SimpleMath::Vector3 color,
    float strength,
    DirectX::SimpleMath::Vector3 position) : data(color,strength,position)
{
}

void Framework::PointLight::Initialize(Framework::IGame* instance)
{
    this->game = instance;
    auto graphics = instance->GetGfx();
    auto context = graphics->GetContext();
    auto device = graphics->GetDevice();

    model = new Model("Models/lamp.fbx", 0.02);
    model->Initialize(instance);
        
    pConstBuffer.Initialize(device.Get(), context.Get());
    pConstBuffer.data = data;
    pConstBuffer.data.position =  data.position;
    pConstBuffer.ApplyChanges();
}

void Framework::PointLight::Update(DirectX::SimpleMath::Vector3 color, float strength, DirectX::SimpleMath::Vector3 position)
{
    this->data = ConstBuffers::PSDynamicLight(color, strength, position);
    pConstBuffer.data = data;
    pConstBuffer.ApplyChanges();
    
    model->Update(game->GetWorldTransform());
}

void Framework::PointLight::Update()
{
    auto transform = game->GetWorldTransform();
    data.viewPosition = transform.cameraPosition;
    pConstBuffer.data = data;
    pConstBuffer.ApplyChanges();
    model->Translate(data.position.x,data.position.y,data.position.z);
    model->Update(game->GetWorldTransform());
}

void Framework::PointLight::Draw()
{
    auto context = game->GetGfx()->GetContext();
    pConstBuffer.data = data;
    pConstBuffer.ApplyChanges();
    model->Draw();
    context->PSSetConstantBuffers(2,1u,pConstBuffer.GetAddressOf());
    
}

