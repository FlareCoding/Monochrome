#pragma once

// Application ID
#define MCSTUDIO_APPLICATION_ID "appId-mcstudio"

// Custom color names
#define MCSTUDIO_COLOR_TRANSPARENT  "transparent"
#define MCSTUDIO_BG_COLOR_PRIMARY   "mcstudioBgColorPrimary"

// Event names
// ...

namespace mc::mcstudio {
class GlobalRegistrationSystem {
public:
    // Registers every custom color name used in the monochrome studio design
    static void registerMcStudioColorNames();
};
} // namespace mc::mcstudio
