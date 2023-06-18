#include "AppManager.h"
#include "widgets/Color.h"

namespace mc {
    static Shared<ApplicationContext> s_applicationContext = nullptr;

    std::string AppManager::s_appId;

    void AppManager::registerApplication(const std::string& appId) {
        s_appId = appId;

        // Initialize resources and perform necessary setup
        registerMcxPrimitiveColorNames();
    }

    Shared<ApplicationContext> AppManager::getApplicationContext() {
        CORE_ASSERT(
            !s_appId.empty(),
            "Current application is not registered, please"
            " call AppManager::registerApplication() first"
        );

        if (!s_applicationContext) {
            s_applicationContext = ApplicationContext::create(s_appId);
        }

        return s_applicationContext;
    }

    void AppManager::startApplicationLoop() {
        auto appContext = getApplicationContext();

        // Assert that all necessary runtime
        // functions are available and valid.
        auto& runtimeFunctions = appContext->getRuntimeUtilityFunctions();

        CORE_ASSERT(
            runtimeFunctions.getTextPixelSizeFn,
            "Runtime function 'getTextPixelSizeFn' is not implemented"
        );

        appContext->startApplicationLoop();
    }

    void AppManager::executeOnUiThread(std::function<void()> fn) {
        const auto context = getApplicationContext();
        context->dispatchActionToUiThread(fn);
    }
} // namespace mc
