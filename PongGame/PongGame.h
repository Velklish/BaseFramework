#pragma once
#include <Keyboard.h>
#include <Mouse.h>

#include "Framework/Forward.h"
#include "BatComponent.h"
#include "Framework/IGame.h"

#include <vector>

#include "BallComponent.h"

namespace PongGame 
{
	class PongGame : public BaseFramework::IGame
	{
	public:
		PongGame();
		// Inherited via Game
		void Initialize(int width, int height, LPCWSTR name = L"Pong") override;
		void Update(BaseFramework::DX::StepTimer const& timer) override;
		void Render() override;
		void Tick() override;
		BaseFramework::Graphics* GetGfx() override { return gfx; }
		BaseFramework::Window* GetWindow() override { return window; }
		void ClearResources() override;

		~PongGame();
		
	private:
		void HandleInput() override;

		std::unique_ptr<DirectX::Keyboard> keyboard;
		std::unique_ptr<DirectX::Mouse> mouse;
		BaseFramework::Window* window;
		BaseFramework::DX::StepTimer m_timer;
		
		std::vector<BaseFramework::GameComponent*> components;
		DirectX::SimpleMath::Rectangle collision {-1,1,1,1};

		BatComponent* playerBat;
		BatComponent* enemyBat;
		BallComponent* ball;

		BaseFramework::Graphics* gfx;
	};
}

