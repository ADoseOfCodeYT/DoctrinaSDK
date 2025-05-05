#include "Renderer/Public/deRenderer.h"

#include "Tools/Public/deConsole.h"
#include "Core/Public/deCVar.h"

namespace de
{
	Renderer::Renderer()
	{
		
	}

	void Renderer::Initialize(int screenWidth, int screenHeight, HWND hwnd, bool fullScreen, float screenDepth, float screenNear)
	{
		m_RHI.Initialize(screenWidth, screenWidth, hwnd, fullScreen, screenDepth, screenNear);

		if (m_RHI.Initialised)
		{
			Console::Post("[de::Renderer] Initialised", Console::LogLevel::Default);

			Initialised = true;
		}
	}

	void Renderer::Shutdown()
	{
		m_RHI.Shutdown();
		Console::Post("[de::Renderer] Shutdown", Console::LogLevel::Default);
	}

	bool Renderer::Render()
	{
		m_RHI.BeginFrame();

		//3D Rendering

		m_RHI.DisableZBuffer();

		//2D Rendering

		m_RHI.EnableZBuffer();

		m_RHI.FinishFrame();

		return true;
	}
}