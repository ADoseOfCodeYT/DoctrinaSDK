#ifndef _DERENDERER_H_
#define _DERENDERER_H_

#include "Renderer/Private/deRHI_D3D11.h"

namespace de
{
	class Renderer
	{
	public:

		Renderer();
		virtual ~Renderer() = default;

		void Initialize(int screenWidth, int screenHeight, HWND hwnd, bool fullScreen, float screenDepth, float screenNear);
		void Shutdown();

		bool Render();

	public:

		bool Initialised = false;

	private:

		RHI_D3D11 m_RHI;

	};
}

#endif // !_DERENDERER_H_
