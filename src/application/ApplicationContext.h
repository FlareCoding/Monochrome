#pragma once
#include <core/Core.h>
#include <string>
#include <functional>
#include <mutex>
#include <vector>

namespace mc {
struct RuntimeUtilityFunctions {
    std::function<
        std::pair<float, float>(
            uint64_t maxWidth,
            uint64_t maxHeight,
            const std::string& text,
            const std::string& font,
            uint32_t fontSize,
            const std::string& fontStyle,
            const std::string& alignment,
            const std::string& wrapMode
            )
    > getTextPixelSizeFn = nullptr;
};

class ApplicationContext {
public:
    static Shared<ApplicationContext> create(const std::string& appId);

    inline const std::string& getAppId() const { return d_appId; }
    inline RuntimeUtilityFunctions& getRuntimeUtilityFunctions() {
        return d_runtimeUtilityFunctions;
    }

    virtual void startApplicationLoop() = 0;

    void dispatchActionToUiThread(std::function<void()> fn);

protected:
    explicit ApplicationContext(const std::string& appId);

    std::string d_appId;
    RuntimeUtilityFunctions d_runtimeUtilityFunctions;

    // Runtime buffer for functions that should be
    // dispatched and executed on the main UI thread.
    std::vector<std::function<void()>> d_uiThreadActionQueue;

    std::mutex d_uiThreadMutex;

    // Processes and clears the queue with
    // actions that depend on the main UI thread.
    void _processUiThreadActions();
};
} // namespace mc
