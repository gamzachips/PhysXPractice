#include "pch.h"
#include "InputManager.h"
#include "GameApp.h"

void InputManager::Init()
{
    _hwnd = GameApp::m_hWnd;

    ::GetCursorPos(&_prevMousePos);
    ::ScreenToClient(_hwnd, &_prevMousePos);
}

void InputManager::Update()
{
    BYTE asciiKeys[KEY_TYPE_COUNT] = {};
    if (::GetKeyboardState(asciiKeys) == false)
        return;

    for (unsigned int key = 0; key < KEY_TYPE_COUNT; key++)
    {
        if (asciiKeys[key] & 0x80)
        {
            KeyState& state = _keyStates[key];

            if (state == KeyState::Pressed || state == KeyState::Down)
                state = KeyState::Pressed;
            else
                state = KeyState::Down;
        }
        else
        {
            KeyState& state = _keyStates[key];
            if (state == KeyState::Pressed || state == KeyState::Down)
                state = KeyState::Up;
            else
                state = KeyState::None;
        }
    }
    _prevMousePos = _mousePos;
    ::GetCursorPos(&_mousePos);
    ::ScreenToClient(_hwnd, &_mousePos);

    _mouseMovement = { (float)(_mousePos.x - _prevMousePos.x), (float)(_mousePos.y - _prevMousePos.y) };

   //::SetCursorPos(GWinSizeX/2, GWinSizeY/2);
}
