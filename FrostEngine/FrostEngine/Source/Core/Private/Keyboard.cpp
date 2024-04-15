#include "Keyboard.h"

bool Keyboard::KeyIsPressed(KEYCODE keycode) const noexcept
{
	return keystates[keycode];
}

Keyboard::Event Keyboard::ReadKey() noexcept
{
	if (!KeyBufferIsEmpty() && keybuffer.size() > 0u)
	{
		Keyboard::Event e = keybuffer.front();
		keybuffer.pop();
		return e;
	}
	else
	{
		return Keyboard::Event();
	}
}

void Keyboard::FlushKey() noexcept
{
	keybuffer = std::queue<Event>();
}


char Keyboard::ReadChar() noexcept
{
	if (charbuffer.size() > 0u)
	{
		KEYCODE code = charbuffer.front();
		charbuffer.pop();
		return code;
	}
	else
	{
		return 0;
	}
}

void Keyboard::FlushChar() noexcept
{
	charbuffer = std::queue<char>();
}

void Keyboard::Flush() noexcept
{
	FlushKey();
	FlushChar();
}

void Keyboard::OnKeyPressed(KEYCODE keycode) noexcept
{
	keystates[keycode] = true;
	keybuffer.push(Keyboard::Event(Keyboard::Event::Type::Pressed, keycode));
	TrimBuffer(keybuffer);
}

void Keyboard::OnKeyReleased(KEYCODE keycode) noexcept
{
	keystates[keycode] = false;
	keybuffer.push(Keyboard::Event(Keyboard::Event::Type::Release, keycode));
	TrimBuffer(keybuffer);
}

void Keyboard::OnChar(char character) noexcept
{
	charbuffer.push(character);
	TrimBuffer(charbuffer);
}

void Keyboard::ClearState() noexcept
{
	keystates.reset();
}

template<typename T>
inline void Keyboard::TrimBuffer(std::queue<T>& buffer) noexcept
{
	while (buffer.size() > bufferSize)
	{
		buffer.pop();
	}
}
