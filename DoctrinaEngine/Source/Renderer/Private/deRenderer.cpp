#include "Renderer/Public/deRenderer.h"

namespace de
{
	Renderer::Renderer()
	{
		
	}

	Renderer::~Renderer()
	{

	}

	void Renderer::Initialize(int screenWidth, int screenHeight, HWND hwnd, bool fullScreen, float screenDepth, float screenNear)
	{
		m_RHI.Initialize(screenWidth, screenWidth, hwnd, fullScreen, screenDepth, screenNear);

		if (m_RHI.Initialised)
		{
			Initialised = true;
		}
	}

	void Renderer::Shutdown()
	{
		m_RHI.Shutdown();
	}

	bool Renderer::Render()
	{
		m_RHI.BeginFrame();

		//Rendering code

		m_RHI.PresentFrame();

		return true;
	}
}