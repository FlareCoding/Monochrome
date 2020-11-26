#pragma once
#include "Event.h"

namespace mc
{
    class DockingUpdateEvent : public Event
    {
    public:
        DockingUpdateEvent(UIView* src) : SourcePanel(src) {}

        UIView* SourcePanel;

        REGISTER_EVT_CATEGORY(EventCategoryApplication)
        REGISTER_EVT_CLASS_TYPE(DockingUpdate)
    };
}
