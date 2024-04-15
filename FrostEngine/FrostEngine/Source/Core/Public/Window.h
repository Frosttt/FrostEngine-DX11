#pragma once
#include "FrWin.h"
#include "FRException.h"
#include "Keyboard.h"
#include "Mouse.h"
#include <optional>

class Window
{
	public:
	class Exception : public FRException
	{
		public:
			Exception(int line, const char* file, HRESULT hr) ;
			const char* what() const noexcept override;
			virtual const char* GetType() const noexcept;
			static std::string TranslateErrorCode(HRESULT hr) noexcept;
			HRESULT GetErrorCode() const noexcept;
			std::string GetErrorString() const noexcept;
			private:
				HRESULT hr;


	};

	private:

		class WindowClass
		{
			public:
				static const char* GetName() noexcept;
				static HINSTANCE GetInstance() noexcept;
			private:
				WindowClass();
				~WindowClass();
				WindowClass(const WindowClass& ) = delete;
				WindowClass& operator=(const WindowClass&) = delete;
				static constexpr const char* wndClassName = "Frost Direct3D Engine Window";
				static WindowClass wndClass;
				HINSTANCE hInst;
		};

	public:
		Window(int width, int height, const char* name);
		~Window();
		Window(const Window& ) = delete;
		Window& operator=(const Window&) = delete;
		void SetTitle(const std::string titleText);
		static std::optional<int> ProcessMessages();


		Keyboard KBInput;
		Mouse MouseInput;
	private:
		
		static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK HandleMsgWinCall(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

		std::string TitleText;
		int width;
		int height;
		HWND hWnd;
};

#define FRWND_EXCEPT(hr) Window::Exception(__LINE__, __FILE__, hr);
#define FRWND_LAST_EXCEPT() Window::Exception(__LINE__, __FILE__, GetLastError());


