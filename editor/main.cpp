#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#include "Editor.h"

int main()
{
    UniqueRef<MonochromeEditor> editor = MakeUniqueRef<MonochromeEditor>();
    editor->Initialize();
    editor->Start();

    return 0;
}
