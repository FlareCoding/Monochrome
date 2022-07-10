set(CMAKE_CURRENT_SOURCE_DIR src/events)
set(cwd ${CMAKE_CURRENT_SOURCE_DIR})

set(
    EVENT_SYSTEM_HEADERS

    ${cwd}/Event.h
    ${cwd}/EventEmitter.h
    ${cwd}/PropertyObserver.h
    ${cwd}/MouseEvents.h
    ${cwd}/KeyboardEvents.h

    PARENT_SCOPE
)

set(
    EVENT_SYSTEM_SOURCES

    ${cwd}/Event.cpp
    ${cwd}/EventEmitter.cpp
    ${cwd}/KeyboardEvents.cpp

    PARENT_SCOPE
)
