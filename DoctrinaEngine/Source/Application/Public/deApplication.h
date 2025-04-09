#ifndef _DEAPPLICATION_H_
#define _DEAPPLICATION_H_

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <iostream>

namespace de
{
	class Application
	{
	public:
		Application(LPCSTR name, int screenWidth, int screenHeight);
		~Application();

		void Initialize();
		void Shutdown();
		void Run();

		HWND GetWindow();

		LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

	public:
		LPCSTR Name;

		int ScreenWidth, ScreenHeight;

		bool FrameIndependentUpdate = false;
		bool Initialised = false;
		bool VSync = true; // TODO: MOVE THIS TO A DIFFERENT CLASS AND MAKE IT CHANGEABLE WHILE APP IS RUNNING
		bool Fullscreen = false; // TODO: MOVE THIS TO A DIFFERENT CLASS AND MAKE IT CHANGEABLE WHILE APP IS RUNNING

	private:
		bool FixedUpdate();
		bool Update(float dt);
		bool InitializeWindows();
		void ShutdownWindows();

	private:

		HINSTANCE m_HINSTANCE;
		HWND m_HWND;
	};
}

static LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

static de::Application* AppHandle = 0;

#endif // !_DEAPPLICATION_H_
