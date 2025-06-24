#include "Renderer/Public/deRenderer.h"

#include "Math/Public/deMath.h"

namespace de::renderer
{
    RHI_D3D11 g_RHI;
    map::CameraComponent g_RenderCam;

    void Initialize(SDL_Window* window)
    {
        g_RHI.Initialize(window, 1000.0f, 0.3f);
    }

    void ResizeWindow()
    {
        g_RHI.ResizeWindow();
    }

    void Render()
    {
        g_RHI.BeginFrame();

		//3D Rendering

        g_RenderCam.Draw();

		g_RHI.DisableZBuffer();

		//2D Rendering

		g_RHI.EnableZBuffer();

		g_RHI.FinishFrame();

    }

    void Update(float dT)
    {
        
    }

    void Shutdown()
    {
        g_RHI.Shutdown();

    }
}