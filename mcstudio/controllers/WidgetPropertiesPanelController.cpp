#include "WidgetPropertiesPanelController.h"

namespace mc::mcstudio {
    WidgetPropertiesPanelController::WidgetPropertiesPanelController(
        Shared<mcx::BaseWidgetMcxAdapter> baseWidgetAdapter
    ) {
        appendAllowedEvent("widgetPropertyModified");

        d_propertiesPanel = getWidgetById<StackPanel>("propertiesListPanel");

        d_basePropertiesPanel = MakeRef<BaseWidgetPropertiesPanel>(baseWidgetAdapter);
        d_basePropertiesPanel->forwardEmittedEvent(this, "widgetPropertyModified");
    }

    void WidgetPropertiesPanelController::clear() {
        d_propertiesPanel->removeAllChildren();
    }

    void WidgetPropertiesPanelController::displayWidgetProperties(
        Shared<mcx::McxNode> widgetNode,
        Shared<mcx::McxParsingAdapter> widgetAdapter
    ) {
        d_propertiesPanel->removeAllChildren();

        // Update the basic properties fields to apply to the the given widget
        d_basePropertiesPanel->update(widgetNode, widgetAdapter);

        // Add fields for all basic properties
        d_propertiesPanel->addChildOffline(d_basePropertiesPanel);

        //// Add records of all custom widget properties
        //for (auto& prop : widgetAdapter->getAvailableProperties()) {
        //    auto propValue = widgetNode->getAttribute(prop);

        //    Shared<BaseWidget> propertyField;
        //    if (isPropBoolean(propValue)) {
        //        propertyField = _createBooleanPropertyField(prop, propValue, false);
        //    } else {
        //        propertyField = _createRegularPropertyTextField(prop, propValue, false);
        //    }

        //    d_propertiesPanel->addChildOffline(propertyField);
        //}

        // Since all the children have been added in the offline mode, a.k.a no
        // layout change events firing, the layout needs to be re-calculated.
        d_propertiesPanel->signalLayoutChanged();
    }
} // namespace mc::mcstudio
