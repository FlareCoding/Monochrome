set(CMAKE_CURRENT_SOURCE_DIR src/window)
set(cwd ${CMAKE_CURRENT_SOURCE_DIR})

set(
    WINDOW_SYSTEM_HEADERS

    ${cwd}/UIWindow.h
    ${cwd}/NativeWindow.h
    ${cwd}/ClassicWindow.h
    ${cwd}/ModernWindow.h
    ${cwd}/Overlay.h
    ${cwd}/EventProcessor.h

    PARENT_SCOPE
)

set(
    WINDOW_SYSTEM_SOURCES

    ${cwd}/UIWindow.cpp
    ${cwd}/NativeWindow.cpp
    ${cwd}/ClassicWindow.cpp
    ${cwd}/ModernWindow.cpp
    ${cwd}/Overlay.cpp
    ${cwd}/EventProcessor.cpp

    PARENT_SCOPE
)
