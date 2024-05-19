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
	oss << "Frost Engine: " << std::setprecision(1) << std::fixed << t << "s";
	wnd.SetTitle(oss.str());

	Renderer& Gfx = wnd.GetRenderer();

	const float c = /*sin(Uptime.Peek() / 4.0f) / 2.0f + 0.5f*/ 0;
	Gfx.ClearBuffer(c, c, c);

	float x = wnd.MouseInput.GetPosX() / 400.0f - 1.0f;
	float y = wnd.MouseInput.GetPosY() / 300.0f - 1.0f;

	Gfx.DrawTestTriangle(0, 0, 0, -Uptime.Peek() / 4.0f, -Uptime.Peek());
	Gfx.DrawTestTriangle(x, 0, 0, Uptime.Peek() * 2, Uptime.Peek());

	wnd.GetRenderer().EndFrame();
}

void Application::Update()
{
	DrawFrame();
}
