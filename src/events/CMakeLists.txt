cmake_minimum_required(VERSION 3.0)

set(
    MC_EVENTS_HEADERS
    
    src/events/Event.h
    src/events/Events.h
    src/events/EventValidator.h
    src/events/IResponder.h
    src/events/KeyboardEvents.h
    src/events/MouseEvents.h
    src/events/WindowEvents.h
    src/events/FocusChangedEvent.h
    src/events/CustomEvents.h

    PARENT_SCOPE
)

set(
    MC_EVENTS_SOURCES

    src/events/EventValidator.cpp
    src/events/IResponder.cpp
    src/events/KeyboardEvents.cpp

    PARENT_SCOPE
)