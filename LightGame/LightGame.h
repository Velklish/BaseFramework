#pragma once
#include <Keyboard.h>
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

namespace LightGame
{
    class LightGame : BaseFramework::IGame
    {
    public:
        void Initialize(int width, int height, LPCWSTR name = L"LightGame") override;
        void Update(BaseFramework::DX::StepTimer const& timer) override;
        void Render() override;
        void Tick() override;
        BaseFramework::Graphics* GetGfx() override;
        BaseFramework::Window* GetWindow() override;
        void ClearResources() override;
    private:
        void HandleInput() override;

        BaseFramework::GridComponent* grid;

        BaseFramework::FPSCamera* fpsCamera;
        BaseFramework::OrbitalCamera* orbitalCamera;
        BaseFramework::Model* donutModel;
        BaseFramework::AmbientLight* ambientLight;

        std::unique_ptr<DirectX::Keyboard> keyboard;
        std::unique_ptr<DirectX::Mouse> mouse;
        BaseFramework::Window* window;
        BaseFramework::DX::StepTimer m_timer;

        std::vector<BaseFramework::XYZGameComponent*> components;

        BaseFramework::Graphics* gfx;
    };
}
