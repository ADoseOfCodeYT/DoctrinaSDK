#ifndef _DEAPPLICATION_H_
#define _DEAPPLICATION_H_

#include "SDL3/SDL.h"

#include "Renderer/Private/deRHI_D3D11.h"

namespace de
{
	class Application
	{
	public:

		Application();
		virtual ~Application() = default;

		virtual void Initialize();
		void Shutdown();
		void Run(double dt);

		virtual void FixedUpdate();
		virtual void Update(double dt);

		void SetWindow(SDL_Window* window);

	public:

		bool Initialised = false;
		bool VSync = true; // TODO: MOVE THIS TO A DIFFERENT CLASS AND MAKE IT CHANGEABLE WHILE APP IS RUNNING
		bool Fullscreen = false; // TODO: MOVE THIS TO A DIFFERENT CLASS AND MAKE IT CHANGEABLE WHILE APP IS RUNNING

	private:

		SDL_Window* m_Window;

		RHI_D3D11 m_RHI;
	};
}

#endif // !_DEAPPLICATION_H_
