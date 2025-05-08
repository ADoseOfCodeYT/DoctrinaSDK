#include "main.h"

#include "stdafx.h"

int main(int argc, char *argv[])
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Doctrina Editor",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1280, 720,SDL_WINDOW_RESIZABLE);

    if(!window)
    {
        return -1;
    }

    SDL_Surface* window_surface = SDL_GetWindowSurface(window);

    if(!window_surface)
    {
        return -1;
    }

    SDL_UpdateWindowSurface(window);

    SDL_Event event;

    bool quit = false;
    while(!quit)
    {   
        while(SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                quit = true;
                break;
            switch (event.window.event) 
            {
                case SDL_WINDOWEVENT_CLOSE:
                     quit = true;
                    break;
                case SDL_WINDOWEVENT_RESIZED:
                    break;
            }
            default:
                break;
            }
        }
    }

    //Destroy window
    SDL_DestroyWindow(window);

    //Quit SDL subsystems
    SDL_Quit();

    return 0;
}