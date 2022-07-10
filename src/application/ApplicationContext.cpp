#include "ApplicationContext.h"

#ifdef MC_PLATFORM_WINDOWS_X64
    #include <platform/windows/Win32ApplicationContext.h>
#elif defined(MC_PLATFORM_MACOS)
    #include <platform/macos/OSXApplicationContext.h>
#endif

namespace mc
{
    Shared<ApplicationContext> ApplicationContext::create(const std::string& appId) {
#ifdef MC_PLATFORM_WINDOWS_X64
        return Shared<Win32ApplicationContext>(new Win32ApplicationContext(appId));
#elif defined(MC_PLATFORM_MACOS)
        return Shared<OSXApplicationContext>(new OSXApplicationContext(appId));
#endif
    }

    ApplicationContext::ApplicationContext(const std::string& appId) : d_appId(appId) {}
}
