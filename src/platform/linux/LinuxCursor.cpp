#include <utils/Cursor.h>

namespace mc::utils {
    void Cursor::setActiveCursor(CursorType type) {
        switch (type) {
        case CursorType::AppStarting: {
            break;
        }
        case CursorType::Arrow: {
            break;
        }
        case CursorType::Cross: {
            break;
        }
        case CursorType::Hand: {
            break;
        }
        case CursorType::Help: {
            break;
        }
        case CursorType::IBeam: {
            break;
        }
        case CursorType::No: {
            break;
        }
        case CursorType::SizeAll: {
            break;
        }
        case CursorType::SizeNESW: {
            break;
        }
        case CursorType::SizeNS: {
            break;
        }
        case CursorType::SizeNWSE: {
            break;
        }
        case CursorType::SizeWE: {
            break;
        }
        case CursorType::UpArrow: {
            break;
        }
        case CursorType::Wait: {
            break;
        }
        default: break;
        }
    }

    const void* Cursor::getActiveCursor() {
        return nullptr;
    }
} // namespace mc::utils
