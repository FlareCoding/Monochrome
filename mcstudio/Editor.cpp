#include "Editor.h"

namespace mc::mcstudio {
    Editor::Editor() {
        d_baseWidgetAdapter     = MakeRef<mcx::BaseWidgetMcxAdapter>();
        d_selectedWidgetNode    = MakeRef<mcx::McxNode>();
        d_selectedWidgetAdapter = nullptr;
        d_selectedWidget        = nullptr;
        d_appRootContainer      = nullptr;

        registerNamedEventHandler("toolboxWidget_OnClick", &Editor::toolboxWidget_OnClick, this);
        registerNamedEventHandler("rootContainerSelection_OnClick",
                &Editor::rootContainerSelection_OnClick, this);
    }

    void Editor::toolboxWidget_OnClick(Shared<Event> e) {
        auto rootContainer = getWidgetById<BaseContainerWidget>("appRootContainer");
        if (!rootContainer) {
            return;
        }

        // Get the widget name from the event's target
        auto target = static_cast<Button*>(e->target);
        auto widgetName = target->label->text.get();
        
        // Create the new widget instance
        auto widget = _spawnWidget(widgetName);

        // If the currently selected widget is a,
        // container add the new widget to it, otherwise
        // add it to the application's root container.
        if (d_selectedWidget && d_selectedWidget->isContainer()) {
            auto selectedContainer =
                std::static_pointer_cast<BaseContainerWidget>(d_selectedWidget);

            selectedContainer->addChild(widget);
        } else {
            d_appRootContainer->addChild(widget);
            
            // Set the newly added widget as the selected one
            setSelectedWidget(widget);
        }
    }

    void Editor::rootContainerSelection_OnClick(Shared<Event> e) {
        getWidgetById("initialRootContainerPromptLabel")->hide();
        getWidgetById("initialRootContainerPromptPanel")->hide();

        auto target = static_cast<Button*>(e->target);
        auto widgetName = target->label->text.get();
        auto mcxAdapter = mcx::McxEngine::getMcxAdapter(widgetName);

        Shared<mcx::McxNode> limboNode = nullptr;
        d_appRootContainer = std::static_pointer_cast<BaseContainerWidget>(
            mcxAdapter->createWidgetInstance(limboNode));

        d_appRootContainer->fixedWidth = 800;
        d_appRootContainer->fixedHeight = 800;
        d_appRootContainer->marginLeft = 20;

        auto editorPanel = getWidgetById<StackPanel>("mcEditorPanel");
        editorPanel->addChild(d_appRootContainer);

        registerWidgetWithUserId("appRootContainer", d_appRootContainer);

        d_appRootContainer->on("mouseUp", [](Shared<Event> e) {
            e->stopPropagation();
        });

        d_appRootContainer->on("mouseDown", [this](Shared<Event> e) {
            e->stopPropagation();

            if (!d_appRootContainer) {
                return;
            }

            _appRootContainer_OnClick(e);
        });
    }
    
    void Editor::_appRootContainer_OnClick(Shared<Event> e) {
        auto& children = d_appRootContainer->getChildren();
        if (children.empty()) {
            return;
        }

        auto mbe = std::static_pointer_cast<MouseButtonEvent>(e);
        auto mousePos = mbe->getLocation();

        auto clickedWidget = _hitTestInnermostWidget(d_appRootContainer, mousePos);
        setSelectedWidget(clickedWidget);
    }

    void Editor::setSelectedWidget(Shared<BaseWidget> widget) {
        // To prevent memory corruption issues with the selected widget
        // memory getting freed up but its focus has not yet been lost,
        // we need to manually unfocus it before doing anything further.
        if (d_selectedWidget) {
            d_selectedWidget->unfocus();
        }

        // Clear the attributes of previously known selected widget
        d_selectedWidgetNode->removeAllAttributes();

        // Set the new selected widget
        d_selectedWidget = widget;

        // Nothing further is needed to be done if no real widget is selected
        if (!d_selectedWidget) {
            _clearPropertiesPanel();
            return;
        }

        // Find the selected widget's mcx adapter
        d_selectedWidgetAdapter =
            mcx::McxEngine::getMcxAdapter(d_selectedWidget->getWidgetName());

        // Fill the selected widget's mcx node with the widget's properties
        d_baseWidgetAdapter->extractProperties(d_selectedWidget, d_selectedWidgetNode);
        d_selectedWidgetAdapter->extractProperties(d_selectedWidget, d_selectedWidgetNode);

        // Display properties for the current selected widget
        _fillPropertiesPanel();
    }

    void Editor::_clearPropertiesPanel() {
        // Get the reference to the widget properties
        // panel and clear it from all previous entries.
        auto propertiesListPanel = getWidgetById<StackPanel>("propertiesListPanel");
        propertiesListPanel->removeAllChildren();
    }

    void Editor::_fillPropertiesPanel() {
        // Get the reference to the widget properties
        // panel and clear it from all previous entries.
        auto propertiesListPanel = getWidgetById<StackPanel>("propertiesListPanel");
        propertiesListPanel->removeAllChildren();

        // Add records of all basic properties
        for (auto& prop : d_baseWidgetAdapter->getAvailableProperties()) {
            auto propertyEntry = _createPropertyEntry(prop, true);
            propertiesListPanel->addChildOffline(propertyEntry);
        }

        // Empty space acting as a divider
        auto dividingSpace = MakeRef<Label>();
        dividingSpace->text = " ";
        propertiesListPanel->addChildOffline(dividingSpace);

        // Add records of all custom widget properties
        for (auto& prop : d_selectedWidgetAdapter->getAvailableProperties()) {
            auto propertyEntry = _createPropertyEntry(prop, false);
            propertiesListPanel->addChildOffline(propertyEntry);
        }

        // Since all the children have been added in the offline mode, a.k.a no
        // layout change events firing, the layout needs to be re-calculated.
        propertiesListPanel->signalLayoutChanged();
    }

    Shared<BaseWidget> Editor::_createPropertyEntry(
        const std::string& name,
        bool isBasicProperty
    ) {
        auto container = MakeRef<StackPanel>();
        container->orientation = Horizontal;
        container->backgroundColor = Color::transparent;

        auto propertyNameLabel = MakeRef<Label>();
        propertyNameLabel->text = name;
        propertyNameLabel->fixedWidth = 110;
        propertyNameLabel->marginLeft = 10;
        propertyNameLabel->alignment = "left";
        container->addChild(propertyNameLabel);

        auto entry = MakeRef<Entry>();
        entry->text = d_selectedWidgetNode->getAttribute(name);
        entry->fixedWidth = 150;
        entry->on("entered", [this, isBasicProperty](Shared<Event> e) {
            auto target = static_cast<Entry*>(e->target);
            auto parentContainer = static_cast<StackPanel*>(target->getParent());
            auto propertyNameLabel = std::static_pointer_cast<Label>(parentContainer->getChild(0));

            auto propName = propertyNameLabel->text.get();
            auto propValue = target->text.get();
            d_selectedWidgetNode->setAttribute(propName, propValue);
            
            if (isBasicProperty) {
                d_baseWidgetAdapter->applyProperties(d_selectedWidget, d_selectedWidgetNode);
            } else {
                d_selectedWidgetAdapter->applyProperties(d_selectedWidget, d_selectedWidgetNode);
            }
        });
        container->addChild(entry);

        return container;
    }

    Shared<BaseWidget> Editor::_spawnWidget(const std::string& widgetName) {
        auto mcxAdapter = mcx::McxEngine::getMcxAdapter(widgetName);
        auto tempNode = MakeRef<mcx::McxNode>();

        return mcxAdapter->createWidgetInstance(tempNode);
    }

    Shared<BaseWidget> Editor::_hitTestInnermostWidget(
        Shared<BaseContainerWidget> root,
        const Point& point
    ) {
        auto& children = root->getChildren();
        
        for (auto& child : children) {
            // Get window-relative position of the child widget
            auto childPos = child->getPositionInWindow();
            
            // Construct the frame object
            auto frame = Frame(childPos, child->getComputedSizeWithMargins());

            // Make sure the widget frame contains the given point,
            // and skip it if it doesn't contain it.
            if (!frame.containsPoint(point)) {
                continue;
            }

            // If the widget is not a container, and at this point it is established
            // that it is under the point's intersection, return this widget.
            if (!child->isContainer()) {
                return child;
            }

            // Now, if the widget under the intersection is a container, we
            // have to check if there is an inner widget that also intersects the point.
            // If there is such a widget, return it, otherwise return the current widget.
            auto innerWidget = _hitTestInnermostWidget(
                std::static_pointer_cast<BaseContainerWidget>(child),
                point
            );

            if (innerWidget) {
                return innerWidget;
            } else {
                return child;
            }
        }

        return nullptr;
    }
} // namespace mc::mcstudio
