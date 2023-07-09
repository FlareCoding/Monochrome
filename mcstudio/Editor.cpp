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

        // Create a node in the widget tree
        d_widgetTreeController->insertWidgetNode(widget);
    }

    void Editor::rootContainerSelection_OnClick(Shared<Event> e) {
        // Initialize controllers
        d_widgetTreeController = MakeRef<WidgetTreeController>();
        d_editorCanvasController = MakeRef<EditorCanvasController>(d_overlayCanvasReference);
        d_widgetPropertiesPanelController =
            MakeRef<WidgetPropertiesPanelController>(d_baseWidgetAdapter);

        d_widgetPropertiesPanelController->on(
            "widgetPropertyModified",
            &Editor::_onWidgetPropertyModified,
            this
        );

        getWidgetById("initialRootContainerPromptLabel")->hide();
        getWidgetById("initialRootContainerPromptPanel")->hide();

        // Create the root container with the appropriate type
        auto target = static_cast<Button*>(e->target);
        auto widgetName = target->label->text.get();
        auto mcxAdapter = mcx::McxEngine::getMcxAdapter(widgetName);

        Shared<mcx::McxNode> limboNode = nullptr;
        d_appRootContainer = std::static_pointer_cast<BaseContainerWidget>(
            mcxAdapter->createWidgetInstance(limboNode));

        d_appRootContainer->fixedWidth = 800;
        d_appRootContainer->fixedHeight = 800;
        d_appRootContainer->marginLeft = 20;

        // Add the root container to the editor panel
        auto editorPanel = getWidgetById<StackPanel>("mcEditorPanel");
        editorPanel->addChild(d_appRootContainer);

        // Register the root container ID
        registerWidgetWithUserId("appRootContainer", d_appRootContainer);

        d_appRootContainer->on("mouseUp", [](Shared<Event> e) {
            e->stopPropagation();
        });

        d_appRootContainer->on("mouseDown", [this](Shared<Event> e) {
            e->stopPropagation();

            _appRootContainer_OnClick(e);
        });

        d_appRootContainer->on("mouseMoved", [this](Shared<Event> e) {
            auto mme = std::static_pointer_cast<MouseMovedEvent>(e);

            auto hoveredWidget = _hitTestInnermostWidget(d_appRootContainer, mme->getLocation());
            if (hoveredWidget) {
                d_editorCanvasController->clearCanvas();
                d_editorCanvasController->drawWidgetEditFrame(hoveredWidget);
                
                if (d_selectedWidget) {
                    d_editorCanvasController->drawWidgetEditFrame(d_selectedWidget);
                }
            }
        });

        d_appRootContainer->on("keyDown", &Editor::_appRootContainer_OnKeyDown, this);

        // Add the root container to the widget tree view
        d_widgetTreeController->setRootWidget(d_appRootContainer);

        // Set the callback so that every time a node is selected
        // in the widget tree, the cooresponding widget is selected.
        d_widgetTreeController->setNodeSelectedCallback([this](uuid_t selectedWidgetId) {
            auto widget = d_appRootContainer->deepSearchWidgetByUuid(selectedWidgetId);
            setSelectedWidget(widget);
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

    void Editor::_appRootContainer_OnKeyDown(Shared<Event> e) {
        auto kde = std::static_pointer_cast<KeyDownEvent>(e);
        
        // Deleting the selected widget
        if (kde->getKeyCode() == KeyCode::KEY_BACKSPACE) {
            if (!d_selectedWidget) {
                return;
            }

            _removeWidget(d_selectedWidget);
        }
    }

    void Editor::_onWidgetPropertyModified(Shared<Event> e) {
        auto event = std::static_pointer_cast<WidgetPropertyModifiedEvent>(e);

        // Update the property in the mcx node
        d_selectedWidgetNode->setAttribute(event->getPropName(), event->getPropValue());

        if (event->isBasicProperty()) {
            d_baseWidgetAdapter->applyProperties(d_selectedWidget, d_selectedWidgetNode);
        } else {
            d_selectedWidgetAdapter->applyProperties(d_selectedWidget, d_selectedWidgetNode);
        }

        // In case the size of the widget changed, the edit frame needs to be redrawn
        d_editorCanvasController->clearCanvas();
        d_editorCanvasController->drawWidgetEditFrame(d_selectedWidget);
    }

    void Editor::_removeWidget(Shared<BaseWidget> widget) {
        // Remove the selected widget from its parent
        const auto parent = static_cast<BaseContainerWidget*>(widget->getParent());
        parent->removeChild(widget);

        // Remove the selected widget from the widget tree
        d_widgetTreeController->removeWidgetNode(widget);

        // Clear the canvas
        d_editorCanvasController->clearCanvas();
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

        // Clear the overlay to be prevent drawing a frame around the previously selected widget
        d_editorCanvasController->clearCanvas();

        // Nothing further is needed to be done if no real widget is selected
        if (!d_selectedWidget) {
            d_widgetPropertiesPanelController->clear();
            d_widgetTreeController->selectWidget(nullptr);
            return;
        }

        // Find the selected widget's mcx adapter
        d_selectedWidgetAdapter =
            mcx::McxEngine::getMcxAdapter(d_selectedWidget->getWidgetName());

        // Fill the selected widget's mcx node with the widget's properties
        d_baseWidgetAdapter->extractProperties(d_selectedWidget, d_selectedWidgetNode);
        d_selectedWidgetAdapter->extractProperties(d_selectedWidget, d_selectedWidgetNode);

        // Display properties for the current selected widget
        d_widgetPropertiesPanelController->displayWidgetProperties(
            d_selectedWidgetNode,
            d_selectedWidgetAdapter
        );

        // Highlight the selected widget in the widget tree
        d_widgetTreeController->selectWidget(d_selectedWidget);

        // Draw an "editor frame" around the selected widget
        d_editorCanvasController->drawWidgetEditFrame(d_selectedWidget);
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
