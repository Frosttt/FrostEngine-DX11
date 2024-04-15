#include "Application.h"
#include <sstream>
#include <iomanip>

Application::Application()
: wnd(Window(800, 400, "Frost Engine"))
{

}

int Application::Run()
{
	//Message
	while (true)
	{
		if (const auto ecode = Window::ProcessMessages())
		{
			return *ecode;
		}


		Update();
	}
	return 0;
}

void Application::DrawFrame()
{
	std::ostringstream oss;
	const float t = Uptime.Peek();
	oss << "Frost Engine: " << std::setprecision(1) << std::fixed << t;
	wnd.SetTitle(oss.str());
}

void Application::Update()
{
	DrawFrame();
}
