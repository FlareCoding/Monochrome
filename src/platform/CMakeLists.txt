cmake_minimum_required(VERSION 3.0)

if (MSVC)
    set(
        MC_PLATFORM_HEADERS

        src/platform/windows/WindowsWindow.h
        src/platform/windows/WindowsRenderTarget.h
        src/platform/windows/WindowsGraphics.h
        src/platform/windows/WindowsBitmap.h
        src/platform/windows/WindowsFileDialogue.h
        src/platform/windows/d2d_resources/CoreResources.h

        PARENT_SCOPE
    )

    set(
        MC_PLATFORM_SOURCES

        src/platform/windows/WindowsWindow.cpp
        src/platform/windows/WindowsRenderTarget.cpp
        src/platform/windows/WindowsGraphics.cpp
        src/platform/windows/WindowsBitmap.cpp
        src/platform/windows/WindowsFileDialogue.cpp
        src/platform/windows/d2d_resources/CoreResources.cpp

        PARENT_SCOPE
    )
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(
        MC_PLATFORM_HEADERS

        src/platform/linux/LinuxWindow.h
        src/platform/linux/LinuxRenderTarget.h
        src/platform/linux/LinuxGraphics.h
        src/platform/linux/LinuxBitmap.h
        src/platform/linux/LinuxFileDialogue.h
        src/platform/linux/stbimage/stb_image.h

        PARENT_SCOPE
    )

    set(
        MC_PLATFORM_SOURCES

        src/platform/linux/LinuxWindow.cpp
        src/platform/linux/LinuxRenderTarget.cpp
        src/platform/linux/LinuxGraphics.cpp
        src/platform/linux/LinuxBitmap.cpp
        src/platform/linux/LinuxFileDialogue.cpp
        src/platform/linux/stbimage/stb_image.cpp

        PARENT_SCOPE
    )
else()
    set(
        MC_PLATFORM_HEADERS

        src/platform/osx/OSXNativeCore.h
        src/platform/osx/OSXWindow.h
        src/platform/osx/OSXRenderTarget.h
        src/platform/osx/OSXGraphics.h
        src/platform/osx/OSXBitmap.h
        src/platform/osx/OSXFileDialogue.h

        PARENT_SCOPE
    )

    set(
        MC_PLATFORM_SOURCES

        src/platform/osx/OSXNativeCore.mm
        src/platform/osx/OSXWindow.mm
        src/platform/osx/OSXRenderTarget.mm
        src/platform/osx/OSXGraphics.mm
        src/platform/osx/OSXBitmap.mm
        src/platform/osx/OSXFileDialogue.mm

        PARENT_SCOPE
    )
endif()
