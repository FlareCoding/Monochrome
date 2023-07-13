#include "EventEmitter.h"
#include <algorithm>

namespace mc {
    // Custom event handler map that allows saving event handlers with custom names
    static std::map<std::string, eventEmiterCallback_t> s_registeredNamedHandlers;

    EventEmitter::EventEmitter(const std::initializer_list<std::string>& allowedEvents) {
        d_allowedEvents = allowedEvents;
    }

    void EventEmitter::on(const std::string& eventName, eventEmiterCallback_t handler) {
        // Check if the event name is allowed
        CORE_ASSERT(
            std::count(d_allowedEvents.begin(), d_allowedEvents.end(), eventName),
            "Event '" + eventName + "' is not an allowed event"
        );

        d_handlers[eventName].push_back(std::any_cast<eventEmiterCallback_t>(handler));
    }

    void EventEmitter::off(const std::string& eventName) {
        // Check if the event name is allowed
        CORE_ASSERT(
            std::count(d_allowedEvents.begin(), d_allowedEvents.end(), eventName),
            "Event '" + eventName + "' is not an allowed event"
        );

        auto it = d_handlers.find(eventName);
        if (it != d_handlers.end()) {
            d_handlers.erase(it);
        }
    }

    void EventEmitter::fireEvent(
        const std::string& eventName,
        Shared<Event> event,
        BaseWidget* target
    ) {
        // Check if the event name is allowed
        CORE_ASSERT(
            std::count(d_allowedEvents.begin(), d_allowedEvents.end(), eventName),
            "Event '" + eventName + "' is not an allowed event"
        );

        event->name = eventName;
        event->target = target;

        if (d_handlers.find(eventName) != d_handlers.end()) {
            auto& eventHandlers = d_handlers[eventName];
            for (auto& handler : eventHandlers) {
                handler(event);
            }
        }
    }

    void EventEmitter::fireEvent(
        const std::string& eventName,
        const eventDataMap_t& data,
        BaseWidget* target
    ) {
        auto event = MakeRef<Event>(data);
        fireEvent(eventName, event, target);
    }

    void EventEmitter::forwardEmittedEvents(EventEmitter* emitter) {
        for (auto& eventName : this->d_allowedEvents) {
            // Copy current allowed events onto the other emitter instance
            if (!std::count(
                emitter->d_allowedEvents.begin(),
                emitter->d_allowedEvents.end(),
                eventName
            )) {
                emitter->appendAllowedEvent(eventName);
            }

            // Register a forwarding callback handler
            this->on(eventName, [eventName, emitter](Shared<Event> event) {
                emitter->fireEvent(eventName, event, event->target);
            });
        }
    }

    void EventEmitter::forwardEmittedEvent(EventEmitter* emitter, const std::string& eventName) {
        // Check if the event name is allowed
        auto it = std::find(d_allowedEvents.begin(), d_allowedEvents.end(), eventName);

        CORE_ASSERT(
            it != d_allowedEvents.end(),
            "Event '" + eventName + "' is not an allowed event"
        );

        // Check if the event name needs to be allowed on the other emitter
        if (!std::count(
            emitter->d_allowedEvents.begin(),
            emitter->d_allowedEvents.end(),
            eventName
        )) {
            emitter->appendAllowedEvent(eventName);
        }

        // Register a forwarding callback handler
        this->on(eventName, [eventName, emitter](Shared<Event> event) {
            emitter->fireEvent(eventName, event, event->target);
        });
    }

    void registerNamedEventHandler(const std::string& name, eventEmiterCallback_t fn) {
        s_registeredNamedHandlers[name] = fn;
    }

    eventEmiterCallback_t getRegisteredNamedEventHandler(const std::string& name) {
        if (s_registeredNamedHandlers.find(name) != s_registeredNamedHandlers.end()) {
            return s_registeredNamedHandlers.at(name);
        }

        return nullptr;
    }
} // namespace mc
