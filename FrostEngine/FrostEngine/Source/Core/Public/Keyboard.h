#pragma once
#include <queue>
#include <bitset>


#define KEYCODE unsigned char

class Keyboard
{
	friend class Window;

public:
	
	class Event
	{
	public:
		enum class State
		{
			Pressed,
			Hold,
			Release,
			Invalid
		};
		private:
		State state;
		KEYCODE code;
		public:
			Event()
				:
				state(State::Invalid),
				code(0)
			{

			}

			Event(State state, KEYCODE code)
				:
				state(state),
				code(code)
			{

			}

			bool IsKeyPressed() const noexcept
			{
				return state == State::Pressed;
			}

			bool IsKeyReleased() const noexcept
			{
				return state == State::Release;
			}

			bool IsValid() const noexcept
			{
				return state != State::Invalid;
			}

			KEYCODE GetCode() const noexcept
			{
				return code;
			}

	};
public:
	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;

	bool KeyIsPressed(KEYCODE keycode) const noexcept;
	Event ReadKey() noexcept;
	__inline bool KeyBufferIsEmpty() const noexcept { return keybuffer.empty(); }
	void FlushKey() noexcept;
	// char stuff
	char ReadChar() noexcept;
	__inline bool CharIsEmpty() const noexcept { return charbuffer.empty(); }
	void FlushChar() noexcept;
	void Flush() noexcept;
	// autorepeat
	__inline void EnableAutoRepeat() noexcept { bAutoRepeat = true; };
	__inline void DisableAutoRepeat() noexcept	  { bAutoRepeat = false; };
	__inline bool IsAutoRepeatEnabled() noexcept { return bAutoRepeat; };

private:
	void OnKeyPressed(KEYCODE keycode) noexcept;
	void OnKeyReleased(KEYCODE keycode) noexcept;
	void OnChar(char character) noexcept;
	void ClearState() noexcept;

	template<typename T>
	static void TrimBuffer(std::queue<T>& buffer) noexcept;
private:
	static constexpr unsigned int nKeys = 256u;
	static constexpr unsigned int bufferSize = 16u;
	bool bAutoRepeat = false;
	std::bitset<nKeys> keystates;
	std::queue<Event> keybuffer;
	std::queue<char> charbuffer;

};

