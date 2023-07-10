#pragma once
#include "../user_widgets/BaseWidgetPropertiesPanel.h"

namespace mc::mcstudio {
class WidgetPropertyModifiedEvent : public Event {
public:
    inline std::string getPropName() {
        return get<std::string>("propName");
    }

    inline std::string getPropValue() {
        return get<std::string>("propValue");
    }

    inline bool isBasicProperty() {
        return get<bool>("isBasicProp");
    }
};

class WidgetPropertiesPanelController : public EventEmitter {
public:
    explicit WidgetPropertiesPanelController(
        Shared<mcx::BaseWidgetMcxAdapter> baseWidgetAdapter
    );

    // Removes all property entries from the widget properties panel
    void clear();

    // Fills the properties panel with the provided widget's properties
    void displayWidgetProperties(
        Shared<mcx::McxNode> widgetNode,
        Shared<mcx::McxParsingAdapter> widgetAdapter
    );

private:
    Shared<StackPanel> d_propertiesPanel;
    Shared<BaseWidgetPropertiesPanel> d_basePropertiesPanel;
};
} // namespace mc::mcstudio
