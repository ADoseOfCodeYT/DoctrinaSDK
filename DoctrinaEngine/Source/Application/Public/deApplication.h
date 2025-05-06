#ifndef _DEAPPLICATION_H_
#define _DEAPPLICATION_H_

#include <windows.h>


#include "Renderer/Public/deRenderer.h"

namespace de
{
	class Application
	{
	public:

		Application();
		virtual ~Application() = default;

		void Shutdown();
		void Run();

		virtual void Initialize();
		virtual void Update(float dt);
		virtual void FixedUpdate();

		void SetWindow(HWND window);

	public:

		HWND Window;

		bool Initialised = false;
		bool VSync = true; // TODO: MOVE THIS TO A DIFFERENT CLASS AND MAKE IT CHANGEABLE WHILE APP IS RUNNING
		bool Fullscreen = false; // TODO: MOVE THIS TO A DIFFERENT CLASS AND MAKE IT CHANGEABLE WHILE APP IS RUNNING

	protected:

		float DeltaTime = 0.0f;
	};
}


#endif // !_DEAPPLICATION_H_
 