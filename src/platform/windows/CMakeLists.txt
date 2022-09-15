set(CMAKE_CURRENT_SOURCE_DIR src/platform/windows)
set(cwd ${CMAKE_CURRENT_SOURCE_DIR})

set(
    PLATFORM_NATIVE_HEADERS

    ${cwd}/Win32ApplicationContext.h
    ${cwd}/Win32NativeWindow.h
    ${cwd}/Win32RenderTarget.h
    ${cwd}/Win32FileDialog.h
    ${cwd}/D2DGraphics.h

    PARENT_SCOPE
)

set(
    PLATFORM_NATIVE_SOURCES

    ${cwd}/Win32ApplicationContext.cpp
    ${cwd}/Win32NativeWindow.cpp
    ${cwd}/Win32RenderTarget.cpp
    ${cwd}/D2DGraphics.cpp
    ${cwd}/Win32Clipboard.cpp
    ${cwd}/Win32Cursor.cpp
    ${cwd}/Win32FileDialog.cpp
    ${cwd}/Win32Bitmap.cpp

    PARENT_SCOPE
)
