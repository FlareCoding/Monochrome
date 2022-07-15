#pragma once
#include <core/Core.h>
#include <string>
#include <functional>

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

protected:
    explicit ApplicationContext(const std::string& appId);

    std::string d_appId;
    RuntimeUtilityFunctions d_runtimeUtilityFunctions;
};
} // namespace mc
