#if defined(NDEBUG) && defined(_WIN32)
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif // !NDEBUG

#include "GlobalResgitrationSystem.h"
#include "McxSourceManager.h"
#include "Editor.h"

#include <core/FunctionProfiler.h>

int main() {
    mc::AppManager::registerApplication(MCSTUDIO_APPLICATION_ID);

    mc::mcstudio::GlobalRegistrationSystem::registerMcStudioColorNames();
    mc::mcstudio::McxSourceManager::initialize();
    mc::mcstudio::McxSourceManager::loadStylesheet();

    auto editor = mc::MakeRef<mc::mcstudio::Editor>();

    auto window = mc::mcstudio::McxSourceManager::loadMainWindow();
    
    auto canvas = window->createOverlayCanvas();
    editor->setWindowCanvasReference(canvas);

    {
        PROFILE_FUNCTION("main");

        volatile int i = 0;
        for (int i = 0; i < 100000; i++) {
            volatile double x = sqrt(64);
        }
    }

    auto& record = mc::debug::GlobalFunctionProfilerRegistry::get().getProfilerSessionRecord("main");
    printf("%s:\n  %llu us\n\n", record.functionName.c_str(), record.durationInMicroseconds);

    mc::AppManager::startApplicationLoop();
    return 0;
}
