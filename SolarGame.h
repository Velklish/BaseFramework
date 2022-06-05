#pragma once
#include <Keyboard.h>
#include <Mouse.h>

#include "Forward.h"
#include "IGame.h"
#include "XYZGameComponent.h"
#include <vector>
#include "SimpleMath.h"
#include "d3d11.h"
#include "GeometricPrimitive.h"
#include "wrl.h"
#include "SphereComponent.h"
#include "CubeComponent.h"
#include "FPSCamera.h"
#include "GridComponent.h"
#include "NewCube.h"
#include "OrbitalCamera.h"

namespace BaseFramework
{
	class SolarGame : public IGame
	{
	public:
		// Inherited via IGame
		void Initialize(int width, int height, LPCWSTR name = L"SolarGame") override;
		void Update(DX::StepTimer const& timer) override;
		void Render() override;
		void Tick() override;
		Graphics* GetGfx() override { return gfx; }
		Window* GetWindow() override { return window; }
		void ClearResources() override;
	private:
		void HandleInput() override;
		SphereComponent* sphere;
		GridComponent* grid;
		std::vector<CubeComponent*> cubes;
		
		FPSCamera* fpsCamera;
		OrbitalCamera* orbitalCamera;

		std::unique_ptr<DirectX::Keyboard> keyboard;
		std::unique_ptr<DirectX::Mouse> mouse;
		Window* window;
		DX::StepTimer m_timer;

		std::vector<XYZGameComponent*> components;

		Graphics* gfx;
	};
}


