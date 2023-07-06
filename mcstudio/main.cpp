#if defined(NDEBUG) && defined(_WIN32)
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif // !NDEBUG

#include "GlobalResgitrationSystem.h"
#include "McxSourceManager.h"
#include "Editor.h"

int main() {
    mc::AppManager::registerApplication(MCSTUDIO_APPLICATION_ID);

    mc::mcstudio::GlobalRegistrationSystem::registerMcStudioColorNames();
    mc::mcstudio::McxSourceManager::initialize();
    mc::mcstudio::McxSourceManager::loadStylesheet();

    auto editor = mc::MakeRef<mc::mcstudio::Editor>();

    auto window = mc::mcstudio::McxSourceManager::loadMainWindow();
    
    auto canvas = window->createOverlayCanvas();
    editor->setWindowCanvasReference(canvas);

    mc::AppManager::startApplicationLoop();
    return 0;
}
