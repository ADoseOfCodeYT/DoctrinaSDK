#include "Application/Public/deApplication.h"

#include <cassert>
#include <windows.h>

#include "SDL3/SDL_video.h"

#include "Core/Public/deVersion.h"
#include "Tools/Public/deConsole.h"
#include "Core/Public/deCVar.h"
#include "Math/Public/deMath.h"


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

		Renderer::Initialize(m_Window);
		
		Initialised = true;
	}

	void Application::Shutdown()
	{
		Console::Post("[de::Application] Shutdown", Console::LogLevel::Default);

		Renderer::Shutdown();

		return;
	}

	void Application::Run()
	{
		assert(Initialised && "[de::Application] Not initialised");

		float dT = float(m_Timer.RecordElapsedSeconds());

		const float targetDeltaTime = 1.0f / 60; // 60 hz

		dT = Math::Clamp(dT, 0.0f, 0.5f);

		FixedUpdate();
		Update(dT);

		Renderer::Render();
	}

    void Application::FixedUpdate()
    {

    }

	void Application::Update(double dt)
	{
		
	}

    void Application::SetWindow(SDL_Window *window)
    {
		m_Window = window;
		Renderer::ResizeWindow();
	}
}