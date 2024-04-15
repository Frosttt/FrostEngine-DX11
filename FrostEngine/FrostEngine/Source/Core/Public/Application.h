#pragma once
#include "Window.h"
#include "FRTimer.h"

class Application
{
	public:
	Application();

	int Run();
private:
	
	void DrawFrame();
	void Update();
private:
	Window wnd;
	FRTimer Uptime;
};

