set(CMAKE_CURRENT_SOURCE_DIR src/platform/macos)
set(cwd ${CMAKE_CURRENT_SOURCE_DIR})

set(
    PLATFORM_NATIVE_HEADERS

    ${cwd}/OSXApplicationContext.h
    ${cwd}/OSXNativeWindow.h
    ${cwd}/OSXRenderTarget.h

    PARENT_SCOPE
)

set(
    PLATFORM_NATIVE_SOURCES

    ${cwd}/OSXApplicationContext.mm
    ${cwd}/OSXNativeWindow.mm
    ${cwd}/OSXRenderTarget.mm
    ${cwd}/OSXClipboard.mm
    ${cwd}/OSXCursor.mm

    PARENT_SCOPE
)
