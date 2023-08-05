#include "TextVisual.h"
#include <application/AppManager.h>

namespace mc {
    Size TextVisual::measureSize() const {
        // Get runtime utility functions
        auto& runtimeFunctions =
            AppManager::getApplicationContext()->getRuntimeUtilityFunctions();

        // Retrieve the function used to calculate
        // minimum required size for the text in pixels.
        auto getTextPixelSizeFn = runtimeFunctions.getTextPixelSizeFn;

        // If there is not size limit, word wrap shouldn't
        // affect the size of the desired text block size.
        uint32_t sizeLimit = widthLimit.get();
        std::string computedWrapMode = sizeLimit ? wordWrapMode.get() : "none";

        // Determine the minimum required width for the text in pixels
        auto [textPixelWidth, textPixelHeight] = getTextPixelSizeFn(
            sizeLimit ? sizeLimit : std::numeric_limits<uint64_t>::max(),
            std::numeric_limits<uint64_t>::max(),
            text, font, fontSize, fontStyle,
            "left", computedWrapMode
        );

        return {
            static_cast<uint32_t>(textPixelWidth),
            static_cast<uint32_t>(textPixelHeight)
        };
    }
} // namespace mc
