#include <iostream>
#include <platform/window/Win32Window.h>

int main()
{
    auto window = mc::MCWindow::Create(1020, 560, "Cross-Platform Monochrome");

    system("pause");
    return 0;
}
