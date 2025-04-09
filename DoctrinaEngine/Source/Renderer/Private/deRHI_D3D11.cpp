#include "Renderer/Public/deRHI_D3D11.h"

#include <iostream>
#include <string>

namespace de
{
	RHI_D3D11::RHI_D3D11()
	{
		m_SwapChain = nullptr;

		m_Device = nullptr;
		m_DeviceContext = nullptr;
		m_RenderTargetView = nullptr;
		m_DepthStencilBuffer = nullptr;

		m_DepthStencilState = nullptr;
		m_DepthDisabledStencilState = nullptr;
		m_DepthStencilView = nullptr;

		m_RasterState = nullptr;

		m_ProjectionMatrix = {};
		m_WorldMatrix = {};
		m_OrthoMatrix = {};

		m_Viewport = {};

		m_AlphaEnableBlendingState = nullptr;
		m_AlphaDisableBlendingState = nullptr;

		m_Numerator = 0;
		m_Denominator = 0;

		VideoCardMemory = 0;
	}

	RHI_D3D11::~RHI_D3D11()
	{
	}

	void RHI_D3D11::Initialize(int screenWidth, int screenHeight, HWND hwnd, bool fullScreen, float screenDepth, float screenNear)
	{
		HRESULT result;
		IDXGIFactory* dxFactory;
		IDXGIAdapter* dxAdapter;
		IDXGIOutput* dxAdapterOutput;

		unsigned int numModes, i;
		unsigned long long stringLength;
		
		DXGI_MODE_DESC* displayModeList;
		DXGI_ADAPTER_DESC adapterDesc;
		DXGI_SWAP_CHAIN_DESC swapChainDesc;

		float fieldOfView, screenAspect;

		int error;

		D3D_FEATURE_LEVEL featureLevel;

		ID3D11Texture2D* backBufferPtr;

		D3D11_TEXTURE2D_DESC depthBufferDesc;
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		D3D11_RASTERIZER_DESC rasterDesc;
		D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
		D3D11_BLEND_DESC blendStateDescription;

		result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&dxFactory);
		if (FAILED(result))
		{
			std::cout << "[de::RHI_D3D11] FAILED : CreateDXGIFactory()" << std::endl;

			Initialised = false;
			return;
		}

		result = dxFactory->EnumAdapters(0, &dxAdapter);
		if (FAILED(result))
		{
			std::cout << "[de::RHI_D3D11] FAILED : EnumAdapters()" << std::endl;

			Initialised = false;
			return;
		}

		result = dxAdapter->EnumOutputs(0, &dxAdapterOutput);
		if (FAILED(result))
		{
			std::cout << "[de::RHI_D3D11] FAILED : EnumOutputs()" << std::endl;

			Initialised = false;
			return;
		}

		result = dxAdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
		if (FAILED(result))
		{
			std::cout << "[de::RHI_D3D11] FAILED : GetDisplayModeList()" << std::endl;

			Initialised = false;
			return;
		}

		displayModeList = new DXGI_MODE_DESC[numModes];
		if (!displayModeList)
		{
			std::cout << "[de::RHI_D3D11] FAILED : new DXGI_MODE_DESC" << std::endl;

			Initialised = false;
			return;
		}

		result = dxAdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
		if (FAILED(result))
		{
			std::cout << "[de::RHI_D3D11] FAILED : GetDisplayModeList()" << std::endl;

			Initialised = false;
			return;
		}

		for (i = 0; i < numModes; i++)
		{
			if (displayModeList[i].Width == (unsigned int)screenWidth)
			{
				if (displayModeList[i].Height == (unsigned int)screenHeight)
				{
					m_Numerator = displayModeList[i].RefreshRate.Numerator;
					m_Denominator = displayModeList[i].RefreshRate.Denominator;
				}
			}
		}

		result = dxAdapter->GetDesc(&adapterDesc);
		if (FAILED(result))
		{
			std::cout << "[de::RHI_D3D11] FAILED : GetDesc()" << std::endl;

			Initialised = false;
			return;
		}

		VideoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

		error = wcstombs_s(&stringLength, VideoCardDesc, 128, adapterDesc.Description, 128);
		if (error != 0)
		{
			std::cout << "[de::RHI_D3D11] FAILED : wcstombs_s()" << std::endl;

			Initialised = false;
			return;
		}

		delete [] displayModeList;
		displayModeList = nullptr;

		dxAdapterOutput->Release();
		dxAdapterOutput = nullptr;

		dxAdapter->Release();
		dxAdapter = nullptr;

		dxFactory->Release();
		dxFactory = nullptr;

		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

		swapChainDesc.BufferCount = 1; // only one backbuffer, i might add double or triple buffering in the future

		swapChainDesc.BufferDesc.Width = screenWidth;
		swapChainDesc.BufferDesc.Height = screenHeight;

		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 32 bit

		// initialise the swapchain with vsync on
		swapChainDesc.BufferDesc.RefreshRate.Numerator = m_Numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = m_Denominator;

		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

		swapChainDesc.OutputWindow = hwnd;

		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;

		if (fullScreen)
		{
			swapChainDesc.Windowed = false;
		}
		else
		{
			swapChainDesc.Windowed = true;
		}

		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		swapChainDesc.Flags = 0;

		featureLevel = D3D_FEATURE_LEVEL_11_0;

		result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, &featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &m_SwapChain, &m_Device, nullptr, &m_DeviceContext);
		if (FAILED(result))
		{
			std::cout << "[de::RHI_D3D11] FAILED : D3D11CreateDeviceAndSwapChain()" << std::endl;

			Initialised = false;
			return;
		}

		Initialised = true;

		std::cout << "[de::RHI_D3D11] Initialised" << std::endl;
	}

	void RHI_D3D11::Shutdown()
	{
	}

	ID3D11Device* RHI_D3D11::GetDevice()
	{
		return nullptr;
	}

	ID3D11DeviceContext* RHI_D3D11::GetDeviceContext()
	{
		return nullptr;
	}
}