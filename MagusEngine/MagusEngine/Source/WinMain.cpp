#include <Windows.h>
#include "WindowsMessageMap.h"
#include <iostream>
#include <sstream>

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static WindowsMessageMap mm;
	OutputDebugStringA((mm(msg, lParam, wParam).c_str()));

	switch (msg)
	{
		case WM_CLOSE:
		PostQuitMessage(0);
		break;
		case WM_KEYDOWN:
		case WM_KEYUP:
		break;
		case WM_CHAR:
		{
			static std::string title;
			title.push_back((char)wParam);
			SetWindowTextA(hWnd, title.c_str());
		}
		break;
		case WM_LBUTTONDOWN:
		{
			POINTS pt = MAKEPOINTS(lParam);
			std::ostringstream oss;
			oss << "(" << pt.x << "," << pt.y << ")";
			SetWindowTextA(hWnd, oss.str().c_str());
		}
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	LPCTSTR pWindowName = L"Frost's Engine";
// register window
	WNDCLASSEX WinClass = { 0 };

	WinClass.cbSize = sizeof(WinClass);
	WinClass.style = CS_OWNDC;
	WinClass.lpfnWndProc = WndProc;
	WinClass.cbClsExtra = 0;
	WinClass.cbWndExtra = 0;
	WinClass.hInstance = hInstance;
	WinClass.hIcon = nullptr;
	WinClass.hCursor = nullptr;
	WinClass.hbrBackground = nullptr;
	WinClass.lpszMenuName = nullptr;
	WinClass.lpszClassName = pWindowName;
	WinClass.hIconSm = nullptr;


	RegisterClassEx(&WinClass);
// create window instance

	HWND hWindow = CreateWindowEx(0,
																pWindowName,
																L"Initial Window",
																WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU,
																200, 200,
																640, 480,
																nullptr,
																nullptr,
																hInstance,
																nullptr);

	ShowWindow(hWindow, SW_SHOW);


	//Message
	MSG WinMsg;
	BOOL gResult;
	while (gResult = GetMessage(&WinMsg, nullptr, 0, 0) > 0)
	{
		TranslateMessage(&WinMsg);
		DispatchMessage(&WinMsg);
	}

	if (gResult == -1)
	{
		return -1;
	}
	else
	{
		return WinMsg.wParam;
	}

}
