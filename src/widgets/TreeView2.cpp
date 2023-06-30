#include "TreeView2.h"
#include "visuals/RectVisual.h"

namespace mc {
    TreeView2::TreeView2() {
        appendAllowedEvent("itemSelected");

        _createVisuals();
        _setupProperties();
    }

    Size TreeView2::_measureSize() {
        return Size(200, 200);
    }

    void TreeView2::_onArrangeChildren() {
    }

    void TreeView2::_createVisuals() {
        // Setup the panel's body rectangle
        auto bodyRect = MakeRef<RectVisual>();

        backgroundColor.forwardAssignment(&bodyRect->color);
        addCoreVisualElement(bodyRect);
    }

    void TreeView2::_setupProperties() {
        backgroundColor = Color::red; // default value
        backgroundColor.forwardEmittedEvents(this);

        cursorType = CursorType::Arrow;
    }
} // namespace mc
