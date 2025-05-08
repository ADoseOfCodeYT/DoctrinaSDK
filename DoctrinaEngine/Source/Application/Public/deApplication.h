#ifndef _DEAPPLICATION_H_
#define _DEAPPLICATION_H_

#include "SDL2/SDL.h"

namespace de
{
	class Application
	{
	public:

		Application();
		virtual ~Application() = default;

		virtual void Initialize();
		void Shutdown();
		void Run();

		virtual void FixedUpdate();

		void SetWindow(SDL_Window* window);

	public:

		bool Initialised = false;
		bool VSync = true; // TODO: MOVE THIS TO A DIFFERENT CLASS AND MAKE IT CHANGEABLE WHILE APP IS RUNNING
		bool Fullscreen = false; // TODO: MOVE THIS TO A DIFFERENT CLASS AND MAKE IT CHANGEABLE WHILE APP IS RUNNING

	private:

		SDL_Window* m_Window;
	};
}

#endif // !_DEAPPLICATION_H_
