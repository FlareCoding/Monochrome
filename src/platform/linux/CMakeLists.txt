set(CMAKE_CURRENT_SOURCE_DIR src/platform/linux)
set(cwd ${CMAKE_CURRENT_SOURCE_DIR})

set(
    PLATFORM_NATIVE_HEADERS

    ${cwd}/LinuxApplicationContext.h
    ${cwd}/LinuxNativeWindow.h
    ${cwd}/LinuxRenderTarget.h
    ${cwd}/LinuxFileDialog.h

    PARENT_SCOPE
)

set(
    PLATFORM_NATIVE_SOURCES

    ${cwd}/LinuxApplicationContext.cpp
    ${cwd}/LinuxNativeWindow.cpp
    ${cwd}/LinuxRenderTarget.cpp
    ${cwd}/LinuxClipboard.cpp
    ${cwd}/LinuxCursor.cpp
    ${cwd}/LinuxFileDialog.cpp
    ${cwd}/LinuxBitmap.cpp

    PARENT_SCOPE
)
