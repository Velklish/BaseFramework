#pragma once
#include <Keyboard.h>
#include <Mouse.h>

#include "Framework/Forward.h"
#include "Framework/IGame.h"
#include "Framework/XYZGameComponent.h"
#include <vector>
#include "SimpleMath.h"
#include "d3d11.h"
#include "GeometricPrimitive.h"
#include "wrl.h"
#include "SphereComponent.h"
#include "CubeComponent.h"
#include "Framework/FPSCamera.h"
#include "Framework/GridComponent.h"
#include "Framework/OrbitalCamera.h"

namespace SolarGame
{
	class SolarGame : public BaseFramework::IGame
	{
	public:
		// Inherited via IGame
		void Initialize(int width, int height, LPCWSTR name = L"SolarGame") override;
		void Update(BaseFramework::DX::StepTimer const& timer) override;
		void Render() override;
		void Tick() override;
		BaseFramework::Graphics* GetGfx() override { return gfx; }
		BaseFramework::Window* GetWindow() override { return window; }
		void ClearResources() override;
	private:
		void HandleInput() override;
		SphereComponent* sphere;
		BaseFramework::GridComponent* grid;
		std::vector<CubeComponent*> cubes;

		BaseFramework::FPSCamera* fpsCamera;
		BaseFramework::OrbitalCamera* orbitalCamera;

		std::unique_ptr<DirectX::Keyboard> keyboard;
		std::unique_ptr<DirectX::Mouse> mouse;
		BaseFramework::Window* window;
		BaseFramework::DX::StepTimer m_timer;

		std::vector<BaseFramework::XYZGameComponent*> components;

		BaseFramework::Graphics* gfx;
	};
}


