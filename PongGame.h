#pragma once
#include <Keyboard.h>
#include <Mouse.h>

#include "Forward.h"
#include "BatComponent.h"
#include "IGame.h"

#include <vector>

#include "BallComponent.h"

namespace BaseFramework 
{
	class PongGame : public IGame
	{
	public:
		PongGame();
		// Inherited via Game
		void Initialize(int width, int height, LPCWSTR name = L"Pong") override;
		void Update(DX::StepTimer const& timer) override;
		void Render() override;
		void Tick() override;
		Graphics* GetGfx() override { return gfx; }
		Window* GetWindow() override { return window; }
		void ClearResources() override;

		~PongGame();
		
	private:
		void HandleInput() override;

		std::unique_ptr<DirectX::Keyboard> keyboard;
		std::unique_ptr<DirectX::Mouse> mouse;
		Window* window;
		DX::StepTimer m_timer;
		
		std::vector<GameComponent*> components;
		DirectX::SimpleMath::Rectangle collision {-1,1,1,1};

		BatComponent* playerBat;
		BatComponent* enemyBat;
		BallComponent* ball; 
		
		Graphics* gfx;
	};
}

