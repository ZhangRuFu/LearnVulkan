#include <Windows.h>

#include "Platform\Win32\WindowsWankelEngine.h"

WindowsWankelEngine *gWankelEngine = nullptr;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreinstance, LPSTR cmdLine, int cmdShow)
{
	const char* wndClassName = "WankelEngineWndClass";

	//Register WndClass
	WNDCLASSEXA wndClass = {};
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;

	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.hInstance = hInstance;
	wndClass.lpszMenuName = nullptr;
	wndClass.lpszClassName = wndClassName;
	wndClass.lpfnWndProc = WndProc;

	wndClass.hbrBackground = nullptr;
	wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wndClass.hIcon = nullptr;
	wndClass.hIconSm = nullptr;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	
	ATOM wndClassID = RegisterClassEx(&wndClass);
	if (!wndClassID)
		return E_FAIL;

	//Create Window
	RECT clientRect = {};
	clientRect.right = 800;
	clientRect.bottom = 800;
	AdjustWindowRect(&clientRect, WS_OVERLAPPED, false);
	HWND hWindow = CreateWindow(wndClassName, "Wankel Engine", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, nullptr, nullptr, hInstance, nullptr);
	if (hWindow == nullptr)
		return E_FAIL;

	//Init Engine
	WindowsWankelEngine::Init();
	gWankelEngine = WindowsWankelEngine::Instance();
	

	//Show Window
	ShowWindow(hWindow, cmdShow);

	//Message Loop
	MSG message = {};
	while (message.message != WM_QUIT)
	{
		if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		/*
		else
			gWankelEngine->OnUpdate();
			*/
	}

	return static_cast<int>(message.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		//Destroy Engine
		WindowsWankelEngine::Destroy();
		
		PostQuitMessage(S_OK);
		break;
	case WM_CREATE:
		//Do not init engine here
		//window can't create yet

		break;
	case WM_ERASEBKGND:
		return 1;
	case WM_PAINT:
		ValidateRect(hwnd, nullptr);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}