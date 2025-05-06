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
		Window = nullptr;
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

	void Application::Run()
	{
		assert(("Application hasnt been initialised", Initialised));

		FixedUpdate();
	}

	void Application::Update(float dt)
	{

	}

	void Application::FixedUpdate()
	{

	}

	void Application::SetWindow(HWND window)
	{
		this->Window = window;
	}
	
}