#pragma once
#include <application/ApplicationContext.h>
#include <Windows.h>
#include <vector>
#include <atomic>

namespace mc
{
    class Win32NativeWindow;

    class Win32ApplicationContext : public ApplicationContext
    {
    public:
        Win32ApplicationContext(const std::string& appId);

        void startApplicationLoop();

        void mainWindowRequestedClose();
        void registerWin32NativeWindowHandle(Win32NativeWindow* handle);

    private:
        std::vector<Win32NativeWindow*> d_win32NativeWindowHandles;
        std::atomic_bool d_applicationRunning = false;
    };
}