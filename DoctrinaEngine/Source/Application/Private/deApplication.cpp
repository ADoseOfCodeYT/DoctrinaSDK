#include "Application/Public/deApplication.h"

#include "Core/Public/deVersion.h"
#include "Tools/Public/deConsole.h"
#include "Core/Public/deCVar.h"

using namespace de::CVar;

namespace de
{

	Application::Application(LPCSTR name)
	{
		Name = name;

		m_HINSTANCE = nullptr;
		m_HWND = nullptr;

		ScreenHeight = 0;
		ScreenWidth = 0;
	}

	void Application::Initialize(int screenWidth, int screenHeight)
	{
		ScreenWidth = screenWidth;
		ScreenHeight = screenHeight;

		if (Initialised)
		{
			Console::Post("[de::Application] Already initialised", Console::LogLevel::Warning);

			return;
		}

		bool result = InitializeWindows();
		if (!result)
		{

			Console::Post("[de::Application] FAILED : InitializeWindows()", Console::LogLevel::Error);
			Initialised = false;

			return;
		}

		m_Renderer.Initialize(screenWidth, screenHeight, GetWindow(), false, 1000.0f, 0.3f);

		Console::Post("[de::Application] Initialised", Console::LogLevel::Default);

		Initialised = true;
	}

	void Application::Shutdown()
	{
		ShutdownWindows();
		m_Renderer.Shutdown();

		Console::Post("[de::Application] Shutdown", Console::LogLevel::Default);

		return;
	}

	void Application::Run()
	{
		MSG msg;
		bool done, result;

		ZeroMemory(&msg, sizeof(MSG));

		done = false;
		while (!done)
		{

			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			if (msg.message == WM_QUIT)
			{
				done = true;
			}
			else
			{
				//TODO: FIXEDUPDATE AND FRAMEINDEPENT UPDATE

				result = FixedUpdate();
				if (!result)
				{
					done = true;
				}
			}

		}

		return;
	}

	HWND Application::GetWindow()
	{
		return m_HWND;
	}

	LRESULT CALLBACK Application::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
	{
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}

	bool Application::FixedUpdate()
	{

		if (m_Renderer.Initialised)
		{
			m_Renderer.Render();
		}

		return true;
	}

	bool Application::Update(float dt)
	{

		return true;
	}

	bool Application::InitializeWindows()
	{
		WNDCLASSEX wcClassEx;
		DEVMODE DMScreenSettings{};
		int PosX, PosY;
		int screenWidth, screenHeight;

		m_HINSTANCE = GetModuleHandle(NULL);

		wcClassEx.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wcClassEx.lpfnWndProc = WndProc;
		wcClassEx.cbClsExtra = 0;
		wcClassEx.cbWndExtra = 0;
		wcClassEx.hInstance = m_HINSTANCE;
		wcClassEx.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		wcClassEx.hIconSm = wcClassEx.hIcon;
		wcClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcClassEx.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wcClassEx.lpszMenuName = NULL;
		wcClassEx.lpszClassName = Name;
		wcClassEx.cbSize = sizeof(WNDCLASSEX);

		RegisterClassEx(&wcClassEx);

		screenWidth = GetSystemMetrics(SM_CXSCREEN);
		screenHeight = GetSystemMetrics(SM_CYSCREEN);

		screenWidth = ScreenWidth;
		screenHeight = ScreenHeight;

		PosX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		PosY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;

		m_HWND = CreateWindowEx(WS_EX_APPWINDOW, Name, Name, WS_OVERLAPPEDWINDOW, PosX, PosY, screenWidth, screenHeight, NULL, NULL, m_HINSTANCE, NULL);

		ShowWindow(m_HWND, SW_SHOW);
		SetForegroundWindow(m_HWND);
		SetFocus(m_HWND);

		ShowCursor(true);

		return true;
	}

	void Application::ShutdownWindows()
	{
		ShowCursor(true);

		DestroyWindow(m_HWND);
		m_HWND = NULL;

		UnregisterClass(Name, m_HINSTANCE);
		m_HINSTANCE = NULL;

		AppHandle = NULL;

		return;
	}

	
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
		break;
	default:
		return AppHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		break;
	}
}