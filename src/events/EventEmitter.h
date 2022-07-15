#pragma once
#include <core/Core.h>
#include "Event.h"
#include <vector>
#include <functional>

namespace mc {
    using eventEmiterCallback_t = std::function<void(Shared<Event>)>;

class EventEmitter {
public:
    EventEmitter() = default;
    EventEmitter(const std::initializer_list<std::string>& allowedEvents);

    void on(const std::string& eventName, eventEmiterCallback_t handler);
    void off(const std::string& eventName);
    void fireEvent(const std::string& eventName, Shared<Event> data);

    void forwardEmittedEvents(EventEmitter* emitter);
    void forwardEmittedEvent(EventEmitter* emitter, const std::string& eventName);

    inline void appendAllowedEvent(
        const std::string& eventName
    ) { d_allowedEvents.push_back(eventName); }

private:
    std::map<std::string, std::vector<eventEmiterCallback_t>> d_handlers;
    std::vector<std::string> d_allowedEvents;
};
} // namespace mc
