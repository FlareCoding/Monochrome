#include "Win32ApplicationContext.h"
#include "Win32NativeWindow.h"

namespace mc
{
	Win32ApplicationContext::Win32ApplicationContext(const std::string& appId)
		: ApplicationContext(appId) {}

	void Win32ApplicationContext::mainWindowRequestedClose() {
		d_applicationRunning = false;
	}

	void Win32ApplicationContext::registerWin32NativeWindowHandle(Win32NativeWindow* handle) {
		d_win32NativeWindowHandles.push_back(handle);
	}

	void Win32ApplicationContext::startApplicationLoop() {
		// Check if any of the window's are opened
		if (!d_win32NativeWindowHandles.size()) {
			return;
		}

		// Set the application running flag to true
		d_applicationRunning = true;

		// Start the application loop
		while (d_applicationRunning) {
			for (auto& window : d_win32NativeWindowHandles) {
				window->updatePlatformWindow();
			}
		}

		// Destroy all windows
		for (auto& window : d_win32NativeWindowHandles) {
			window->destroyPlatformWindow();
		}
	}
}
