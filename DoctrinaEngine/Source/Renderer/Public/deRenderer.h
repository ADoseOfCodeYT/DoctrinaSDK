#ifndef _DERENDERER_H_
#define _DERENDERER_H_

#include "SDL3/SDL.h"

#include "Renderer/Private/deRHI_D3D11.h"

namespace de::Renderer
{
    void Initialize(SDL_Window* window);
    void Shutdown();

    void Render();

    extern RHI_D3D11 RHI;
}



#endif // !_DERENDERER_H_