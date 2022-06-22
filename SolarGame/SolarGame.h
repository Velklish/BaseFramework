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
	class SolarGame : public Framework::IGame
	{
	public:
		// Inherited via IGame
		void Initialize(int width, int height, LPCWSTR name = L"SolarGame") override;
		void Update(Framework::DX::StepTimer const& timer) override;
		void Render() override;
		void Tick() override;
		Framework::Graphics* GetGfx() override { return gfx; }
		Framework::Window* GetWindow() override { return window; }
		void ClearResources() override;
	private:
		void HandleInput() override;
		SphereComponent* sphere;
		Framework::GridComponent* grid;
		std::vector<CubeComponent*> cubes;

		Framework::FPSCamera* fpsCamera;
		Framework::OrbitalCamera* orbitalCamera;

		std::unique_ptr<DirectX::Keyboard> keyboard;
		std::unique_ptr<DirectX::Mouse> mouse;
		Framework::Window* window;
		Framework::DX::StepTimer m_timer;

		std::vector<Framework::XYZGameComponent*> components;

		Framework::Graphics* gfx;
	};
}


