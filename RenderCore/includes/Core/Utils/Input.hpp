#pragma once

#include "Core/Utils/Keys.hpp"

namespace rec 
{
    class Input 
    {
    public:
        static bool IsKeyPressed(const KeyCode key_code);
        static void PressKey(const KeyCode key_code);
        static void ReleaseKey(const KeyCode key_code);

        static bool IsMouseButtonPressed(const MouseButton mouse_button);
        static void PressMouseButton(const MouseButton mouse_button);
        static void ReleaseMouseButton(const MouseButton mouse_button);

        static int GetPreviousMouseXPosition();
        static int GetPreviousMouseYPosition();
        static int GetCurrentMouseXPosition();
        static int GetCurrentMouseYPosition();

        static void SetPreviousMousePositions(const int XY[2]);
        static void SetCurrentMousePositions(const int XY[2]);

    private:
        static bool m_keys_pressed[];
        static bool m_mouse_buttons_pressed[];
        static int  m_mouse_positions[];
    };
}