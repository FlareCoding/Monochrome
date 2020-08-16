#include "UICursor.h"
#include <Windows.h>
#include <unordered_map>

static void _set_cursor(LPCWSTR cursor)
{
	SetCursor(LoadCursorW(NULL, cursor));
}

namespace mc
{
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

	static CursorType s_ActiveCursorType = CursorType::Arrow;

	void UICursor::ActivateCursor(CursorType type)
	{
		if (type != CursorType::Unknown)
			_set_cursor(CursorNameMappings[type]);
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
