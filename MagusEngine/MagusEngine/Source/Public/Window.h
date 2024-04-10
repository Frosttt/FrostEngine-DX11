#pragma once
#include "FrWin.h"

class Window
{
	private:
		class WindowClass
		{
			public:
				static const char* GetName() noexcept;
				static HINSTANCE GetInstance() noexcept;
			private:
				WindowClass() noexcept;
				~WindowClass() noexcept;
				WindowClass(const WindowClass& ) = delete;
				WindowClass& operator=(const WindowClass&) = delete;
				static constexpr const char* wndClassName = "Frost Direct3D Engine Window";
				static WindowClass wndClass;
				HINSTANCE hInst;
		};
		public:
			Window(int width, int height, const char* name) noexcept;
			~Window();
			Window(const Window& ) = delete;
			Window& operator=(const Window&) = delete;
};

