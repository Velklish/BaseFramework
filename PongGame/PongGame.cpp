#include "PongGame.h"
#include "Framework/Window.h"
#include "Framework/Graphics.h"

using namespace BaseFramework;

PongGame::PongGame::PongGame()
{
	this->playerBat = new BatComponent(0.7f, 0.08f, 0.0f, -0.95f);
	this->enemyBat = new BatComponent(0.7f, 0.08f, 0.0f, 0.95f);
	this->ball = new BallComponent(0.06f,0.06f,0.0f,0.0f);
}

void PongGame::PongGame::Initialize(int width, int height, LPCWSTR name)
{
	this->window = new Window(width, height, L"Pong", this);
	this->gfx = new Graphics(width, height, window->GetHwnd());

	keyboard = std::make_unique<DirectX::Keyboard>();
	mouse = std::make_unique<DirectX::Mouse>();
	
	components.push_back(playerBat);
	components.push_back(enemyBat);
	components.push_back(ball);

	for (auto comp : components) {
		comp->Initialize(this);
	}

	GetWindow()->GetSize(width, height);

	collision.x = -width/2;
	collision.y = -height/2;
	collision.width = width;
	collision.height = height;
	
	this->window->InitializeMessageLoop();
	this->m_timer.SetFixedTimeStep(true);
	this->m_timer.SetTargetElapsedSeconds(1.0 / 144);
}

void PongGame::PongGame::Update(BaseFramework::DX::StepTimer const& timer)
{
	HandleInput();

	if(playerBat->collision.Intersects(ball->collision)
		|| enemyBat->collision.Intersects(ball->collision))
	{
		ball->Bounce(BallComponent::Vertical);
	}
	
	for (auto comp : components) {
		comp->Update();
	}
}

void PongGame::PongGame::Render()
{
	gfx->ClearBuffer();

	for (auto comp : components) {
		comp->Draw();
	}
	
	gfx->Present();
}

void PongGame::PongGame::Tick()
{
	m_timer.Tick([&]()
		{
			Update(m_timer);
		});

	Render();
}

void PongGame::PongGame::ClearResources()
{
	for (auto comp : components) {
		comp->ClearResources();
	}
}

PongGame::PongGame::~PongGame()
{
	PongGame::PongGame::ClearResources();
}

void PongGame::PongGame::HandleInput()
{
	auto time = static_cast<float>(m_timer.GetTotalSeconds());
	auto kb = keyboard->GetState();

	if(kb.D && collision.Contains(playerBat->collision))
	{
		playerBat->Translate(1.0f,0.0f);
	}
	if(kb.A && collision.Contains(playerBat->collision))
	{
		playerBat->Translate(-1.0f,0.0f);
	}

	if(kb.Right && collision.Contains(enemyBat->collision))
	{
		enemyBat->Translate(1.0f,0.0f);
	}
	if(kb.Left && collision.Contains(enemyBat->collision))
	{
		enemyBat->Translate(-1.0f,0.0f);
	}
	if(kb.Space)
	{
		ball->Launch();
	}
}

