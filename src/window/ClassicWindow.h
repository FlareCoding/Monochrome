#pragma once
#include "UIWindow.h"

namespace mc
{
    class ClassicWindow : public UIWindow
    {
    public:
        ClassicWindow(uint32_t width = 800, uint32_t height = 600, const std::string& title = "Window");
    };
}
