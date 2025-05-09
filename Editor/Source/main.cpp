#include "main.h"


SDL_Window* window;
SDL_Renderer* renderer;

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {


    if (!SDL_Init(SDL_INIT_VIDEO)) 
    {
        de::Console::Post("Coudln't initalize SDL",  de::Console::LogLevel::ExtremeError);
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Doctrina Editor", 640, 480,  SDL_WINDOW_RESIZABLE, &window, &renderer)) {

        de::Console::Post("Coudln't create window and renderer",  de::Console::LogLevel::ExtremeError);
        return SDL_APP_FAILURE;
    }

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
        
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
        de::Console::Post("Window resized to: " + std::to_string(newWidth) + "x" + std::to_string(newHeight), de::Console::LogLevel::Default);
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) 
{
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {

    SDL_Quit();
}