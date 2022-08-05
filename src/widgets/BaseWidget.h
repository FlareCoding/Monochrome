#pragma once
#include "WidgetContainer.h"
#include <events/PropertyObserver.h>
#include <utils/uuid.h>

namespace mc {
class BaseWidget : public WidgetContainer {
public:
    virtual ~BaseWidget() = default;

    PropertyObserver<uint32_t>    zIndex;

    void setParent(BaseWidget* parent) { d_parent = parent; }
    inline BaseWidget* getParent() const { return d_parent; }

protected:
    BaseWidget* d_parent = nullptr;

private:
    uuid_t d_uuid;
    uint64_t d_internalFlags = 0;
};
} // namespace mc
