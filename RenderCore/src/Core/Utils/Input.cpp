#include "Core/Utils/Input.hpp"

namespace rec
{
    bool Input::m_keys_pressed[static_cast<size_t>(KeyCode::KEY_LAST) + 1] = {};
    bool Input::m_mouse_buttons_pressed[static_cast<size_t>(MouseButton::MOUSE_BUTTON_LAST) + 1] = {};
    int Input::m_mouse_positions[4] = {};

    bool Input::IsKeyPressed(const KeyCode key_code)
    {
        return m_keys_pressed[static_cast<size_t>(key_code)];
    }

    void Input::PressKey(const KeyCode key_code)
    {
        m_keys_pressed[static_cast<size_t>(key_code)] = true;
    }

    void Input::ReleaseKey(const KeyCode key_code)
    {
        m_keys_pressed[static_cast<size_t>(key_code)] = false;
    }

    bool Input::IsMouseButtonPressed(const MouseButton mouse_button)
    {
        return m_mouse_buttons_pressed[static_cast<size_t>(mouse_button)];
    }

    void Input::PressMouseButton(const MouseButton mouse_button)
    {
        m_mouse_buttons_pressed[static_cast<size_t>(mouse_button)] = true;
    }

    void Input::ReleaseMouseButton(const MouseButton mouse_button)
    {
        m_mouse_buttons_pressed[static_cast<size_t>(mouse_button)] = false;
    }


    int Input::GetPreviousMouseXPosition()
    {
        return m_mouse_positions[0];
    }
    int Input::GetPreviousMouseYPosition()
    {
        return m_mouse_positions[1];
    }
    int Input::GetCurrentMouseXPosition()
    {
        return m_mouse_positions[2];
    }
    int Input::GetCurrentMouseYPosition()
    {
        return m_mouse_positions[3];
    }
    void Input::SetPreviousMousePositions(const int XY[2])
    {
        m_mouse_positions[0] = XY[0];
        m_mouse_positions[1] = XY[1];
    }
    void Input::SetCurrentMousePositions(const int XY[2])
    {
        m_mouse_positions[2] = XY[0];
        m_mouse_positions[3] = XY[1];
    }
}