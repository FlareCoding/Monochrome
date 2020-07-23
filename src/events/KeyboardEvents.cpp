#include "KeyboardEvents.h"

#include <Windows.h>
#include <map>

namespace mc
{
	static std::map<KeyCode, int> _monochrome_keyboard_keycode_bindings_ = {
		{ KeyCode::KEY_0, 0x30 },
		{ KeyCode::KEY_1, 0x31 },
		{ KeyCode::KEY_2, 0x32 },
		{ KeyCode::KEY_3, 0x33 },
		{ KeyCode::KEY_4, 0x34 },
		{ KeyCode::KEY_5, 0x35 },
		{ KeyCode::KEY_6, 0x36 },
		{ KeyCode::KEY_7, 0x37 },
		{ KeyCode::KEY_8, 0x38 },
		{ KeyCode::KEY_9, 0x39 },

		{ KeyCode::KEY_A, 0x41 },
		{ KeyCode::KEY_B, 0x42 },
		{ KeyCode::KEY_C, 0x43 },
		{ KeyCode::KEY_D, 0x44 },
		{ KeyCode::KEY_E, 0x45 },
		{ KeyCode::KEY_F, 0x46 },
		{ KeyCode::KEY_G, 0x47 },
		{ KeyCode::KEY_H, 0x48 },
		{ KeyCode::KEY_I, 0x49 },
		{ KeyCode::KEY_J, 0x4A },
		{ KeyCode::KEY_K, 0x4B },
		{ KeyCode::KEY_L, 0x4C },
		{ KeyCode::KEY_M, 0x4D },
		{ KeyCode::KEY_N, 0x4E },
		{ KeyCode::KEY_O, 0x4F },
		{ KeyCode::KEY_P, 0x50 },
		{ KeyCode::KEY_Q, 0x51 },
		{ KeyCode::KEY_R, 0x52 },
		{ KeyCode::KEY_S, 0x53 },
		{ KeyCode::KEY_T, 0x54 },
		{ KeyCode::KEY_U, 0x55 },
		{ KeyCode::KEY_V, 0x56 },
		{ KeyCode::KEY_W, 0x57 },
		{ KeyCode::KEY_X, 0x58 },
		{ KeyCode::KEY_Y, 0x59 },
		{ KeyCode::KEY_Z, 0x5A },

		{ KeyCode::KEY_F1, VK_F1 },
		{ KeyCode::KEY_F2, VK_F2 },
		{ KeyCode::KEY_F3, VK_F3 },
		{ KeyCode::KEY_F4, VK_F4 },
		{ KeyCode::KEY_F5, VK_F5 },
		{ KeyCode::KEY_F6, VK_F6 },
		{ KeyCode::KEY_F7, VK_F7 },
		{ KeyCode::KEY_F8, VK_F8 },
		{ KeyCode::KEY_F9, VK_F9 },
		{ KeyCode::KEY_F10, VK_F10 },
		{ KeyCode::KEY_F11, VK_F11 },
		{ KeyCode::KEY_F12, VK_F12 },

		{ KeyCode::KEY_BACKSPACE,	VK_BACK				},
		{ KeyCode::KEY_BACKSPACE,	VK_BACK				},
		{ KeyCode::KEY_SPACE,		VK_SPACE			},
		{ KeyCode::KEY_BACKTICK,	VkKeyScan('`')		},
		{ KeyCode::KEY_ENTER,		VK_RETURN			},
		{ KeyCode::KEY_CAPSLOCK,	VK_CAPITAL			},
		{ KeyCode::KEY_TAB,			VK_TAB				},
		{ KeyCode::KEY_LSHIFT,		VK_LSHIFT			},
		{ KeyCode::KEY_RSHIFT,		VK_RSHIFT			},
		{ KeyCode::KEY_LCONTROL,	VK_LCONTROL			},
		{ KeyCode::KEY_RCONTROL,	VK_RCONTROL			},
		{ KeyCode::KEY_ALT,			VK_MENU				},
		{ KeyCode::KEY_OPTION,		VK_MENU				},

		{ KeyCode::KEY_UP,		VK_UP		},
		{ KeyCode::KEY_DOWN,	VK_DOWN		},
		{ KeyCode::KEY_RIGHT,	VK_RIGHT	},
		{ KeyCode::KEY_LEFT,	VK_LEFT		},

		{ KeyCode::KEY_MINUS,				VkKeyScan('-')		},
		{ KeyCode::KEY_EQUALS,				VkKeyScan('=')		},
		{ KeyCode::KEY_COMMA,				VkKeyScan(',')		},
		{ KeyCode::KEY_PERIOD,				VkKeyScan('.')		},
		{ KeyCode::KEY_SEMICOLON,			VkKeyScan(';')	},
		{ KeyCode::KEY_TICK,				VK_OEM_7 },
		{ KeyCode::KEY_SQUARE_BRACKET_RIGHT,VkKeyScan('[')		},
		{ KeyCode::KEY_SQUARE_BRACKET_LEFT, VkKeyScan(']')		},
		{ KeyCode::KEY_BACKSLASH,			VkKeyScan('\\')		},
		{ KeyCode::KEY_SLASH,				VkKeyScan('/')		},
	};

	static std::map<KeyCode, std::pair<char, char>> _monochrome_keyboard_keycode_capital_char_bindings_ = {
		{ KeyCode::KEY_0, std::pair<char, char>('0', ')') },
		{ KeyCode::KEY_1, std::pair<char, char>('1', '!') },
		{ KeyCode::KEY_2, std::pair<char, char>('2', '@') },
		{ KeyCode::KEY_3, std::pair<char, char>('3', '#') },
		{ KeyCode::KEY_4, std::pair<char, char>('4', '$') },
		{ KeyCode::KEY_5, std::pair<char, char>('5', '%') },
		{ KeyCode::KEY_6, std::pair<char, char>('6', '^') },
		{ KeyCode::KEY_7, std::pair<char, char>('7', '&') },
		{ KeyCode::KEY_8, std::pair<char, char>('8', '*') },
		{ KeyCode::KEY_9, std::pair<char, char>('9', '(') },

		{ KeyCode::KEY_A, std::pair<char, char>('a', 'A') },
		{ KeyCode::KEY_B, std::pair<char, char>('b', 'B') },
		{ KeyCode::KEY_C, std::pair<char, char>('c', 'C') },
		{ KeyCode::KEY_D, std::pair<char, char>('d', 'D') },
		{ KeyCode::KEY_E, std::pair<char, char>('e', 'E') },
		{ KeyCode::KEY_F, std::pair<char, char>('f', 'F') },
		{ KeyCode::KEY_G, std::pair<char, char>('g', 'G') },
		{ KeyCode::KEY_H, std::pair<char, char>('h', 'H') },
		{ KeyCode::KEY_I, std::pair<char, char>('i', 'I') },
		{ KeyCode::KEY_J, std::pair<char, char>('j', 'J') },
		{ KeyCode::KEY_K, std::pair<char, char>('k', 'K') },
		{ KeyCode::KEY_L, std::pair<char, char>('l', 'L') },
		{ KeyCode::KEY_M, std::pair<char, char>('m', 'M') },
		{ KeyCode::KEY_N, std::pair<char, char>('n', 'N') },
		{ KeyCode::KEY_O, std::pair<char, char>('o', 'O') },
		{ KeyCode::KEY_P, std::pair<char, char>('p', 'P') },
		{ KeyCode::KEY_Q, std::pair<char, char>('q', 'Q') },
		{ KeyCode::KEY_R, std::pair<char, char>('r', 'R') },
		{ KeyCode::KEY_S, std::pair<char, char>('s', 'S') },
		{ KeyCode::KEY_T, std::pair<char, char>('t', 'T') },
		{ KeyCode::KEY_U, std::pair<char, char>('u', 'U') },
		{ KeyCode::KEY_V, std::pair<char, char>('v', 'V') },
		{ KeyCode::KEY_W, std::pair<char, char>('w', 'W') },
		{ KeyCode::KEY_X, std::pair<char, char>('x', 'X') },
		{ KeyCode::KEY_Y, std::pair<char, char>('y', 'Y') },
		{ KeyCode::KEY_Z, std::pair<char, char>('z', 'Z') },

		{ KeyCode::KEY_F1, std::pair<char, char>('\0', '\0') },
		{ KeyCode::KEY_F2, std::pair<char, char>('\0', '\0') },
		{ KeyCode::KEY_F3, std::pair<char, char>('\0', '\0') },
		{ KeyCode::KEY_F4, std::pair<char, char>('\0', '\0') },
		{ KeyCode::KEY_F5, std::pair<char, char>('\0', '\0') },
		{ KeyCode::KEY_F6, std::pair<char, char>('\0', '\0') },
		{ KeyCode::KEY_F7, std::pair<char, char>('\0', '\0') },
		{ KeyCode::KEY_F8, std::pair<char, char>('\0', '\0') },
		{ KeyCode::KEY_F9, std::pair<char, char>('\0', '\0') },
		{ KeyCode::KEY_F10, std::pair<char, char>('\0', '\0') },
		{ KeyCode::KEY_F11, std::pair<char, char>('\0', '\0') },
		{ KeyCode::KEY_F12, std::pair<char, char>('\0', '\0') },

		{ KeyCode::KEY_BACKSPACE,	std::pair<char, char>('\0', '\0') },
		{ KeyCode::KEY_SPACE,		std::pair<char, char>(' ', ' ')	  },
		{ KeyCode::KEY_BACKTICK,	std::pair<char, char>('`', '~')   },
		{ KeyCode::KEY_ENTER,		std::pair<char, char>('\n', '\n') },
		{ KeyCode::KEY_CAPSLOCK,	std::pair<char, char>('\0', '\0') },
		{ KeyCode::KEY_TAB,			std::pair<char, char>('\t', '\t') },
		{ KeyCode::KEY_LSHIFT,		std::pair<char, char>('\0', '\0') },
		{ KeyCode::KEY_RSHIFT,		std::pair<char, char>('\0', '\0') },
		{ KeyCode::KEY_LCONTROL,	std::pair<char, char>('\0', '\0') },
		{ KeyCode::KEY_RCONTROL,	std::pair<char, char>('\0', '\0') },
		{ KeyCode::KEY_ALT,			std::pair<char, char>('\0', '\0') },
		{ KeyCode::KEY_OPTION,		std::pair<char, char>('\0', '\0') },
		{ KeyCode::KEY_UP,			std::pair<char, char>('\0', '\0') },
		{ KeyCode::KEY_DOWN,		std::pair<char, char>('\0', '\0') },
		{ KeyCode::KEY_RIGHT,		std::pair<char, char>('\0', '\0') },
		{ KeyCode::KEY_LEFT,		std::pair<char, char>('\0', '\0') },

		{ KeyCode::KEY_MINUS,		std::pair<char, char>('-', '_')		},
		{ KeyCode::KEY_EQUALS,		std::pair<char, char>('=', '+')		},
		{ KeyCode::KEY_COMMA,		std::pair<char, char>(',', '<')		},
		{ KeyCode::KEY_PERIOD,		std::pair<char, char>('.', '>')		},
		{ KeyCode::KEY_SEMICOLON,	std::pair<char, char>(';', ':')	},
		{ KeyCode::KEY_TICK,		std::pair<char, char>('\'', '\"')	},
		{ KeyCode::KEY_SQUARE_BRACKET_RIGHT, std::pair<char, char>('[', '{') },
		{ KeyCode::KEY_SQUARE_BRACKET_LEFT,	 std::pair<char, char>(']', '}') },
		{ KeyCode::KEY_BACKSLASH,	std::pair<char, char>('\\', '|')	},
		{ KeyCode::KEY_SLASH,		std::pair<char, char>('/', '?')		},
	};

	KeyCode VkToMcKeycode(int vkkeycode)
	{
		for (auto it = _monochrome_keyboard_keycode_bindings_.begin(); it != _monochrome_keyboard_keycode_bindings_.end(); it++)
			if (it->second == vkkeycode)
				return it->first;

		return KeyCode::None;
	}

	int McToVkKeycode(KeyCode keycode)
	{
		return _monochrome_keyboard_keycode_bindings_[keycode];
	}

	char McKeycodeToChar(KeyCode keycode, bool capital, bool capslock_enabled)
	{
		if (keycode > KeyCode::KEY_9&& keycode < KeyCode::KEY_F1 && (capital || capslock_enabled))
			return _monochrome_keyboard_keycode_capital_char_bindings_[keycode].second;

		if (capital)
			return _monochrome_keyboard_keycode_capital_char_bindings_[keycode].second;
		else
			return _monochrome_keyboard_keycode_capital_char_bindings_[keycode].first;
	}

	std::string KeyPressedEvent::ToString() const
	{
		std::stringstream ss;
		ss << "Key Pressed: " << McKeycodeToChar(keycode, capital, capslock_on);
		return ss.str();
	}

	std::string KeyReleasedEvent::ToString() const
	{
		std::stringstream ss;
		ss << "Key Released: " << McKeycodeToChar(keycode, false, false);
		return ss.str();
	}
}
