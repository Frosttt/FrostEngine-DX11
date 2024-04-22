#include "Application.h"
#include <sstream>
#include <iomanip>

Application::Application()
: wnd(Window(800, 600, "Frost Engine"))
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

	Renderer& Gfx = wnd.GetRenderer();

	const float c = sin(Uptime.Peek()) / 2.0f + 0.5f;
	Gfx.ClearBuffer(c, c, 1.0f);

	Gfx.DrawTestTriangle();

	wnd.GetRenderer().EndFrame();
}

void Application::Update()
{
	DrawFrame();
}
