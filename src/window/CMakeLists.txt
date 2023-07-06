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
    ${cwd}/Canvas2D.h

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
    ${cwd}/Canvas2D.cpp

    PARENT_SCOPE
)
