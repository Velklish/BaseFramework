#pragma once
#include "Forward.h"
#include "Windows.h"
#include "StepTimer.h"

namespace BaseFramework {
    class IGame
    {
    public:
        virtual void Initialize(int width, int height, LPCWSTR name) = 0;

        virtual void Update(DX::StepTimer const& timer) = 0;

        virtual void Render() = 0;

        virtual void Tick() = 0;

        virtual Graphics* GetGfx() = 0;

        virtual Window* GetWindow() = 0;

        virtual void ClearResources() = 0;
    private:
        virtual void HandleInput() = 0;
    };
}
