#pragma once

namespace mc
{
    enum class CursorType
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
        SizeNWE,
        SizeWE,
        UpArrow,
        Wait
    };

    class UICursor
    {
    public:
        /// Changes the current cursor icon.
        static void ActivateCursor(CursorType type);

        /// Sets the active cursor type that gets applied on the next WM_SETCURSOR call.
        static void SetCursor(CursorType type);

        /// Selects the latest activated cursor type.
        static CursorType GetActiveCursor();
    };
}
