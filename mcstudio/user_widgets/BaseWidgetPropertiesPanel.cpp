#include "BaseWidgetPropertiesPanel.h"
#include "RegularPropertyTextField.h"
#include "BooleanPropertyField.h"
#include "DockAnchorPropertyField.h"

namespace mc::mcstudio {
    static bool isPropBoolean(std::string& value) {
        return (value == "true" || value == "false");
    }

    BaseWidgetPropertiesPanel::BaseWidgetPropertiesPanel(
        Shared<mcx::BaseWidgetMcxAdapter> adapter
    ) : StackPanel(), d_basePropAdapter(adapter) {
        // Initialize properties
        backgroundColor = Color::transparent;
        horizontalAlignment = HAFill;
        marginBottom = 20;

        appendAllowedEvent("widgetPropertyModified");
    }

    void BaseWidgetPropertiesPanel::update(
        Shared<mcx::McxNode> widgetNode,
        Shared<mcx::McxParsingAdapter> widgetAdapter
    ) {
        removeAllChildren();

        for (auto& prop : d_basePropAdapter->getAvailableProperties()) {
            auto propValue = widgetNode->getAttribute(prop);

            Shared<BaseWidget> propertyField;
            if (isPropBoolean(propValue)) {
                propertyField = MakeRef<BooleanPropertyField>(prop, propValue);
            
            } else if (prop == "dockAnchor") {
                propertyField = MakeRef<DockAnchorPropertyField>(prop, propValue);

            } else {
                propertyField = MakeRef<RegularPropertyTextField>(prop, propValue);
            }

            propertyField->on("widgetPropertyModified", &BaseWidgetPropertiesPanel::_onWidgetPropertyModified, this);
            addChildOffline(propertyField);
        }
    }

    void BaseWidgetPropertiesPanel::_onWidgetPropertyModified(Shared<Event> e) {
        e->store("isBasicProp", true);

        // Forward the event with additional data
        fireEvent("widgetPropertyModified", e);
    }
} // namespace mc::mcstudio
