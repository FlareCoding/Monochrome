#include "StackPanel.h"
#include "visuals/Visuals.h"
#include "layouts/VerticalStackLayout.h"
#include "layouts/HorizontalStackLayout.h"

namespace mc {
    StackPanel::StackPanel() {
        _createVisuals();
        _setupProperties();
    }

    void StackPanel::_createVisuals() {
        // Setup the panel's body rectangle
        auto bodyRect = MakeRef<RectVisual>();

        cornerRadius.forwardAssignment(&bodyRect->cornerRadius);
        backgroundColor.forwardAssignment(&bodyRect->color);
        addCoreVisualElement(bodyRect);
    }

    void StackPanel::_setupProperties() {
        d_verticalLayout = MakeRef<VerticalStackLayout>();
        d_horizontalLayout = MakeRef<HorizontalStackLayout>();

        d_verticalLayout->setSize(width, height);
        d_horizontalLayout->setSize(width, height);

        backgroundColor = Color::lightGray;
        backgroundColor.forwardEmittedEvents(this);

        cornerRadius = 2;
        cornerRadius.forwardEmittedEvents(this);

        orientation = Orientaion::Vertical;
        orientation.forwardEmittedEvents(this);

        on("childAdded", [this](Shared<Event> e) {
            auto child = e->get<BaseWidget*>("child");
            _onChildAdded(child);
        });

        on("childRemoved", [this](Shared<Event> e) {
            auto child = e->get<BaseWidget*>("child");
            _onChildRemoved(child);
        });

        on("widgetResized", [this](Shared<Event> e) {
            d_verticalLayout->setSize(width, height);
            d_horizontalLayout->setSize(width, height);

            _layoutUpdate();
        });
    }

    void StackPanel::_onChildAdded(BaseWidget* child) {
        _layoutUpdate();
    }

    void StackPanel::_onChildRemoved(BaseWidget* child) {
        _layoutUpdate();
    }

    void StackPanel::_layoutUpdate() {
        if (orientation == Horizontal) {
            d_horizontalLayout->positionChildren(_getChildren());
        } else {
            d_verticalLayout->positionChildren(_getChildren());
        }
    }
} // namespace mc
