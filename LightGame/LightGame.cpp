#include "LightGame.h"

#include "Framework/Graphics.h"
#include "Framework/Window.h"
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_win32.h"
#include "ImGUI/imgui_impl_dx11.h"

using namespace DirectX::SimpleMath;

namespace LightGame
{
    void LightGame::Initialize(int width, int height, LPCWSTR name)
    {
        this->window = new BaseFramework::Window(width, height, name, this);
        this->gfx = new BaseFramework::Graphics(width, height, window->GetHwnd());
        this->grid = new BaseFramework::GridComponent();
        this->donutModel = new BaseFramework::Model(std::string("Models/character.fbx"), 0.02f);
        this->ambientLight = new BaseFramework::AmbientLight(Vector3(1,1,0),0.2);
	
        keyboard = std::make_unique<DirectX::Keyboard>();
        mouse = std::make_unique<DirectX::Mouse>();

        this->fpsCamera = new BaseFramework::FPSCamera(this, keyboard, mouse);
        this->orbitalCamera = new BaseFramework::OrbitalCamera(this, keyboard, mouse);
        this->grid->Initialize(this, 12.0f);
        this->donutModel->Initialize(this);
        this->ambientLight->Initialize(this);

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

    void LightGame::Update(BaseFramework::DX::StepTimer const& timer)
    {
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
        donutModel->Update(m_world,m_view,m_proj);
    }

    void LightGame::Render()
    {
        gfx->ClearBuffer();
        
        grid->Draw();
        donutModel->Draw();

        for (auto comp : components) {
            comp->Draw();
        }

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        //Create ImGui Test Window
        ImGui::Begin("Light Controls");
        ImGui::DragFloat3("Ambient Light Color", &ambientLight->data.color.x, 0.01f, 0.0f, 1.0f);
        ImGui::DragFloat("Ambient Light Strength", &ambientLight->data.lightStrength, 0.01f, 0.0f, 1.0f);
        ImGui::End();
        //Assemble Together Draw Data
        ImGui::Render();
        //Render Draw Data
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        
        ambientLight->Draw();

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

    BaseFramework::Graphics* LightGame::GetGfx()
    {
        return gfx;
    }

    BaseFramework::Window* LightGame::GetWindow()
    {
        return  window;
    }

    void LightGame::ClearResources()
    {
    }

    void LightGame::HandleInput()
    {
    }
}