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
        /// Sets the current cursor icon.
        static void SetCursor(CursorType type);
    };
}
