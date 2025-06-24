#ifndef _DERENDERER_H_
#define _DERENDERER_H_

#include "SDL3/SDL.h"

#include "Renderer/Private/deRHI_D3D11.h"

#include "Map/Public/deMap.h"

namespace de::renderer
{
    void Initialize(SDL_Window* window);
    void Shutdown();

    void ResizeWindow();

    void Render();
    void Update(float dT);

    extern RHI_D3D11 g_RHI;
    extern map::CameraComponent g_RenderCam;
}



#endif // !_DERENDERER_H_