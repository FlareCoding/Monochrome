#include "WidgetPropertiesPanelController.h"

namespace mc::mcstudio {
    static bool isPropBoolean(std::string& value) {
        return (value == "true" || value == "false");
    }

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
            auto propValue = widgetNode->getAttribute(prop);

            Shared<BaseWidget> propertyField;
            if (isPropBoolean(propValue)) {
                propertyField = _createBooleanPropertyField(prop, propValue, true);
            } else if (prop == "dockAnchor") {
                propertyField = _createDockAnchorPropertyField(propValue);
            } else {
                propertyField = _createRegularPropertyTextField(prop, propValue, true);
            }

            propertiesListPanel->addChildOffline(propertyField);
        }

        // Empty space acting as a divider
        auto dividingSpaceLabel = MakeRef<Label>();
        dividingSpaceLabel->text = " ";
        propertiesListPanel->addChildOffline(dividingSpaceLabel);

        // Add records of all custom widget properties
        for (auto& prop : widgetAdapter->getAvailableProperties()) {
            auto propValue = widgetNode->getAttribute(prop);

            Shared<BaseWidget> propertyField;
            if (isPropBoolean(propValue)) {
                propertyField = _createBooleanPropertyField(prop, propValue, true);
            } else {
                propertyField = _createRegularPropertyTextField(prop, propValue, true);
            }

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

    Shared<BaseWidget> WidgetPropertiesPanelController::_createBooleanPropertyField(
        const std::string& propName,
        const std::string& propValue,
        bool isBasicProperty
    ) {
        static const auto dimmedColor = Color(53, 54, 59);
        bool propBoolVal = propValue == "true";

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

        auto switchStackPanel = MakeRef<StackPanel>();
        switchStackPanel->orientation = Horizontal;
        switchStackPanel->backgroundColor = Color::transparent;
        switchStackPanel->fixedHeight = 20;

        auto falseButton = MakeRef<Button>();
        falseButton->fixedHeight = 16;
        falseButton->verticalAlignment = VACenter;
        falseButton->label->text = "false";
        falseButton->label->color = Color::white;
        falseButton->backgroundColor = Color(100, 0, 0);
        falseButton->borderColor = Color::transparent;
        falseButton->cornerRadius = 0;
        switchStackPanel->addChildOffline(falseButton);

        auto trueButton = MakeRef<Button>();
        trueButton->fixedHeight = 16;
        trueButton->verticalAlignment = VACenter;
        trueButton->label->text = "true";
        trueButton->label->color = Color::white;
        trueButton->backgroundColor = Color(0, 100, 0);
        trueButton->borderColor = Color::transparent;
        trueButton->cornerRadius = 0;
        switchStackPanel->addChildOffline(trueButton);

        if (propBoolVal) {
            falseButton->backgroundColor = dimmedColor;
        } else {
            trueButton->backgroundColor = dimmedColor;
        }

        falseButton->on("clicked", [this, isBasicProperty](Shared<Event> e) {
            // Get the property name through the parent's sibling label
            auto target = static_cast<Button*>(e->target);
            auto parentContainer = static_cast<StackPanel*>(target->getParent()->getParent());
            auto propertyNameLabel =
                std::static_pointer_cast<Label>(parentContainer->getChild(0));

            auto propName = propertyNameLabel->text.get();

            // Highlight the "false" button
            target->backgroundColor = Color(100, 0, 0);

            // Get the "true" button and dim it
            auto trueButton = std::static_pointer_cast<Button>(
                static_cast<StackPanel*>(target->getParent())->getChild(1)
            );
            trueButton->backgroundColor = dimmedColor;

            fireEvent("widgetPropertyModified", {
                { "propName", propName },
                { "propValue", std::string("false") },
                { "isBasicProp", isBasicProperty },
            });
        });

        trueButton->on("clicked", [this, isBasicProperty](Shared<Event> e) {
            // Get the property name through the parent's sibling label
            auto target = static_cast<Button*>(e->target);
            auto parentContainer = static_cast<StackPanel*>(target->getParent()->getParent());
            auto propertyNameLabel =
                std::static_pointer_cast<Label>(parentContainer->getChild(0));

            auto propName = propertyNameLabel->text.get();

            // Highlight the "true" button
            target->backgroundColor = Color(0, 100, 0);

            // Get the "false" button and dim it
            auto falseButton = std::static_pointer_cast<Button>(
                static_cast<StackPanel*>(target->getParent())->getChild(0)
            );
            falseButton->backgroundColor = dimmedColor;

            fireEvent("widgetPropertyModified", {
                { "propName", propName },
                { "propValue", std::string("true") },
                { "isBasicProp", isBasicProperty },
            });
        });

        container->addChildOffline(switchStackPanel);

        return container;
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
