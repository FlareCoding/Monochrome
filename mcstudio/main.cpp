//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#include "Studio.h"

int main()
{
    UniqueRef<MonochromeStudio> studio = MakeUniqueRef<MonochromeStudio>();
    studio->Initialize();
    studio->Start();

    return 0;
}
