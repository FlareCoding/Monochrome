#pragma once
#include <core/Core.h>
#include "Event.h"
#include <vector>
#include <functional>

namespace mc {
using eventEmiterCallback_t = std::function<void(Shared<Event>)>;

class BaseWidget;

class EventEmitter {
public:
    EventEmitter() = default;
    EventEmitter(const std::initializer_list<std::string>& allowedEvents);

    void on(const std::string& eventName, eventEmiterCallback_t handler);
    void off(const std::string& eventName);
    void fireEvent(const std::string& eventName, Shared<Event> event, BaseWidget* target = nullptr);
    void fireEvent(
        const std::string& eventName,
        const eventDataMap_t& data,
        BaseWidget* target = nullptr
    );

    template <typename T>
    void on(const std::string& eventName, void (T::*fn)(Shared<Event>), T* obj) {
        on(eventName, [fn, obj](Shared<Event> e) {
            (obj->*fn)(e);
        });
    }

    void forwardEmittedEvents(EventEmitter* emitter);
    void forwardEmittedEvent(EventEmitter* emitter, const std::string& eventName);

    inline void appendAllowedEvent(
        const std::string& eventName
    ) { d_allowedEvents.push_back(eventName); }

    inline const std::vector<std::string>& inspectAllowedEvents() const {
        return d_allowedEvents;
    }

private:
    std::map<std::string, std::vector<eventEmiterCallback_t>> d_handlers;
    std::vector<std::string> d_allowedEvents;
};

/// @brief Saves the provided callback fn 
/// @param name Unique name for the event handler
/// @param fn Callback function to be used in the event handler
void registerNamedEventHandler(const std::string& name, eventEmiterCallback_t fn);

/// @brief Saves the provided callback fn 
/// @param name Unique name for the event handler
/// @param fn Callback class member function to be used in the event handler
/// @param obj Class instance for the member function
template <typename T>
void registerNamedEventHandler(const std::string& eventName, void (T::* fn)(Shared<Event>), T* obj) {
    registerNamedEventHandler(eventName, [fn, obj](Shared<Event> e) {
        (obj->*fn)(e);
    });
}

/// @param name Unique name for the event handler
/// @returns The event handler function associated with the
/// given name, nullptr if no handler was not found.
eventEmiterCallback_t getRegisteredNamedEventHandler(const std::string& name);
} // namespace mc
