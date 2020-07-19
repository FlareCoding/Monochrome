#pragma once
#include "Event.h"

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

	KeyCode VkToMcKeycode(int vkkeycode);
	int		McToVkKeycode(KeyCode keycode);
	char	McKeycodeToChar(KeyCode keycode, bool capital, bool capslock_enabled);

	class KeyboardEvent : public Event
	{
	public:
		KeyCode keycode;

		REGISTER_EVT_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

	protected:
		KeyboardEvent(KeyCode keycode)
			: keycode(keycode) {}
	};

	class KeyPressedEvent : public KeyboardEvent
	{
	public:
		KeyPressedEvent(KeyCode keycode, bool repeated, bool capital, bool capslock_on)
			: KeyboardEvent(keycode), repeated(repeated), capital(capital), capslock_on(capslock_on) {}

		std::string ToString() const override;

		bool repeated;
		bool capital;
		bool capslock_on;

		REGISTER_EVT_CLASS_TYPE(KeyPressed)
	};

	using KeyDownEvent = KeyPressedEvent;

	class KeyReleasedEvent : public KeyboardEvent
	{
	public:
		KeyReleasedEvent(KeyCode keycode)
			: KeyboardEvent(keycode) {}

		std::string ToString() const override;

		REGISTER_EVT_CLASS_TYPE(KeyReleased)
	};
}
