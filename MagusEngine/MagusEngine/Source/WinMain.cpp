#include <Windows.h>

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	LPCTSTR pWindowName = L"Frost's Engine";
// register window
	WNDCLASSEX WinClass = { 0 };

	WinClass.cbSize = sizeof(WinClass);
	WinClass.style = CS_OWNDC;
	WinClass.lpfnWndProc = DefWindowProc;
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

	while (GetMessage(&WinMsg, nullptr, 0, 0) > 0)
	{
		TranslateMessage(&WinMsg);
		DispatchMessage(&WinMsg);
	}
	return 0;
}
