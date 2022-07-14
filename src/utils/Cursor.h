#pragma once
#include <core/Core.h>

namespace mc
{
    enum CursorType
    {
        Unknown,
        AppStarting,
        Arrow,
        Cross,
        Hand,
        Help,
        IBeam,
        No,
        SizeAll,
        SizeNESW,
        SizeNS,
        SizeNWSE,
        SizeWE,
        UpArrow,
        Wait
    };

#define DEFAULT_CURSOR_TYPE CursorType::Arrow

    namespace utils
    {
        class Cursor
        {
        public:
            // Sets the active cursor icon
            static void setActiveCursor(CursorType type);

            // Returns a pointer to the platform-dependent cursor data
            static const void* getActiveCursor();
        };
    }
}
