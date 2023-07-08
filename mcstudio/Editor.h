#pragma once
#include "controllers/WidgetTreeController.h"
#include "controllers/EditorCanvasController.h"
#include <mcx/adapters/BaseWidgetMcxAdapter.h>

namespace mc::mcstudio {
class Editor {
public:
    Editor();
    inline void setWindowCanvasReference(Shared<Canvas2D> canvas) {
        d_overlayCanvasReference = canvas;
    }

    void toolboxWidget_OnClick(Shared<Event> e);
    void rootContainerSelection_OnClick(Shared<Event> e);

    // Marks a widget as the currently selected one, showing its
    // properties and giving the user the ability to customize it.
    void setSelectedWidget(Shared<BaseWidget> widget);

private:
    // Logic pertaining to whenever the app's root container is pressed on
    void _appRootContainer_OnClick(Shared<Event> e);

    // Logic pertaining to whenever a key is pressed in the editor
    void _appRootContainer_OnKeyDown(Shared<Event> e);

    // Removes the widget from the app view, the widget tree, and all other necessary places
    void _removeWidget(Shared<BaseWidget> widget);

    // Creates and returns new widget from a given widget name
    Shared<BaseWidget> _spawnWidget(const std::string& widgetName);

    // Removes all property entries from the widget properties panel
    void _clearPropertiesPanel();

    // Fills the properties panel with current widget's properties
    void _fillPropertiesPanel();

    // Creates a widget container consisting of a label and an entry
    // which upon modification will modify the selected widget's property.
    Shared<BaseWidget> _createPropertyEntry(const std::string& name, bool isBasicProperty);

    // Returns the inner-most widget in the app's widget tree that a given point intersects
    Shared<BaseWidget> _hitTestInnermostWidget(
        Shared<BaseContainerWidget> root,
        const Point& point
    );

private:
    // Controller responsible for the widget tree panel
    Shared<WidgetTreeController> d_widgetTreeController;

    // Controller responsible for drawing widget overlay frames
    Shared<EditorCanvasController> d_editorCanvasController;

    // Overlay canvas
    Shared<Canvas2D> d_overlayCanvasReference;

    // App's root container widget
    Shared<BaseContainerWidget> d_appRootContainer;

    // Node responsible for holding current selected widget's mcx information
    Shared<mcx::McxNode> d_selectedWidgetNode;

    // Adapter for basic widget properties
    Shared<mcx::BaseWidgetMcxAdapter> d_baseWidgetAdapter;

    // Adapter for the current selected widget
    Shared<mcx::McxParsingAdapter> d_selectedWidgetAdapter;

    // Currently selected widget (either newly added or clicked on with a mouse)
    Shared<BaseWidget> d_selectedWidget;
};
} // namespace mc::mcstudio
