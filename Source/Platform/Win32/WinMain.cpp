#include <Windows.h>

#include "Core\WankelEngine.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPreinstance, LPTSTR cmdLine, int cmdShow)
{
	const char16_t* wndClassName = u"WankelEngineWndClass";

	//Register WndClass
	WNDCLASSEXW wndClass = {};
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;

	wndClass.cbSize = sizeof(WNDCLASSEXW);
	wndClass.hInstance = hInstance;
	wndClass.lpszMenuName = nullptr;
	wndClass.lpszClassName = (wchar_t*)wndClassName;
	wndClass.lpfnWndProc = WndProc;

	wndClass.hbrBackground = nullptr;
	wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wndClass.hIcon = nullptr;
	wndClass.hIconSm = nullptr;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	
	ATOM wndClassID = RegisterClassExW(&wndClass);
	if (!wndClassID)
		return E_FAIL;

	//Create Window
	RECT clientRect = {};
	clientRect.right = 800;
	clientRect.bottom = 800;
	AdjustWindowRect(&clientRect, WS_OVERLAPPED, false);
	HWND hWindow = CreateWindowW((wchar_t*)wndClassName, L"Engine", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, nullptr, nullptr, hInstance, nullptr);
	if (hWindow == nullptr)
		return E_FAIL;

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
		else
			WankelEngine::Instance()->Update();
	}

	return message.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		//Destroy Engine
		WankelEngine::Destroy();
		
		PostQuitMessage(S_OK);
		break;
	case WM_CREATE:
		//Init Engine
		//TODO : How Unity do?
		WankelEngine::Init();

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