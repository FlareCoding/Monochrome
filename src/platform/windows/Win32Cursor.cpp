#include <utils/Cursor.h>
#include <Windows.h>

namespace mc::utils {
    static LPCWSTR s_activeWin32CursorIcon = IDC_ARROW;

    void Cursor::setActiveCursor(CursorType type) {
        switch (type) {
        case CursorType::AppStarting: {
            s_activeWin32CursorIcon = IDC_APPSTARTING;
            break;
        }
        case CursorType::Arrow: {
            s_activeWin32CursorIcon = IDC_ARROW;
            break;
        }
        case CursorType::Cross: {
            s_activeWin32CursorIcon = IDC_CROSS;
            break;
        }
        case CursorType::Hand: {
            s_activeWin32CursorIcon = IDC_HAND;
            break;
        }
        case CursorType::Help: {
            s_activeWin32CursorIcon = IDC_HELP;
            break;
        }
        case CursorType::IBeam: {
            s_activeWin32CursorIcon = IDC_IBEAM;
            break;
        }
        case CursorType::No: {
            s_activeWin32CursorIcon = IDC_NO;
            break;
        }
        case CursorType::SizeAll: {
            s_activeWin32CursorIcon = IDC_SIZEALL;
            break;
        }
        case CursorType::SizeNESW: {
            s_activeWin32CursorIcon = IDC_SIZENESW;
            break;
        }
        case CursorType::SizeNS: {
            s_activeWin32CursorIcon = IDC_SIZENS;
            break;
        }
        case CursorType::SizeNWSE: {
            s_activeWin32CursorIcon = IDC_SIZENWSE;
            break;
        }
        case CursorType::SizeWE: {
            s_activeWin32CursorIcon = IDC_SIZEWE;
            break;
        }
        case CursorType::UpArrow: {
            s_activeWin32CursorIcon = IDC_UPARROW;
            break;
        }
        case CursorType::Wait: {
            s_activeWin32CursorIcon = IDC_WAIT;
            break;
        }
        default: break;
        }
    }

    const void* Cursor::getActiveCursor() {
        return static_cast<const void*>(s_activeWin32CursorIcon);
    }
} // namespace mc::utils
