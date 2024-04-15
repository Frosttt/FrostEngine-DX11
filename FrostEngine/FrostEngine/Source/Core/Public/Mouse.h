#pragma once

#include <utility>
#include <queue>

class Mouse
{

	friend class Window;
public:
	class Event
	{
	public:
		enum class Type
		{
			LPress,
			LRelease,
			RPress,
			RRelease,
			WheelUp,
			WheelDown,
			Move,
			Enter,
			Leave,
			Invalid
		};
	private:
		Type type;
		bool bLeftMousePressed;
		bool bRightMousePressed;
		int x;
		int y;
		bool bInWindow;
	public:
		Event() noexcept
			: type(Type::Invalid)
			, bLeftMousePressed(false)
			, bRightMousePressed(false)
			, x(0)
			, y(0)
		{

		}

		Event(Type type, const Mouse& Parent) noexcept
			: type(type)
			, bLeftMousePressed(Parent.bLeftMousePressed)
			, bRightMousePressed(Parent.bRightMousePressed)
			, x(Parent.x)
			, y(Parent.y)
			, bInWindow(Parent.bInWindow)
		{

		}

		__inline Type GetType() const noexcept { return type; };
		__inline std::pair<int, int> GetPos() const noexcept { return {x, y}; };
		__inline int GetPosX() const noexcept { return x; };
		__inline int GetPosY() const noexcept { return y; };
		__inline bool IsLeftMouseButtonPressed() const noexcept { return bLeftMousePressed; };
		__inline bool IsRightMouseButtonPressed() const noexcept { return bRightMousePressed; };
		__inline bool IsMouseInWindow() const noexcept { return bInWindow; };
	};

public:
	Mouse() = default;
	Mouse(const Mouse&) = delete;
	__inline std::pair<int, int> GetPos() const noexcept { return { x, y }; };
	__inline int GetPosX() const noexcept { return x; };
	__inline int GetPosY() const noexcept { return y; };
	__inline bool IsLeftMouseButtonPressed() const noexcept { return bLeftMousePressed; };
	__inline bool IsRightMouseButtonPressed() const noexcept { return bRightMousePressed; };
	__inline bool IsMouseInWindow() const noexcept { return bInWindow; };
	Mouse::Event Read() noexcept;
	__inline bool IsEmpty() const noexcept { return buffer.empty(); };
	void Flush() noexcept;

private:
	void OnMouseMove(int x, int y) noexcept;
	void OnLeftPressed(int x, int y) noexcept;
	void OnLeftReleased(int x, int y) noexcept;
	void OnRightPressed(int x, int y) noexcept;
	void OnRightReleased(int x, int y) noexcept;
	void OnWheelUp(int x, int y) noexcept;
	void OnWheelDown(int x, int y) noexcept;
	void OnMouseEnter() noexcept;
	void OnMouseLeave() noexcept;
	void HandleWheelDelta(int x, int y, int delta) noexcept;
	void TrimBuffer() noexcept;

	

private:
	static constexpr unsigned int bufferSize = 16u;
	int x;
	int y;
	bool bLeftMousePressed = false;
	bool bRightMousePressed = false;
	bool bInWindow = false;
	int wheelDeltaCarry = 0;
	std::queue<Event> buffer;
};

