#include "WidgetPropertiesPanelController.h"

namespace mc::mcstudio {
    WidgetPropertiesPanelController::WidgetPropertiesPanelController(
        Shared<mcx::BaseWidgetMcxAdapter> baseWidgetAdapter
    ) {
        d_baseWidgetAdapter = baseWidgetAdapter;
        d_propertiesPanel = getWidgetById<StackPanel>("propertiesListPanel");

        appendAllowedEvent("widgetPropertyModified");
    }

    void WidgetPropertiesPanelController::clear() {
        d_propertiesPanel->removeAllChildren();
    }

    void WidgetPropertiesPanelController::displayWidgetProperties(
        Shared<mcx::McxNode> widgetNode,
        Shared<mcx::McxParsingAdapter> widgetAdapter
    ) {
        // Get the reference to the widget properties
        // panel and clear it from all previous entries.
        auto propertiesListPanel = getWidgetById<StackPanel>("propertiesListPanel");
        propertiesListPanel->removeAllChildren();

        // Add records of all basic properties
        for (auto& prop : d_baseWidgetAdapter->getAvailableProperties()) {
            /*auto propertyEntry = _createPropertyEntry(prop, true, widget, mcxNode, baseAdapter, customAdapter);*/
            auto propValue = widgetNode->getAttribute(prop);

            auto propertyField = _createRegularPropertyTextField(prop, propValue, true);
            propertiesListPanel->addChildOffline(propertyField);
        }

        // Empty space acting as a divider
        auto dividingSpaceLabel = MakeRef<Label>();
        dividingSpaceLabel->text = " ";
        propertiesListPanel->addChildOffline(dividingSpaceLabel);

        // Add records of all custom widget properties
        for (auto& prop : widgetAdapter->getAvailableProperties()) {
            //auto propertyEntry = _createPropertyEntry(prop, false, widget, mcxNode, baseAdapter, customAdapter);
            auto propValue = widgetNode->getAttribute(prop);

            auto propertyField = _createRegularPropertyTextField(prop, propValue, false);
            propertiesListPanel->addChildOffline(propertyField);
        }

        // Since all the children have been added in the offline mode, a.k.a no
        // layout change events firing, the layout needs to be re-calculated.
        propertiesListPanel->signalLayoutChanged();
    }
    
    Shared<BaseWidget> WidgetPropertiesPanelController::_createRegularPropertyTextField(
        const std::string& propName,
        const std::string& propValue,
        bool isBasicProperty
    ) {
        auto container = MakeRef<StackPanel>();
        container->orientation = Horizontal;
        container->backgroundColor = Color::transparent;
        container->marginBottom = 4;

        auto propertyNameLabel = MakeRef<Label>();
        propertyNameLabel->text = propName;
        propertyNameLabel->fixedWidth = 110;
        propertyNameLabel->fixedHeight = 20;
        propertyNameLabel->marginLeft = 10;
        propertyNameLabel->alignment = "left";
        container->addChildOffline(propertyNameLabel);

        auto entry = MakeRef<Entry>();
        entry->backgroundColor = Color::transparent;
        entry->borderColor = Color::transparent;
        entry->textColor = Color::white;
        entry->cursorColor = Color::white;
        entry->focusedBorderColor = Color(0, 240, 216);
        entry->text = propValue;
        entry->fixedWidth = 150;
        entry->fixedHeight = 20;
        entry->on("entered", [this, isBasicProperty](Shared<Event> e) {
            auto target = static_cast<Entry*>(e->target);
            auto parentContainer = static_cast<StackPanel*>(target->getParent());
            auto propertyNameLabel = std::static_pointer_cast<Label>(parentContainer->getChild(0));

            auto propName = propertyNameLabel->text.get();
            auto propValue = target->text.get();
            
            fireEvent("widgetPropertyModified", {
                { "propName", propName },
                { "propValue", propValue },
                { "isBasicProp", isBasicProperty },
            });
        });
        container->addChildOffline(entry);

        return container;
    }
} // namespace mc::mcstudio