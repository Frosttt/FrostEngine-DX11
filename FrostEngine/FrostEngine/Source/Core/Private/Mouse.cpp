#include "Mouse.h"
#include "FrWin.h"

Mouse::Event Mouse::Read() noexcept
{
	if (buffer.size() > 0u)
	{
		Mouse::Event e = buffer.front();
		buffer.pop();
		return e;
	}
	else
	{
		return Mouse::Event();
	}
}

void Mouse::Flush() noexcept
{
	buffer = std::queue<Event>();

}

void Mouse::OnMouseMove(int newx, int newy) noexcept
{
	x = newx;
	y = newy;

	buffer.push(Mouse::Event(Mouse::Event::Type::Move, *this));
	TrimBuffer();

}

void Mouse::OnLeftPressed(int x, int y) noexcept
{
	bLeftMousePressed = true;

	buffer.push(Mouse::Event(Mouse::Event::Type::LPress, *this));
	TrimBuffer();
}

void Mouse::OnLeftReleased(int x, int y) noexcept
{
	bLeftMousePressed = false;

	buffer.push(Mouse::Event(Mouse::Event::Type::LRelease, *this));
	TrimBuffer();
}

void Mouse::OnRightPressed(int x, int y) noexcept
{
	bRightMousePressed = true;

	buffer.push(Mouse::Event(Mouse::Event::Type::RPress, *this));
	TrimBuffer();
}

void Mouse::OnRightReleased(int x, int y) noexcept
{
	bRightMousePressed = true;

	buffer.push(Mouse::Event(Mouse::Event::Type::RRelease, *this));
	TrimBuffer();
}

void Mouse::OnWheelUp(int x, int y) noexcept
{
	buffer.push(Mouse::Event(Mouse::Event::Type::WheelUp, *this));
	TrimBuffer();
}

void Mouse::OnWheelDown(int x, int y) noexcept
{
	buffer.push(Mouse::Event(Mouse::Event::Type::WheelDown, *this));
	TrimBuffer();
}

void Mouse::OnMouseEnter() noexcept
{
	bInWindow = true;

	buffer.push(Mouse::Event(Mouse::Event::Type::Enter, *this));
	TrimBuffer();
}

void Mouse::OnMouseLeave() noexcept
{
	bInWindow = false;

	buffer.push(Mouse::Event(Mouse::Event::Type::Leave, *this));
	TrimBuffer();
}


void Mouse::HandleWheelDelta(int x, int y, int delta) noexcept
{
	wheelDeltaCarry += delta;

	//generate for every 120
	while (wheelDeltaCarry >= WHEEL_DELTA)
	{
		wheelDeltaCarry -= WHEEL_DELTA;
		OnWheelUp(x,y);
	}
	while (wheelDeltaCarry <= -WHEEL_DELTA)
	{
		wheelDeltaCarry += WHEEL_DELTA;
		OnWheelDown(x, y);
	}

}

void Mouse::TrimBuffer() noexcept
{
	if (buffer.size() > bufferSize)
	{
		buffer.pop();
	}

}
