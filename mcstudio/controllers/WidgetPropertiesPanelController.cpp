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

        // Add fields for all basic properties
        d_basePropertiesPanel->update(widgetNode, widgetAdapter);
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
    
    Shared<BaseWidget> WidgetPropertiesPanelController::_createDockAnchorPropertyField(
        const std::string& propValue
    ) {
        static const auto unselectedDockButtonColor = Color(70, 70, 70);
        static const auto selectedDockButtonColor = Color(130, 130, 130);

        auto container = MakeRef<StackPanel>();
        container->backgroundColor = Color::transparent;
        container->marginBottom = 4;
        container->marginLeft = 10;

        auto propertyNameLabel = MakeRef<Label>();
        propertyNameLabel->text = "Dock Anchor";
        propertyNameLabel->fixedWidth = 260;
        propertyNameLabel->fixedHeight = 10;
        propertyNameLabel->marginBottom = 6;
        container->addChildOffline(propertyNameLabel);

        auto dockAnchorTopButton = MakeRef<Button>();
        dockAnchorTopButton->borderColor = Color(10, 10, 10);
        dockAnchorTopButton->borderThickness = 1;
        dockAnchorTopButton->cornerRadius = 0;
        dockAnchorTopButton->label->color = Color::white;
        dockAnchorTopButton->label->text = "";
        dockAnchorTopButton->fixedHeight = 16;
        dockAnchorTopButton->dockAnchor = DockAnchor::Top;

        auto dockAnchorBottomButton = MakeRef<Button>();
        dockAnchorBottomButton->borderColor = Color(10, 10, 10);
        dockAnchorBottomButton->borderThickness = 1;
        dockAnchorBottomButton->cornerRadius = 0;
        dockAnchorBottomButton->label->color = Color::white;
        dockAnchorBottomButton->label->text = "";
        dockAnchorBottomButton->fixedHeight = 16;
        dockAnchorBottomButton->dockAnchor = DockAnchor::Bottom;

        auto dockAnchorLeftButton = MakeRef<Button>();
        dockAnchorLeftButton->borderColor = Color(10, 10, 10);
        dockAnchorLeftButton->borderThickness = 1;
        dockAnchorLeftButton->cornerRadius = 0;
        dockAnchorLeftButton->label->color = Color::white;
        dockAnchorLeftButton->label->text = "";
        dockAnchorLeftButton->fixedWidth = 20;
        dockAnchorLeftButton->dockAnchor = DockAnchor::Left;

        auto dockAnchorRightButton = MakeRef<Button>();
        dockAnchorRightButton->borderColor = Color(10, 10, 10);
        dockAnchorRightButton->borderThickness = 1;
        dockAnchorRightButton->cornerRadius = 0;
        dockAnchorRightButton->label->color = Color::white;
        dockAnchorRightButton->label->text = "";
        dockAnchorRightButton->fixedWidth = 20;
        dockAnchorRightButton->dockAnchor = DockAnchor::Right;

        auto dockAnchorFillButton = MakeRef<Button>();
        dockAnchorFillButton->borderColor = Color(10, 10, 10);
        dockAnchorFillButton->borderThickness = 1;
        dockAnchorFillButton->cornerRadius = 0;
        dockAnchorFillButton->label->color = Color::white;
        dockAnchorFillButton->label->text = "";
        dockAnchorFillButton->dockAnchor = DockAnchor::Fill;

        auto controlDockPanel = MakeRef<DockPanel>();
        controlDockPanel->horizontalAlignment = HACenter;
        controlDockPanel->backgroundColor = Color::red;
        controlDockPanel->fixedWidth = 160;
        controlDockPanel->fixedHeight = 80;
        controlDockPanel->backgroundColor = Color::transparent;
        controlDockPanel->addChildOffline(dockAnchorTopButton);
        controlDockPanel->addChildOffline(dockAnchorBottomButton);
        controlDockPanel->addChildOffline(dockAnchorLeftButton);
        controlDockPanel->addChildOffline(dockAnchorRightButton);
        controlDockPanel->addChildOffline(dockAnchorFillButton);
        container->addChildOffline(controlDockPanel);

        // Apply appropriate coloring to the control buttons
        if (propValue == "left") {
            dockAnchorLeftButton->backgroundColor = selectedDockButtonColor;

            dockAnchorTopButton->backgroundColor = unselectedDockButtonColor;
            dockAnchorRightButton->backgroundColor = unselectedDockButtonColor;
            dockAnchorBottomButton->backgroundColor = unselectedDockButtonColor;
            dockAnchorFillButton->backgroundColor = unselectedDockButtonColor;
        } else if (propValue == "top") {
            dockAnchorTopButton->backgroundColor = selectedDockButtonColor;

            dockAnchorLeftButton->backgroundColor = unselectedDockButtonColor;
            dockAnchorRightButton->backgroundColor = unselectedDockButtonColor;
            dockAnchorBottomButton->backgroundColor = unselectedDockButtonColor;
            dockAnchorFillButton->backgroundColor = unselectedDockButtonColor;
        } else if (propValue == "right") {
            dockAnchorRightButton->backgroundColor = selectedDockButtonColor;

            dockAnchorTopButton->backgroundColor = unselectedDockButtonColor;
            dockAnchorLeftButton->backgroundColor = unselectedDockButtonColor;
            dockAnchorBottomButton->backgroundColor = unselectedDockButtonColor;
            dockAnchorFillButton->backgroundColor = unselectedDockButtonColor;
        } else if (propValue == "bottom") {
            dockAnchorBottomButton->backgroundColor = selectedDockButtonColor;

            dockAnchorTopButton->backgroundColor = unselectedDockButtonColor;
            dockAnchorLeftButton->backgroundColor = unselectedDockButtonColor;
            dockAnchorRightButton->backgroundColor = unselectedDockButtonColor;
            dockAnchorFillButton->backgroundColor = unselectedDockButtonColor;
        } else {
            dockAnchorFillButton->backgroundColor = selectedDockButtonColor;

            dockAnchorTopButton->backgroundColor = unselectedDockButtonColor;
            dockAnchorBottomButton->backgroundColor = unselectedDockButtonColor;
            dockAnchorLeftButton->backgroundColor = unselectedDockButtonColor;
            dockAnchorRightButton->backgroundColor = unselectedDockButtonColor;
        }

        return container;
    }
} // namespace mc::mcstudio
