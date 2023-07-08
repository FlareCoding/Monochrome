#pragma once
#include <widgets/AllWidgets.h>

namespace mc::mcstudio {
class WidgetPropertiesPanelController {
public:
    WidgetPropertiesPanelController();

private:
    Shared<StackPanel> d_propertiesPanel;
};
} // namespace mc::mcstudio
