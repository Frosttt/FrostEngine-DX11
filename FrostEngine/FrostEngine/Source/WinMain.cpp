#include "FrWin.h"
#include "WindowsMessageMap.h"
#include <iostream>
#include <sstream>
#include "Window.h"




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


	try
	{
		Window win(800, 400, "Frost Engine");

		//Message
		MSG WinMsg;
		BOOL gResult;

		// Main windows Loop
		while (gResult = GetMessage(&WinMsg, nullptr, 0, 0) > 0)
		{
			TranslateMessage(&WinMsg);
			DispatchMessage(&WinMsg);

			if (win.KBInput.KeyIsPressed(VK_SPACE))
			{
				MessageBox(nullptr, "You are approaching 950 zells in your computer", "Zell", MB_OK | MB_ICONEXCLAMATION);
			}

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

	// Exception Handling
	catch (const FRException& e)
	{
		MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e)
	{
		MessageBox(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch ( ... )
	{
		MessageBox(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);

	}
	return -1;


}
