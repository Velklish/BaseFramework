#include "Katamari.h"
#include "Framework/Graphics.h"
#include "Framework/PointLight.h"
#include "Framework/Window.h"
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_win32.h"
#include "ImGUI/imgui_impl_dx11.h"

void Katamari::Initialize(int width, int height, LPCWSTR name)
{
    this->window = new Framework::Window(width, height, name, this);
    this->gfx = new Framework::Graphics(width, height, window->GetHwnd());
    this->grid = new Framework::GridComponent();
    this->player = new Framework::Model(std::string("Models/Character.fbx"), 0.02f);
    this->sphere = new DirectX::BoundingSphere(DirectX::SimpleMath::Vector3(0,0,0), 5.0f);
    
    components.push_back(new Framework::Model(std::string("Models/block.fbx"), 0.05f));
    
    keyboard = std::make_unique<DirectX::Keyboard>();
    mouse = std::make_unique<DirectX::Mouse>();
    this->fpsCamera = new Framework::FPSCamera(this, keyboard, mouse);
    this->player->Initialize(this);
    this->grid->Initialize(this, 12.0f);

    int index = 0;
    for (auto element : components)
    {
        states.insert({index, false});
        element->Translate(0,0,14);
        element->Initialize(this);
        index++;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplWin32_Init(window->GetHwnd());
    ImGui_ImplDX11_Init(gfx->GetDevice().Get(), gfx->GetContext().Get());
    ImGui::StyleColorsDark();

    this->window->InitializeMessageLoop();
}

void Katamari::Update(Framework::DX::StepTimer const& timer)
{

    gfx->ClearBuffer();
    fpsCamera->Update(m_view,m_proj);
    //components[0]->Translate(2,2,2);
    //components[0]->Update(GetWorldTransform());
    
    auto test1 = components[0]->GetWorld().Transpose().Translation();
    auto test2 = components[0]->GetWorld().Translation();

    HandleInput();
    if(sphere->Contains(test2))
    {
        
        auto test = "lol";
    }
    components[0]->Update(GetWorldTransform());

    int index = 0;
    for (auto element : components)
    {
        auto test = element->GetWorld().Translation();
        auto res = states[index];
        if(sphere->Contains(element->GetWorld().Translation()) || states[index] == true)
        {
            states[index] = true;
            auto transform = GetWorldTransform();
            element->Update(Framework::Transform(transform.view,transform.proj,player->GetWorld(),transform.cameraPosition));
        }
        else
        {
            element->Update(GetWorldTransform());
        }
        index++;
    }
    
    grid->Update(m_view, m_proj);
    player->Update(GetWorldTransform());
    
    auto time = static_cast<float>(timer.GetTotalSeconds());
    //model->Rotate(Framework::XYZGameComponent::RotDirection::Y, time * 40.f);
}

void Katamari::Render()
{
    gfx->ClearBuffer();
    grid->Draw();
    player->Draw();

    for (auto element : components)
    {
        element->Draw();
    }
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    //Create ImGui Test Window

    //Assemble Together Draw Data
    ImGui::Render();
    //Render Draw Data
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        
    gfx->Present();
}

void Katamari::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();
}

Framework::Graphics* Katamari::GetGfx()
{
    return gfx;
}

Framework::Window* Katamari::GetWindow()
{
    return  window;
}

void Katamari::ClearResources()
{
}

Framework::Transform Katamari::GetWorldTransform()
{
    return Framework::Transform(m_view,m_proj,m_world, fpsCamera->GetCameraPos());
}

void Katamari::HandleInput()
{
    auto kb = keyboard->GetState();
    if(kb.D)
    {
        DirectX::SimpleMath::Vector3 vector(-0.1f,0.0f,0.0f);
        player->Translate(vector.x,vector.y,vector.z);
        sphere->Center = sphere->Center - vector;
    }
    if(kb.A)
    {
        DirectX::SimpleMath::Vector3 vector(0.1f,0.0f,0.0f);
        player->Translate(vector.x,vector.y,vector.z);
        sphere->Center = sphere->Center - vector;
    }

    if(kb.W)
    {
        DirectX::SimpleMath::Vector3 vector(0.0f,0.0f,0.1f);
        player->Translate(vector.x,vector.y,vector.z);
        sphere->Center = sphere->Center - vector;
    }
    if(kb.S)
    {
        DirectX::SimpleMath::Vector3 vector(0.0f,0.0f,-0.1f);
        player->Translate(vector.x,vector.y,vector.z);
        sphere->Center = sphere->Center - vector;
    }
}
