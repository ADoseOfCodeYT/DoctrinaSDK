#include "Renderer/Public/deRenderer.h"

namespace de::Renderer
{
    RHI_D3D11 RHI;

    void Initialize(SDL_Window* window)
    {
        RHI.Initialize(window, 1000.0f, 0.3f);
    }

    void ResizeWindow()
    {
        RHI.ResizeWindow();
    }

    void Render()
    {
        RHI.BeginFrame();

		//3D Rendering

		RHI.DisableZBuffer();

		//2D Rendering

		RHI.EnableZBuffer();

		RHI.FinishFrame();

    }

    void Shutdown()
    {
        RHI.Shutdown();

    }
}