#pragma once

enum class KeyType
{
	W = 'W',
	S = 'S',
	A = 'A',
	D = 'D',
	E = 'E',
	LeftMouse = VK_LBUTTON,
	RightMouse = VK_RBUTTON,
	ESC = VK_ESCAPE,
};

enum class KeyState
{
	None,
	Pressed,
	Down,
	Up,
	End

};

const int KEY_TYPE_COUNT = static_cast<int>(256) + 1;
const int KEY_STATE_COUNT = static_cast<int>(KeyState::End);

class InputManager
{
public:
	void Init();
	void Update();

	bool GetButtonPressed(KeyType type) { return GetState(type) == KeyState::Pressed; }
	bool GetButtonDown(KeyType type) { return GetState(type) == KeyState::Down; }

	bool GetButtonUp(KeyType type) { return GetState(type) == KeyState::Up; }
	Vector2 GetMousePos() { return { (float)_mousePos.x, (float)_mousePos.y }; }
	Vector2 GetMouseMovement() { return _mouseMovement; }

private:
	KeyState GetState(KeyType key) { return _keyStates[static_cast<unsigned char>(key)]; }
private:
	HWND _hwnd = 0;
	KeyState _keyStates[KEY_TYPE_COUNT];
	POINT _prevMousePos{};
	POINT _mousePos{};
	Vector2 _mouseMovement{};

};

