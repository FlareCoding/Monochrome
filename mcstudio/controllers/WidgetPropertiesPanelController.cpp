#include "WidgetPropertiesPanelController.h"

namespace mc::mcstudio {
    WidgetPropertiesPanelController::WidgetPropertiesPanelController() {
        d_propertiesPanel = getWidgetById<StackPanel>("propertiesListPanel");
    }
} // namespace mc::mcstudio
