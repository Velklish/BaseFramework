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
	class PongGame : public Framework::IGame
	{
	public:
		PongGame();
		// Inherited via Game
		void Initialize(int width, int height, LPCWSTR name = L"Pong") override;
		void Update(Framework::DX::StepTimer const& timer) override;
		void Render() override;
		void Tick() override;
		Framework::Graphics* GetGfx() override { return gfx; }
		Framework::Window* GetWindow() override { return window; }
		void ClearResources() override;

		~PongGame();
		
	private:
		void HandleInput() override;

		std::unique_ptr<DirectX::Keyboard> keyboard;
		std::unique_ptr<DirectX::Mouse> mouse;
		Framework::Window* window;
		Framework::DX::StepTimer m_timer;
		
		std::vector<Framework::GameComponent*> components;
		DirectX::SimpleMath::Rectangle collision {-1,1,1,1};

		BatComponent* playerBat;
		BatComponent* enemyBat;
		BallComponent* ball;

		Framework::Graphics* gfx;
	};
}

