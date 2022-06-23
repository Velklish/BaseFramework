#include "LightGame.h"

#include "Framework/Graphics.h"
#include "Framework/PointLight.h"
#include "Framework/Window.h"
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_win32.h"
#include "ImGUI/imgui_impl_dx11.h"

using namespace DirectX::SimpleMath;

namespace LightGame
{
    void LightGame::Initialize(int width, int height, LPCWSTR name)
    {
        this->window = new Framework::Window(width, height, name, this);
        this->gfx = new Framework::Graphics(width, height, window->GetHwnd());
        this->grid = new Framework::GridComponent();
        this->model2 = new Framework::Model(std::string("Models/block.fbx"), 0.10f, std::string("Models//donut.png"));
        this->model = new Framework::Model(std::string("Models/Character.fbx"), 0.02f);
        
        this->ambientLight = new Framework::AmbientLight(DirectX::XMFLOAT3(1,1,1),1);
        this->dynamicLight = new Framework::PointLight(DirectX::XMFLOAT3(1,1,1),1,Vector3(0,0,0));
	
        keyboard = std::make_unique<DirectX::Keyboard>();
        mouse = std::make_unique<DirectX::Mouse>();

        this->fpsCamera = new Framework::FPSCamera(this, keyboard, mouse);
        this->orbitalCamera = new Framework::OrbitalCamera(this, keyboard, mouse);
        this->grid->Initialize(this, 12.0f);
        this->model->Initialize(this);
        this->model2->Initialize(this);
        this->ambientLight->Initialize(this);
        this->dynamicLight->Initialize(this);

        for (auto comp : components) {
            comp->Initialize(this);
        }

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        ImGui_ImplWin32_Init(window->GetHwnd());
        ImGui_ImplDX11_Init(gfx->GetDevice().Get(), gfx->GetContext().Get());
        ImGui::StyleColorsDark();

        this->window->InitializeMessageLoop();
    }

    void LightGame::Update(Framework::DX::StepTimer const& timer)
    {
        if(keyboard->GetState().LeftAlt)
        {
            orbitalCamera->Update(m_view,m_proj);
        }
        else
        {
            fpsCamera->Update(m_view,m_proj);
        }
	
        grid->Update(m_view, m_proj);
        model->Translate(0,0,0);
        model->Update(GetWorldTransform());
        model2->Update(GetWorldTransform());
        dynamicLight->Update();
        auto time = static_cast<float>(timer.GetTotalSeconds());
        //model->Rotate(Framework::XYZGameComponent::RotDirection::Y, time * 40.f);
    }

    void LightGame::Render()
    {
        gfx->ClearBuffer();
        
        grid->Draw();
        model->Draw();
        model2->Draw();

        for (auto comp : components) {
            comp->Draw();
        }

        ambientLight->Draw();
        dynamicLight->Draw();

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        //Create ImGui Test Window
        ImGui::Begin("Ambient Light");
        ImGui::DragFloat3("Ambient Light Color", &ambientLight->data.color.x, 0.01f, 0.0f, 1.0f);
        ImGui::DragFloat("Ambient Light Strength", &ambientLight->data.lightStrength, 0.01f, 0.0f, 1.0f);
        ImGui::End();

        ImGui::Begin("Dynamic Light");
        ImGui::DragFloat3("Dynamic Light Position", &dynamicLight->data.position.x, 0.01f, -10.0f, 10.0f);
        ImGui::DragFloat3("Dynamic Light Color", &dynamicLight->data.color.x, 0.01f, 0.0f, 1.0f);
        ImGui::DragFloat("Dynamic Light Strength", &dynamicLight->data.lightStrength, 0.01f, 0.0f, 1.0f);

        ImGui::End();
        //Assemble Together Draw Data
        ImGui::Render();
        //Render Draw Data
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        
        gfx->Present();
    }

    void LightGame::Tick()
    {
        m_timer.Tick([&]()
    {
        Update(m_timer);
    });

        Render();
    }

    Framework::Graphics* LightGame::GetGfx()
    {
        return gfx;
    }

    Framework::Window* LightGame::GetWindow()
    {
        return  window;
    }

    void LightGame::ClearResources()
    {
    }

    Framework::Transform LightGame::GetWorldTransform() 
    {
        return Framework::Transform(m_view,m_proj,m_world, fpsCamera->GetCameraPos());
    }

    void LightGame::HandleInput()
    {
    }
}