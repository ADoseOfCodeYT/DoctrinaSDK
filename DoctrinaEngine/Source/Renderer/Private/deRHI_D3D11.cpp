#include "Renderer/Private/deRHI_D3D11.h"

#include <string>

#include "Tools/Public/deConsole.h"

#define RHI_DEBUG_LAYER 1

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

		ProjectionMatrix = {};
		WorldMatrix = {};
		OrthoMatrix = {};

		m_Viewport = {};

		m_AlphaBlendingState = nullptr;
		m_AlphaDisabledBlendingState = nullptr;

		m_Numerator = 0;
		m_Denominator = 0;

		VideoCardMemory = 0;
	}

	void RHI_D3D11::Initialize(SDL_Window* window, float screenDepth, float screenNear)
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

		assert(window != nullptr);

		m_Window = window;

		SDL_GetWindowSize(window, &m_WindowWidth, &m_WindowHeight);

		result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&dxFactory);
		if (FAILED(result))
		{

			console::Post("[de::RHI_D3D11] FAILED : CreateDXGIFactory()", console::LogLevel::ExtremeError);

			Initialised = false;
			return;
		}

		result = dxFactory->EnumAdapters(0, &dxAdapter);
		if (FAILED(result))
		{
			console::Post("[de::RHI_D3D11] FAILED : EnumAdapters()", console::LogLevel::ExtremeError);


			Initialised = false;
			return;
		}

		result = dxAdapter->EnumOutputs(0, &dxAdapterOutput);
		if (FAILED(result))
		{
			console::Post("[de::RHI_D3D11] FAILED : EnumOutputs()", console::LogLevel::ExtremeError);

			Initialised = false;
			return;
		}

		result = dxAdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
		if (FAILED(result))
		{
			console::Post("[de::RHI_D3D11] FAILED : GetDisplayModeList()", console::LogLevel::ExtremeError);

			Initialised = false;
			return;
		}

		displayModeList = new DXGI_MODE_DESC[numModes];
		if (!displayModeList)
		{
			console::Post("[de::RHI_D3D11] FAILED : new DXGI_MODE_DESC", console::LogLevel::ExtremeError);

			Initialised = false;
			return;
		}

		result = dxAdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
		if (FAILED(result))
		{
			console::Post("[de::RHI_D3D11] FAILED : GetDisplayModeList()", console::LogLevel::ExtremeError);

			Initialised = false;
			return;
		}

		for (i = 0; i < numModes; i++)
		{
			if (displayModeList[i].Width == (unsigned int)m_WindowWidth)
			{
				if (displayModeList[i].Height == (unsigned int)m_WindowHeight)
				{
					m_Numerator = displayModeList[i].RefreshRate.Numerator;
					m_Denominator = displayModeList[i].RefreshRate.Denominator;
				}
			}
		}

		result = dxAdapter->GetDesc(&adapterDesc);
		if (FAILED(result))
		{
			console::Post("[de::RHI_D3D11] FAILED : GetDesc()", console::LogLevel::ExtremeError);


			Initialised = false;
			return;
		}

		VideoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

		error = wcstombs_s(&stringLength, VideoCardDesc, 128, adapterDesc.Description, 128);
		if (error != 0)
		{
			console::Post("[de::RHI_D3D11] FAILED : wcstomb_s()", console::LogLevel::ExtremeError);


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

		swapChainDesc.BufferDesc.Width = m_WindowWidth;
		swapChainDesc.BufferDesc.Height = m_WindowHeight;

		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 32 bit

		// initialise the swapchain with vsync on
		swapChainDesc.BufferDesc.RefreshRate.Numerator = m_Numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = m_Denominator;

		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

		HWND hwnd = (HWND)SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_WIN32_HWND_POINTER, NULL);
		assert(hwnd != nullptr);

		swapChainDesc.OutputWindow = hwnd;

		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;

		// if (fullScreen)
		// {
		// 	swapChainDesc.Windowed = false;
		// }
		// else
		// {
		swapChainDesc.Windowed = true;
		// }

		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		swapChainDesc.Flags = 0;

		featureLevel = D3D_FEATURE_LEVEL_11_0;

		UINT creationFlags = 0;

		#if RHI_DEBUG_LAYER
			creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
		#endif


		result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, creationFlags, &featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &m_SwapChain, &m_Device, nullptr, &m_DeviceContext);
		if (FAILED(result))
		{
			console::Post("[de::RHI_D3D11] FAILED : D3D11CreateDeviceAndSwapChain()", console::LogLevel::ExtremeError);
			Initialised = false;
			return;
		}
		result = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
		if (FAILED(result))
		{
			console::Post("[de::RHI_D3D11] FAILED : GetBuffer()", console::LogLevel::ExtremeError);
			Initialised = false;
			return;
		}

		result = m_Device->CreateRenderTargetView(backBufferPtr, nullptr, &m_RenderTargetView);
		backBufferPtr->Release();
		backBufferPtr = nullptr;
		if (FAILED(result))
		{
			console::Post("[de::RHI_D3D11] FAILED : CreateRenderTargetView()", console::LogLevel::ExtremeError);
			Initialised = false;
			return;
		}

		ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

		depthBufferDesc.Width = m_WindowWidth;
		depthBufferDesc.Height = m_WindowHeight;
		depthBufferDesc.MipLevels = 1;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.SampleDesc.Quality = 0;
		depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthBufferDesc.CPUAccessFlags = 0;
		depthBufferDesc.MiscFlags = 0;

		result = m_Device->CreateTexture2D(&depthBufferDesc, nullptr, &m_DepthStencilBuffer);
		if (FAILED(result))
		{
			console::Post("[de::RHI_D3D11] FAILED : CreateTexture2D()", console::LogLevel::ExtremeError);

			Initialised = false;
			return;
		}

		ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

		depthStencilDesc.StencilEnable = true;
		depthStencilDesc.StencilReadMask = 0xFF;
		depthStencilDesc.StencilWriteMask = 0xFF;

		//Front facing pixels
		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		//Back facing pixels
		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		
		result = m_Device->CreateDepthStencilState(&depthStencilDesc, &m_DepthStencilState);
		if (FAILED(result))
		{
			console::Post("[de::RHI_D3D11] FAILED : CreateDepthStencilState()", console::LogLevel::ExtremeError);

			Initialised = false;
			return;
		}


		m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState, 1);

		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		result = m_Device->CreateDepthStencilView(m_DepthStencilBuffer, &depthStencilViewDesc, &m_DepthStencilView);
		if (FAILED(result))
		{
			console::Post("[de::RHI_D3D11] FAILED : CreateDepthStencilView()", console::LogLevel::ExtremeError);

			Initialised = false;
			return;
		}

		m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);

		rasterDesc.AntialiasedLineEnable = false;
		rasterDesc.CullMode = D3D11_CULL_NONE;
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0.0f;
		rasterDesc.DepthClipEnable = true;
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.FrontCounterClockwise = false;
		rasterDesc.MultisampleEnable = false;
		rasterDesc.ScissorEnable = false;
		rasterDesc.SlopeScaledDepthBias = 0.0f;

		result = m_Device->CreateRasterizerState(&rasterDesc, &m_RasterState);
		if (FAILED(result))
		{
			console::Post("[de::RHI_D3D11] FAILED : CreateTexture2D()", console::LogLevel::ExtremeError);

			Initialised = false;
			return;
		}

		m_DeviceContext->RSSetState(m_RasterState);

		m_Viewport.Width = (float)m_WindowWidth;
		m_Viewport.Height = (float)m_WindowHeight;
		m_Viewport.MinDepth = 0.0f;
		m_Viewport.MaxDepth = 1.0f;
		m_Viewport.TopLeftX = 0.0f;
		m_Viewport.TopLeftY = 0.0f;

		m_DeviceContext->RSSetViewports(1, &m_Viewport);

		fieldOfView = 3.141592654f / 4.0f;
		screenAspect = (float)m_WindowWidth / (float)m_WindowHeight;

		ProjectionMatrix = math::MatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);

		WorldMatrix = math::IdentityMatrix();

		OrthoMatrix = math::MatrixOrthographicLH((float)m_WindowWidth, (float)m_WindowHeight, screenNear, screenDepth);

		ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

		//Depth stencil for 2D renderiing
		depthDisabledStencilDesc.DepthEnable = false;
		depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
		depthDisabledStencilDesc.StencilEnable = true;
		depthDisabledStencilDesc.StencilReadMask = 0xFF;
		depthDisabledStencilDesc.StencilWriteMask = 0xFF;
		depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		result = m_Device->CreateDepthStencilState(&depthDisabledStencilDesc, &m_DepthDisabledStencilState);
		if (FAILED(result))
		{
			console::Post("[de::RHI_D3D11] FAILED : CreateDepthStencilState()", console::LogLevel::ExtremeError);

			Initialised = false;
			return;
		}

		ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

		blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
		blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

		result = m_Device->CreateBlendState(&blendStateDescription, &m_AlphaBlendingState);
		if (FAILED(result))
		{
			console::Post("[de::RHI_D3D11] FAILED : CreateBlendState()", console::LogLevel::ExtremeError);

			Initialised = false;
			return;
		}

		blendStateDescription.RenderTarget[0].BlendEnable = FALSE;

		result = m_Device->CreateBlendState(&blendStateDescription, &m_AlphaDisabledBlendingState);
		if (FAILED(result))
		{
			console::Post("[de::RHI_D3D11] FAILED : CreateBlendState()", console::LogLevel::ExtremeError);

			Initialised = false;
			return;
		}

		console::Post("[de::RHI_D3D11] Initialised", console::LogLevel::Default);

		Initialised = true;

		
	}

	void RHI_D3D11::Shutdown()
	{
		if (m_SwapChain)
		{
			m_SwapChain->SetFullscreenState(false, nullptr);
			m_SwapChain->Release();
			m_SwapChain = nullptr;
		}

		if (m_Device)
		{
			m_Device->Release();
			m_Device = nullptr;
		}

		if (m_DeviceContext)
		{
			m_DeviceContext->Release();
			m_DeviceContext = nullptr;
		}

		if (m_RenderTargetView)
		{
			m_RenderTargetView->Release();
			m_RenderTargetView = nullptr;
		}

		if (m_DepthStencilBuffer)
		{
			m_DepthStencilBuffer->Release();
			m_DepthStencilBuffer = nullptr;
		}

		if (m_DepthStencilState)
		{
			m_DepthStencilState->Release();
			m_DepthStencilState = nullptr;
		}

		if (m_DepthDisabledStencilState)
		{
			m_DepthDisabledStencilState->Release();
			m_DepthDisabledStencilState = nullptr;
		}

		if (m_DepthStencilView)
		{
			m_DepthStencilView->Release();
			m_DepthStencilView = nullptr;
		}

		if (m_RasterState)
		{
			m_RasterState->Release();
			m_RasterState = nullptr;
		}

		if (m_AlphaBlendingState)
		{
			m_AlphaBlendingState->Release();
			m_AlphaBlendingState = nullptr;
		}

		if (m_AlphaDisabledBlendingState)
		{
			m_AlphaDisabledBlendingState->Release();
			m_AlphaDisabledBlendingState = nullptr;
		}

		console::Post("[de::RHI_D3D11] Shutdown", console::LogLevel::Default);
	}

	void RHI_D3D11::BeginFrame()
	{
		float color[4];

		color[0] = 1.0f;
		color[1] = 0.0f;
		color[2] = 1.0f;
		color[3] = 1.0f;

		m_DeviceContext->ClearRenderTargetView(m_RenderTargetView, color);

		m_DeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	void RHI_D3D11::FinishFrame()
	{
		// TODO : ADD VSYNC TOGGLE
		m_SwapChain->Present(1, 0); // vsync

	}

	void RHI_D3D11::ResizeWindow()
	{
		if(Initialised)
		{
			HRESULT result;
			D3D11_VIEWPORT newViewPort;
			ID3D11Texture2D* currentBuffer;

			
			m_RenderTargetView->Release();

        	result = m_SwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

        	result = m_SwapChain->GetBuffer(0, __uuidof( ID3D11Texture2D),(void**) &currentBuffer );

        	result = m_Device->CreateRenderTargetView(currentBuffer, nullptr, &m_RenderTargetView);

        	currentBuffer->Release();

			SDL_GetWindowSize(m_Window, &m_WindowWidth, &m_WindowHeight);

        	m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, nullptr );
        	
        	newViewPort.Width = m_WindowWidth;
        	newViewPort.Height = m_WindowHeight;
        	newViewPort.MinDepth = 0.0f;
        	newViewPort.MaxDepth = 1.0f;
        	newViewPort.TopLeftX = 0;
        	newViewPort.TopLeftY = 0;

        	m_DeviceContext->RSSetViewports( 1, &newViewPort );
		}
	}

	ID3D11Device* RHI_D3D11::GetDevice()
	{
		return m_Device;
	}

	ID3D11DeviceContext* RHI_D3D11::GetDeviceContext()
	{
		return m_DeviceContext;
	}

	void RHI_D3D11::GetSwapChainNumerator(int& numerator)
	{
		numerator = m_Numerator;
	}

	void RHI_D3D11::GetSwapChainDenominator(int& denominator)
	{
		denominator = m_Denominator;
	}

	void RHI_D3D11::EnableZBuffer()
	{
		m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState, 1);
	}

	void RHI_D3D11::DisableZBuffer()
	{
		m_DeviceContext->OMSetDepthStencilState(m_DepthDisabledStencilState, 1);
	}

	void RHI_D3D11::EnableAlphaBlending()
	{
		float blendFactor[4];


		// Setup the blend factor.
		blendFactor[0] = 0.0f;
		blendFactor[1] = 0.0f;
		blendFactor[2] = 0.0f;
		blendFactor[3] = 0.0f;

		// Turn on the alpha blending.
		m_DeviceContext->OMSetBlendState(m_AlphaBlendingState, blendFactor, 0xffffffff);
	}

	void RHI_D3D11::DisableAlphaBlending()
	{
		float blendFactor[4];


		// Setup the blend factor.
		blendFactor[0] = 0.0f;
		blendFactor[1] = 0.0f;
		blendFactor[2] = 0.0f;
		blendFactor[3] = 0.0f;

		// Turn on the alpha blending.
		m_DeviceContext->OMSetBlendState(m_AlphaDisabledBlendingState, blendFactor, 0xffffffff);
	}
	
}