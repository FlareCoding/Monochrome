#include "GlobalResgitrationSystem.h"
#include <mcx/McxEngine.h>

namespace mc::mcstudio {
void GlobalRegistrationSystem::registerMcStudioColorNames() {
    registerCustomColor(MCSTUDIO_COLOR_TRANSPARENT, Color::transparent);
    registerCustomColor(MCSTUDIO_BG_COLOR_PRIMARY,  Color(34, 33, 38));
}
} // namespace mc::mcstudio
