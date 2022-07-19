#pragma once
#include "Layer.h"
#include "Color.h"
#include <utils/uuid.h>
#include <utils/Cursor.h>
#include <events/PropertyObserver.h>

namespace mc {
class BaseWidget : public EventEmitter {
public:
    BaseWidget();
    virtual ~BaseWidget() = default;

    virtual const std::string getType() const = 0;
    Frame getLayerFrame() const;
    Position getPositionInWindow() const;

    inline uuid_t getID() const { return d_uuid; }
    inline virtual bool isContainer() const { return false; }
    inline uint64_t& getInternalFlags() { return d_internalFlags; }

    PropertyObserver<uint32_t>    zIndex;
    PropertyObserver<bool>        enabled;
    PropertyObserver<bool>        focused;
    PropertyObserver<bool>        visible;
    PropertyObserver<CursorType>  cursorType;
    PropertyObserver<Position>    position;
    PropertyObserver<Size>        size;
    PropertyObserver<Color>       color;
    PropertyObserver<Color>       backgroundColor;
    PropertyObserver<uint32_t>    marginTop;
    PropertyObserver<uint32_t>    marginBottom;
    PropertyObserver<uint32_t>    marginLeft;
    PropertyObserver<uint32_t>    marginRight;

    void setParent(BaseWidget* parent) { d_parent = parent; }
    inline BaseWidget* getParent() const { return d_parent; }

    bool isFocused();
    void focus();
    void unfocus();

protected:
    // Allows widgets to setup observable
    // properties and forward their events.
    virtual void _setupProperties() = 0;

    BaseWidget* d_parent = nullptr;

private:
    uuid_t d_uuid;
    uint64_t d_internalFlags = 0;

    void _setDefaultProperties();
};
} // namespace mc
