#pragma once
#include "ApplicationContext.h"

namespace mc
{
    class AppManager
    {
    public:
        static void registerApplication(const std::string& appId);
        static Shared<ApplicationContext> getApplicationContext();

        static void startApplicationLoop();

    private:
        static std::string s_appId;
    };
}
