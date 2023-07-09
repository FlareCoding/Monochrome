#pragma once
#include <widgets/AllWidgets.h>
#include <mcx/adapters/BaseWidgetMcxAdapter.h>

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
    WidgetPropertiesPanelController(Shared<mcx::BaseWidgetMcxAdapter> baseWidgetAdapter);

    // Removes all property entries from the widget properties panel
    void clear();

    // Fills the properties panel with the provided widget's properties
    void displayWidgetProperties(
        Shared<mcx::McxNode> widgetNode,
        Shared<mcx::McxParsingAdapter> widgetAdapter
    );

private:
    Shared<StackPanel> d_propertiesPanel;
    Shared<mcx::BaseWidgetMcxAdapter> d_baseWidgetAdapter;

    // Creates a widget container consisting of a label and an entry
    // which upon modification will modify the selected widget's property.
    Shared<BaseWidget> _createRegularPropertyTextField(
        const std::string& propName,
        const std::string& propValue,
        bool isBasicProperty
    );
};
} // namespace mc::mcstudio
