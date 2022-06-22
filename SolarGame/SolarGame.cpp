#include "SolarGame.h"
#include "Framework/Window.h"
#include "Framework/Graphics.h"

using namespace DirectX::SimpleMath;

void SolarGame::SolarGame::Initialize(int width, int height, LPCWSTR name)
{
	this->window = new BaseFramework::Window(width, height, name, this);
	this->gfx = new BaseFramework::Graphics(width, height, window->GetHwnd());
	this->sphere = new SphereComponent();
	this->grid = new BaseFramework::GridComponent();
	
	for(int i = 0; i < 7; i++)
	{
		auto cube = new CubeComponent(0.2);
		cubes.push_back(cube);
		components.push_back(cube);
	}
	
	keyboard = std::make_unique<DirectX::Keyboard>();
	mouse = std::make_unique<DirectX::Mouse>();

	this->fpsCamera = new BaseFramework::FPSCamera(this, keyboard, mouse);
	this->orbitalCamera = new BaseFramework::OrbitalCamera(this, keyboard, mouse);
	this->grid->Initialize(this);
	this->sphere->Initialize(this);

	for (auto comp : components) {
		comp->Initialize(this);
	}

	this->window->InitializeMessageLoop();
}

void SolarGame::SolarGame::Update(BaseFramework::DX::StepTimer const& timer)
{
	auto time = static_cast<float>(timer.GetTotalSeconds());

	HandleInput();
	
	Matrix m_world = Matrix::Identity;
	Matrix m_view;
	Matrix m_proj;

	if(keyboard->GetState().LeftAlt)
	{
		orbitalCamera->Update(m_view,m_proj);
	}
	else
	{
		fpsCamera->Update(m_view,m_proj);
	}
	
	grid->Update(m_view, m_proj);
	
	auto baseComp = components.at(0);
	components.at(0)->Rotate(BaseFramework::XYZGameComponent::RotDirection::Y, time * 40.f);
	components.at(0)->Update(m_world, m_view, m_proj);

	components.at(1)->Translate(0, 0, 3);
	components.at(1)->Rotate(BaseFramework::XYZGameComponent::RotDirection::Y, time * 40.f);
	components.at(1)->Update(components.at(0)->GetWorld(), m_view, m_proj);

	components.at(2)->Translate(-3, 0, 0);
	components.at(2)->Rotate(BaseFramework::XYZGameComponent::RotDirection::Y, time * 40.f);
	components.at(2)->Update(components.at(0)->GetWorld(), m_view, m_proj);

	components.at(3)->Translate(0, 0, -3);
	components.at(3)->Rotate(BaseFramework::XYZGameComponent::RotDirection::Y, time * 40.f);
	components.at(3)->Update(components.at(0)->GetWorld(), m_view, m_proj);
	
	components.at(4)->Translate(3, 0, 0);
	components.at(4)->Rotate(BaseFramework::XYZGameComponent::RotDirection::Y, time * 40.f);
	components.at(4)->Update(components.at(0)->GetWorld(), m_view, m_proj);

	components.at(5)->Rotate(BaseFramework::XYZGameComponent::RotDirection::Y, 12 * time * 40.f);
	components.at(5)->Translate(-1, 0, 0);
	components.at(5)->Rotate(BaseFramework::XYZGameComponent::RotDirection::Y, 12 * time * 40.f);
	components.at(5)->Update(components.at(3)->GetWorld(), m_view, m_proj);
	components.at(6)->Translate(1, 0, 0);
	components.at(6)->Rotate(BaseFramework::XYZGameComponent::RotDirection::Y, time * 40.f);
	components.at(6)->Update(components.at(3)->GetWorld(), m_view, m_proj);

	/*components.at(0)->Rotate(XYZGameComponent::RotDirection::Y, time * 40.f);
	components.at(0)->Update(m_world, m_view, m_proj);
	
	for (int i = 1; i < 7; i++)
	{
		components.at(i)->Rotate(XYZGameComponent::RotDirection::X, time * 20.f);
		components.at(i)->Translate(0,2.0f/i+0.5f,0);
		components.at(i)->Update(m_world,m_view,m_proj);
		m_world = components.at(i)->GetWorld();
	}*/
	
	/*for (auto comp : components) {
		comp->Rotate(XYZGameComponent::RotDirection::X, time * 40.f);
		comp->Translate(0,2,0);
		comp->Update(m_world,m_view,m_proj);
		m_world = comp->GetWorld();
	}*/
}

void SolarGame::SolarGame::Render()
{
	gfx->ClearBuffer();

	grid->Draw();

	for (auto comp : components) {
		comp->Draw();
	}

	gfx->Present();
}

void SolarGame::SolarGame::Tick()
{
	m_timer.Tick([&]()
		{
			Update(m_timer);
		});

	Render();
}

void SolarGame::SolarGame::ClearResources()
{
	for (auto comp : components) {
		comp->ClearResources();
	}
}

void SolarGame::SolarGame::HandleInput()
{
}
