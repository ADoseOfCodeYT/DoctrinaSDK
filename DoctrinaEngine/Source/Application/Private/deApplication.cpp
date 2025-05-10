#include "Application/Public/deApplication.h"

#include <cassert>
#include <windows.h>

#include "SDL3/SDL_video.h"

#include "Core/Public/deVersion.h"
#include "Tools/Public/deConsole.h"
#include "Core/Public/deCVar.h"

using namespace de::CVar;

namespace de
{

	Application::Application()
	{
		m_Window = nullptr;
	}

	void Application::Initialize()
	{

		if (Initialised)
		{
			Console::Post("[de::Application] Already initialised", Console::LogLevel::Warning);

			return;
		}

		Console::Post("[de::Application] Initialised", Console::LogLevel::Default);

		int windowWidth, windowHeight;

		assert(m_Window!= nullptr && "[de::Application] Window not set");
		SDL_GetWindowSize(m_Window, &windowWidth, &windowHeight);

		// fina-fucking-ly i managed to get the HWND. I must be stupid as fuck for not being able to use SDL_GetWindowProperties(), i just couldnt figure that out but atleast this works!!!!
		HWND hwnd = reinterpret_cast<HWND>(SDL_GetWindowFromID(SDL_GetWindowID(m_Window)));
		if (hwnd) {
			Console::Post("[de::Application] Retrieved HWND", Console::LogLevel::Default);
		} else {
			Console::Post("[de::Application] Failed to retrieve HWND", Console::LogLevel::ExtremeError);
			assert(false && "HWND retrieval failed");
		}

		m_RHI.Initialize(windowWidth, windowHeight, hwnd, false, 1000.0f, 0.3f);
		
		Initialised = true;
	}

	void Application::Shutdown()
	{
		Console::Post("[de::Application] Shutdown", Console::LogLevel::Default);

		return;
	}

	void Application::Run(double dt)
	{
		assert(Initialised && "[de::Application] Not initialised");

		FixedUpdate();
		Update(dt);
	}

    void Application::FixedUpdate()
    {

    }

	void Application::Update(double dt)
	{
		m_RHI.BeginFrame();

		m_RHI.FinishFrame();

		SDL_UpdateWindowSurface(m_Window);
	}

    void Application::SetWindow(SDL_Window *window)
    {
		m_Window = window;
	}
}