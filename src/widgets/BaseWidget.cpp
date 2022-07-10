#include "BaseWidget.h"
#include <core/InternalFlags.h>

namespace mc
{
    BaseWidget::BaseWidget() {
        d_uuid = utils::generateUUID();
        
        _setDefaultProperties();
        appendAllowedEvent("propertyChanged");
        appendAllowedEvent("widgetMoved");
        appendAllowedEvent("widgetResized");
        appendAllowedEvent("mouseDown");
        appendAllowedEvent("mouseUp");
        appendAllowedEvent("mouseMoved");
        appendAllowedEvent("clicked");
        appendAllowedEvent("hoveredOn");
        appendAllowedEvent("hoveredOff");
        appendAllowedEvent("keyDown");
        appendAllowedEvent("keyUp");
        appendAllowedEvent("gainedFocus");
        appendAllowedEvent("lostFocus");
        appendAllowedEvent("requestedFocusGain");
        appendAllowedEvent("requestedFocusLoss");
    }

    void BaseWidget::_setDefaultProperties() {
        visible = true;
        visible.forwardEmittedEvents(this);

        enabled = true;
        enabled.forwardEmittedEvents(this);

        focused = true;
        focused.forwardEmittedEvents(this);

        position = { 0, 0 };
        position.forwardEmittedEvents(this);
        position.on("propertyChanged", [this](auto e) {
            this->fireEvent(
                "widgetMoved",
                MakeRef<Event>(eventDataMap_t{
                    { "x", position->x },
                    { "y", position->y },
                    { "width", size->width },
                    { "height", size->height }
                })
            );
        });

        size = { 10, 10 };
        size.forwardEmittedEvents(this);
        size.on("propertyChanged", [this](auto e) {
            this->fireEvent(
                "widgetResized",
                MakeRef<Event>(eventDataMap_t{
                    { "x", position->x },
                    { "y", position->y },
                    { "width", size->width },
                    { "height", size->height }
                })
            );
        });

        color = Color::black;
        color.forwardEmittedEvents(this);

        backgroundColor = Color::lightGray;
        backgroundColor.forwardEmittedEvents(this);

        marginTop = 0;
        marginTop.forwardEmittedEvents(this);

        marginBottom = 0;
        marginBottom.forwardEmittedEvents(this);

        marginLeft = 0;
        marginLeft.forwardEmittedEvents(this);

        marginRight = 0;
        marginRight.forwardEmittedEvents(this);
    }
    
    Frame BaseWidget::getLayerFrame() const {
        return Frame(
            Position({ position->x, position->y }),
            Size({ size->width, size->height })
        );
    }

    Position BaseWidget::getPositionInWindow() const {
        Position result = this->position.get();

        BaseWidget* parent = d_parent;
        while (parent) {
            result.x += parent->position->x;
            result.y += parent->position->y;

            parent = parent->d_parent;
        }

        return result;
    }

    bool BaseWidget::isFocused() {
        return getInternalFlag(d_internalFlags, InternalWidgetFlag::IsFocused);
    }

    void BaseWidget::focus() {
        fireEvent("requestedFocusGain", MakeRef<Event>(eventDataMap_t{
            { "target", this }
        }));
    }
    
    void BaseWidget::unfocus() {
        fireEvent("requestedFocusLoss", Event::empty);
    }
}
