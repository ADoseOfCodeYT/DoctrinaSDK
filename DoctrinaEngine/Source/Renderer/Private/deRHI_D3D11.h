#ifndef _DERHI_D3D11_H_
#define _DERHI_D3D11_H_

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

namespace de
{
	class RHI_D3D11
	{
	public:

		RHI_D3D11();
		virtual ~RHI_D3D11() = default;

		void Initialize(int screenWidth, int screenHeight, HWND hwnd, bool fullscreen, float screenDepth, float screenNear);
		void Shutdown();

		void BeginFrame();
		void FinishFrame();

		ID3D11Device* GetDevice();
		ID3D11DeviceContext* GetDeviceContext();

		void GetSwapChainNumerator(int& numerator);
		void GetSwapChainDenominator(int& denominator);

		void EnableZBuffer();
		void DisableZBuffer();

		void EnableAlphaBlending();
		void DisableAlphaBlending();

		void GetProjectionMatrix(XMMATRIX& projectionMatrix);
		void GetWorldMatrix(XMMATRIX& worldMatrix);
		void GetOrthoMatrix(XMMATRIX& orthoMatrix);

	public:

		bool Initialised = false;


		char VideoCardDesc[128];
		int VideoCardMemory;
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

		XMMATRIX m_ProjectionMatrix;
		XMMATRIX m_WorldMatrix;
		XMMATRIX m_OrthoMatrix;

		D3D11_VIEWPORT m_Viewport;

		ID3D11BlendState* m_AlphaBlendingState;
		ID3D11BlendState* m_AlphaDisabledBlendingState;

		int m_Numerator;
		int m_Denominator;
	};
}

#endif // !_DERHI_D3D11_H_
