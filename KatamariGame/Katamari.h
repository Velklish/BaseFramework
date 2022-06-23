#pragma once
#include <Keyboard.h>
#include <map>
#include <Mouse.h>

#include "Model.h"
#include "Framework/AmbientLight.h"
#include "Framework/Forward.h"
#include "Framework/FPSCamera.h"
#include "Framework/IGame.h"
#include "Framework/OrbitalCamera.h"
#include "Framework/XYZGameComponent.h"
#include "Framework/GridComponent.h"
#include "Framework/Model.h"
#include "Framework/PointLight.h"
#include "Framework/Transform.h"

class Katamari : Framework::IGame
{
public:
    void Initialize(int width, int height, LPCWSTR name = L"Katamari") override;
    void Update(Framework::DX::StepTimer const& timer) override;
    void Render() override;
    void Tick() override;
    Framework::Graphics* GetGfx() override;
    Framework::Window* GetWindow() override;
    void ClearResources() override;
    Framework::Transform GetWorldTransform() override;
private:
    void HandleInput() override;
    Framework::Model* player;
    DirectX::BoundingSphere* sphere;
        
    Framework::GridComponent* grid;

    Framework::FPSCamera* fpsCamera;
    std::map<int, bool> states;

    std::unique_ptr<DirectX::Keyboard> keyboard;
    std::unique_ptr<DirectX::Mouse> mouse;
    Framework::Window* window;
    Framework::DX::StepTimer m_timer;

    std::vector<Framework::XYZGameComponent*> components;

    Framework::Graphics* gfx;

    DirectX::SimpleMath::Matrix m_world = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Matrix m_view = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Matrix m_proj = DirectX::SimpleMath::Matrix::Identity;
};
