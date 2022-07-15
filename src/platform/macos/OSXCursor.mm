#include <utils/Cursor.h>
#include <Cocoa/Cocoa.h>

namespace mc::utils
{
    NSCursor* s_activeOSXCursorIcon = [NSCursor arrowCursor];

    void Cursor::setActiveCursor(CursorType type) {
        switch (type) {
        case CursorType::AppStarting: {
            s_activeOSXCursorIcon = [NSCursor arrowCursor];
            break;
        }
        case CursorType::Arrow: {
            s_activeOSXCursorIcon = [NSCursor arrowCursor];
            break;
        }
        case CursorType::Cross: {
            s_activeOSXCursorIcon = [NSCursor crosshairCursor];
            break;
        }
        case CursorType::Hand: {
            s_activeOSXCursorIcon = [NSCursor pointingHandCursor];
            break;
        }
        case CursorType::IBeam: {
            s_activeOSXCursorIcon = [NSCursor IBeamCursor];
            break;
        }
        case CursorType::No: {
            s_activeOSXCursorIcon = [NSCursor operationNotAllowedCursor];
            break;
        }
        case CursorType::SizeAll: {
            s_activeOSXCursorIcon = [NSCursor resizeLeftRightCursor];
            break;
        }
        case CursorType::SizeNESW: {
            s_activeOSXCursorIcon = [NSCursor resizeUpDownCursor];
            break;
        }
        case CursorType::SizeNS: {
            s_activeOSXCursorIcon = [NSCursor resizeUpDownCursor];
            break;
        }
        case CursorType::SizeNWSE: {
            s_activeOSXCursorIcon = [NSCursor resizeUpDownCursor];
            break;
        }
        case CursorType::SizeWE: {
            s_activeOSXCursorIcon = [NSCursor resizeLeftRightCursor];
            break;
        }
        case CursorType::UpArrow: {
            s_activeOSXCursorIcon = [NSCursor resizeUpCursor];
            break;
        }
        default: {
            s_activeOSXCursorIcon = [NSCursor arrowCursor];
            break;
        }
        }

        [s_activeOSXCursorIcon set];
    }

    const void* Cursor::getActiveCursor() {
        return static_cast<const void*>(s_activeOSXCursorIcon);
    }
}
