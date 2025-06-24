#include "Application/Public/deApplication.h"

#include <cassert>
#include <windows.h>

#include "SDL3/SDL_video.h"

#include "Core/Public/deVersion.h"
#include "Tools/Public/deConsole.h"
#include "Core/Public/deCVar.h"
#include "Math/Public/deMath.h"


using namespace de::cvar;

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
			console::Post("[de::Application] Already initialised", console::LogLevel::Warning);

			return;
		}

		console::Post("[de::Application] Initialised", console::LogLevel::Default);

		assert(m_Window != nullptr);

		renderer::Initialize(m_Window);
		
		Initialised = true;
	}

	void Application::Shutdown()
	{
		console::Post("[de::Application] Shutdown", console::LogLevel::Default);

		renderer::Shutdown();

		return;
	}

	void Application::Run()
	{
		assert(Initialised && "[de::Application] Not initialised");

		float dT = float(m_Timer.RecordElapsedSeconds());

		const float targetDeltaTime = 1.0f / 60; // 60 hz

		dT = math::Clamp(dT, 0.0f, 0.5f);

		FixedUpdate();
		Update(dT);

		renderer::Render();
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
		renderer::ResizeWindow();
	}
}