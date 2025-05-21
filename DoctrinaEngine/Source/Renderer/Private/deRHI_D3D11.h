#ifndef _DERHI_D3D11_H_
#define _DERHI_D3D11_H_

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>


#include "SDL3/SDL.h"

#include "Math/Public/deMath.h"

namespace de
{
	class RHI_D3D11
	{
	public:

		RHI_D3D11();
		virtual ~RHI_D3D11() = default;

		void Initialize(SDL_Window* window, float screenDepth, float screenNear);
		void Shutdown();

		void BeginFrame();
		void FinishFrame();

		void ResizeWindow();

		ID3D11Device* GetDevice();
		ID3D11DeviceContext* GetDeviceContext();

		void GetSwapChainNumerator(int& numerator);
		void GetSwapChainDenominator(int& denominator);

		void EnableZBuffer();
		void DisableZBuffer();

		void EnableAlphaBlending();
		void DisableAlphaBlending();

	public:

		bool Initialised = false;

		char VideoCardDesc[128];
		int VideoCardMemory;

		Math::Matrix ProjectionMatrix;
		Math::Matrix WorldMatrix;
		Math::Matrix OrthoMatrix;

	private:

		IDXGISwapChain* m_SwapChain;

		ID3D11Device* m_Device;
		ID3D11DeviceContext* m_DeviceContext;
		ID3D11RenderTargetView* m_RenderTargetView;
		ID3D11Texture2D* m_DepthStencilBuffer;

		ID3D11DepthStencilState* m_DepthStencilState;
		ID3D11DepthStencilState* m_DepthDisabledStencilState;
		ID3D11DepthStencilView* m_DepthStencilView;

		ID3D11RasterizerState* m_RasterState;	

		D3D11_VIEWPORT m_Viewport;

		SDL_Window* m_Window;

		ID3D11BlendState* m_AlphaBlendingState;
		ID3D11BlendState* m_AlphaDisabledBlendingState;

		int m_Numerator;
		int m_Denominator;

		int m_WindowWidth;
		int m_WindowHeight;
	};

	

}



#endif // !_DERHI_D3D11_H_
