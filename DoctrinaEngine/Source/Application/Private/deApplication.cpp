#include "Application/Public/deApplication.h"

#include <cassert>

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

		Initialised = true;
	}

	void Application::Shutdown()
	{
		Console::Post("[de::Application] Shutdown", Console::LogLevel::Default);

		return;
	}

	void Application::Run(double dt)
	{
		assert(Initialised && "[de::Application] not initialised");

		FixedUpdate();
		Update(dt);
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
	}
}