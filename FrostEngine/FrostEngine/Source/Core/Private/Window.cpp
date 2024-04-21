#include "Window.h"
#include <sstream>
#include "resource.h"


// Window Class Stuff
Window::WindowClass Window::WindowClass::wndClass;


Window::Window(int _width, int _height, const char* name) 
 : width(_width)
 , height(_height)
{
	RECT wr;
	wr.left = 100;
	wr.top = 100;
	wr.right = width + wr.left;
	wr.bottom = height + wr.top;

	if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == false)
	{
		throw FRWND_LAST_EXCEPT();
	}

	hWnd = CreateWindow(
		WindowClass::GetName(), name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, WindowClass::GetInstance(), this
	);

	if (hWnd == nullptr)
	{
		throw FRWND_LAST_EXCEPT();
	}

	ShowWindow(hWnd, SW_SHOWDEFAULT);
	pRenderer = std::make_unique<Renderer>(hWnd);
}

void Window::SetTitle(const std::string titleText)
{
	if (titleText == TitleText)
	{
		return;
	}
	// TODO: Be wary of spamming window settings, add timer to block canging title text for 1 second or half a second
	
	TitleText = titleText;
	
	if (SetWindowText(hWnd, titleText.c_str()) == false)
	{
		throw FRWND_LAST_EXCEPT();
	}
}

std::optional<int> Window::ProcessMessages()
{
	MSG msg;
	
	// while there are messages remove ad dispatch them
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return msg.wParam;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return {};
}

Renderer& Window::GetRenderer()
{
	return *pRenderer;
}

Window::~Window()
{
	DestroyWindow(hWnd);
}

LRESULT CALLBACK Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// use create parameter passed in from CreateWindow to store pointer
	if (msg == WM_NCCREATE)
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);

		// set winAPI managed data to store pointer to our window class
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		// reroute handle msg pointer to our actual call
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgWinCall));
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::HandleMsgWinCall(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	return pWnd->HandleMsg(hWnd, msg, wParam,lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (msg)
	{
		// Close Screen
		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;
		case WM_KILLFOCUS:
			KBInput.ClearState();
			break;

		/* Keyboard Input */

		// We check against the previous keystate if it was down using bit 30 on the windows keyflags 
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			if ( !(lParam & 0x40000000) || KBInput.IsAutoRepeatEnabled())
			{ 
				KBInput.OnKeyPressed(static_cast<KEYCODE>(wParam));
			}
			break;
		case WM_KEYUP:
		case WM_SYSKEYUP:
			KBInput.OnKeyReleased(static_cast<KEYCODE>(wParam));
			break;
		case WM_CHAR:
			KBInput.OnChar(static_cast<KEYCODE>(wParam));
			break;
		/* END OF KEYBOARD */

		/****** MOVE EVENTS ******/
		case WM_MOUSEMOVE:
		{
			
			// Check if we are inside

			POINTS pt = MAKEPOINTS(lParam);

			// Within window
			if (pt.x >= 0 && pt.x < width && pt.y >= 0 && pt.y < height)
			{
				MouseInput.OnMouseMove(pt.x, pt.y);
				if (!MouseInput.IsMouseInWindow())
				{
					SetCapture(hWnd);
					MouseInput.OnMouseEnter();
				}

			}
			else
			{
				if (wParam & (MK_LBUTTON | MK_RBUTTON))
				{
					MouseInput.OnMouseMove(pt.x, pt.y);
				}
				else
				{
					ReleaseCapture();
					MouseInput.OnMouseLeave();
				}
			}
			break;
		}
		case WM_MOUSEWHEEL:
		{
			POINTS pt = MAKEPOINTS(lParam);
			const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
			MouseInput.HandleWheelDelta(pt.x, pt.y, delta);
			break;
		}
		case WM_LBUTTONDOWN:
		{
			POINTS pt = MAKEPOINTS(lParam);
			MouseInput.OnLeftPressed(pt.x, pt.y);
			break;
		}
		case WM_LBUTTONUP:
		{
			POINTS pt = MAKEPOINTS(lParam);
			MouseInput.OnLeftReleased(pt.x, pt.y);
			break;
		}
		case WM_RBUTTONDOWN:
		{
			POINTS pt = MAKEPOINTS(lParam);
			MouseInput.OnRightPressed(pt.x, pt.y);
			break;
		}
		case WM_RBUTTONUP:
		{
			POINTS pt = MAKEPOINTS(lParam);
			MouseInput.OnRightReleased(pt.x, pt.y);
			break;
		}
		case WM_LBUTTONDBLCLK:
		case WM_RBUTTONDBLCLK:
		break;
		/****** END MOVE EVENTS ******/

	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

#pragma region WindowClass
const char* Window::WindowClass::GetName() noexcept
{
	return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return  wndClass.hInst;
}

Window::WindowClass::WindowClass() 
	: hInst(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_APPICON), IMAGE_ICON, 32, 32, 0));
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_APPICON), IMAGE_ICON, 16, 16, 0));
	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass() noexcept
{
	UnregisterClass(wndClassName, GetInstance());
}
#pragma endregion

#pragma  region Window Exception
Window::Exception::Exception(int lin, const char* file, HRESULT hr)
	: 
	FRException(lin, file),
	hr(hr)
{

}

const char* Window::Exception::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
	<< "[Error Code] " << GetErrorCode() << std::endl
	<< "[Description] " << GetErrorString() << std::endl
	<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Window::Exception::GetType() const noexcept
{
	return "Frost Window Exception";
}

std::string Window::Exception::TranslateErrorCode(HRESULT hr) noexcept
{
	char* pMsgBuf = nullptr;
	DWORD nMsgLen = FormatMessage(
	FORMAT_MESSAGE_ALLOCATE_BUFFER |
	FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
	nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
	reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr);

	if (nMsgLen == 0)
	{
		return "Unknown Error Code, you fucked up somewhere bad";
	}
	std::string errorStr = pMsgBuf;
	LocalFree(pMsgBuf);
	return errorStr;
}

HRESULT Window::Exception::GetErrorCode() const noexcept
{
	return hr;
}

std::string Window::Exception::GetErrorString() const noexcept
{
	return TranslateErrorCode(hr);
}
#pragma endregion
