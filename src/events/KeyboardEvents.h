#pragma once
#include "Event.h"
#include <widgets/Layer.h>

namespace mc
{
    enum class KeyCode
    {
        None = 0,
        KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9,

        KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H, KEY_I, KEY_J, KEY_K, KEY_L, KEY_M,
        KEY_N, KEY_O, KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z,

        KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12,

        KEY_BACKSPACE, KEY_SPACE, KEY_BACKTICK, KEY_RETURN, KEY_ENTER = KEY_RETURN, KEY_CAPSLOCK, KEY_TAB, KEY_LSHIFT, KEY_RSHIFT, KEY_LCONTROL, KEY_RCONTROL, KEY_ALT, KEY_OPTION,
        KEY_UP, KEY_DOWN, KEY_RIGHT, KEY_LEFT,
        KEY_MINUS, KEY_EQUALS, KEY_COMMA, KEY_PERIOD, KEY_SEMICOLON, KEY_TICK, KEY_SQUARE_BRACKET_RIGHT, KEY_SQUARE_BRACKET_LEFT, KEY_BACKSLASH, KEY_SLASH
    };

    // Converts a native platform keycode to Monochrome's key code.
    KeyCode NativeToMcKeycode(int keycode);

    // Converts Monochrome's keycode to a native platform key code.
    int		McToNativeKeycode(KeyCode keycode);

    // Converts Monochrome's keycode to its character representation.
    char	McKeycodeToChar(KeyCode keycode, bool capital, bool capslock_enabled);

    class KeyDownEvent : public Event
    {
    public:
        inline KeyCode getKeyCode() { return get<KeyCode>("keyCode");  }
        inline char getChar() { return get<char>("char");  }
        inline bool isShiftPressed() { return get<bool>("shiftPressed"); }
        inline bool isCtrlPressed() { return get<bool>("ctrlPressed"); }
        inline bool isCapslockOn() { return get<bool>("capslockOn"); }
        inline bool isRepeated() { return get<bool>("repeated"); }
    };

    class KeyUpEvent : public Event
    {
    public:
        inline KeyCode getKeyCode() { return get<KeyCode>("keyCode"); }
        inline char getChar() { return get<char>("char"); }
    };
}
