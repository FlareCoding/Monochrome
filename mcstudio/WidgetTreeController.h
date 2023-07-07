#pragma once
#include <widgets/AllWidgets.h>
#include <mcx/McxEngine.h>

namespace mc::mcstudio {
using widgetTreeNodeSelectedCallback_t = std::function<void(uuid_t)>;

class WidgetTreeController {
public:
    WidgetTreeController();

    // Removes the entire widget tree
    void clearTree();

    // Creates a root node for the widget tree that corresponds to the given container
    void setRootWidget(Shared<BaseContainerWidget> root);

    // Highlights and selects the corresponding node in the widget tree
    void selectWidget(Shared<BaseWidget> widget);

    // Creates and inserts a node associated with the widget into the widget tree
    void insertWidgetNode(Shared<BaseWidget> widget);

    // Removes the associated with the widget node from the widget tree
    void removeWidgetNode(Shared<BaseWidget> widget);

    // Sets a callback function that gets called whenever a node is selected
    inline void setNodeSelectedCallback(widgetTreeNodeSelectedCallback_t cb) {
        d_nodeSelectedCallback = cb;
    }

private:
    // Reference to the tree view widget
    Shared<TreeView> d_widgetTreeView;

    // Main root container node of the widget tree
    Shared<BaseContainerWidget> d_rootWidget;

    // Callback function that gets called whenever a node is selected
    widgetTreeNodeSelectedCallback_t d_nodeSelectedCallback;
};
} // namespace mc::mcstudio

