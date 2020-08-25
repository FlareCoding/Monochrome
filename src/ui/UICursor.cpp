#include "UICursor.h"
#include <unordered_map>

#if defined(_WIN32)
	#include <Windows.h>
	static void _set_cursor(LPCWSTR cursor)
	{
		SetCursor(LoadCursorW(NULL, cursor));
	}
#endif

namespace mc
{
#if defined(_WIN32)
	static std::unordered_map<CursorType, LPCWSTR> CursorNameMappings =
	{
		{ CursorType::AppStarting,	IDC_APPSTARTING },
		{ CursorType::Arrow,		IDC_ARROW },
		{ CursorType::Cross,		IDC_CROSS },
		{ CursorType::Hand,			IDC_HAND },
		{ CursorType::Help,			IDC_HELP },
		{ CursorType::IBeam,		IDC_IBEAM },
		{ CursorType::No,			IDC_NO },
		{ CursorType::SizeAll,		IDC_SIZEALL },
		{ CursorType::SizeNESW,		IDC_SIZENESW },
		{ CursorType::SizeNS,		IDC_SIZENS },
		{ CursorType::SizeNWE,		IDC_SIZENWSE },
		{ CursorType::SizeWE,		IDC_SIZEWE },
		{ CursorType::UpArrow,		IDC_UPARROW },
		{ CursorType::Wait,			IDC_WAIT },
	};
#endif

	static CursorType s_ActiveCursorType = CursorType::Arrow;

	void UICursor::ActivateCursor(CursorType type)
	{
#if defined(_WIN32)
		if (type != CursorType::Unknown)
			_set_cursor(CursorNameMappings[type]);
#endif
	}

	void UICursor::SetCursor(CursorType type)
	{
		s_ActiveCursorType = type;
	}

	CursorType UICursor::GetActiveCursor()
	{
		return s_ActiveCursorType;
	}
}
