#include "main.h"

#include "Editor.h"

SDL_Window* window;

Editor DoctrinaEditor;

Uint64 NOW = SDL_GetPerformanceCounter();
Uint64 LAST = 0;
double DeltaTime = 0;

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
    if (!SDL_Init(SDL_INIT_VIDEO)) 
    {
        return SDL_APP_FAILURE;
    }

    window = SDL_CreateWindow("Doctrina Editor", 640, 480, SDL_WINDOW_RESIZABLE);

    DoctrinaEditor.SetWindow(window);

    DoctrinaEditor.Initialize();

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {

    if (event->type == SDL_EVENT_QUIT) 
    {
        return SDL_APP_SUCCESS; 
    }

    if (event->type == SDL_EVENT_WINDOW_RESIZED) 
    {
        int newWidth = event->window.data1;
        int newHeight = event->window.data2;

        DoctrinaEditor.SetWindow(window);
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) 
{
    LAST = NOW;
    NOW = SDL_GetPerformanceCounter();

    DeltaTime = (double)((NOW - LAST)*1000 / (double)SDL_GetPerformanceFrequency() );

    DoctrinaEditor.Run(DeltaTime);

    SDL_UpdateWindowSurface(window);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {

    SDL_Quit();
}