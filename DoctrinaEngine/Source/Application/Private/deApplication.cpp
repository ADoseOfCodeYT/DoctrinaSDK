#include "Application/Public/deApplication.h"
#include "Core/Public/deVersion.h"

namespace de
{
	Application::Application(LPCSTR name, int screenwidth, int screenheight)
	{
		Name = name;
		ScreenWidth = screenwidth;
		ScreenHeight = screenheight;

		m_HINSTANCE = NULL;
		m_HWND = NULL;
	}

	Application::~Application()
	{

	}

	void Application::Initialize()
	{
		if (Initialised)
		{
			return;
		}
		Initialised = true;
		InitializeWindows();

	}

	void Application::Shutdown()
	{
		ShutdownWindows();

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

				if (FrameIndependentUpdate)
				{
					// TODO: DO THIS
				}
				else
				{
					result = FixedUpdate();
					if (!result)
					{
						done = true;
					}
				}
			}

		}

		return;
	}

	LRESULT CALLBACK Application::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
	{
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}

	bool Application::FixedUpdate()
	{
		bool result;


		return true;
	}

	bool Application::Update(float dt)
	{
		bool result; 

		return true;
	}

	void Application::InitializeWindows()
	{
		WNDCLASSEX WC;
		DEVMODE DMScreenSettings;
		int posx, posy;
		int screenwidth, screenheight;

		m_HINSTANCE = GetModuleHandle(NULL);

		WC.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		WC.lpfnWndProc = WndProc;
		WC.cbClsExtra = 0;
		WC.cbWndExtra = 0;
		WC.hInstance = m_HINSTANCE;
		WC.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		WC.hIconSm = WC.hIcon;
		WC.hCursor = LoadCursor(NULL, IDC_ARROW);
		WC.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		WC.lpszMenuName = NULL;
		WC.lpszClassName = Name;
		WC.cbSize = sizeof(WNDCLASSEX);

		RegisterClassEx(&WC);

		screenwidth = GetSystemMetrics(SM_CXSCREEN);
		screenheight = GetSystemMetrics(SM_CYSCREEN);

		screenwidth = ScreenWidth;
		screenheight = ScreenHeight;

		posx = (GetSystemMetrics(SM_CXSCREEN) - screenwidth) / 2;
		posy = (GetSystemMetrics(SM_CYSCREEN) - screenheight) / 2;

		m_HWND = CreateWindowEx(WS_EX_APPWINDOW, Name, Name, WS_OVERLAPPEDWINDOW, posx, posy, screenwidth, screenheight, NULL, NULL, m_HINSTANCE, NULL);

		ShowWindow(m_HWND, SW_SHOW);
		SetForegroundWindow(m_HWND);
		SetFocus(m_HWND);

		ShowCursor(true);

		return;
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