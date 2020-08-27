cmake_minimum_required(VERSION 3.0)

if (MSVC)
    set(
        MC_PLATFORM_HEADERS

        src/platform/WindowsWindow.h
        src/platform/WindowsRenderTarget.h
        src/platform/WindowsGraphics.h
        src/graphics/d2d_resources/CoreResources.h

        PARENT_SCOPE
    )

    set(
        MC_PLATFORM_SOURCES

        src/platform/WindowsWindow.cpp
        src/platform/WindowsRenderTarget.cpp
        src/platform/WindowsGraphics.cpp
        src/graphics/d2d_resources/CoreResources.cpp

        PARENT_SCOPE
    )
elif(CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(
        MC_PLATFORM_HEADERS

        src/platform/LinuxWindow.h
        src/platform/LinuxRenderTarget.h
        src/platform/LinuxGraphics.h

        PARENT_SCOPE
    )

    set(
        MC_PLATFORM_SOURCES

        src/platform/LinuxWindow.cpp
        src/platform/LinuxRenderTarget.cpp
        src/platform/LinuxGraphics.cpp

        PARENT_SCOPE
    )
else()
    set(
        MC_PLATFORM_HEADERS

        src/platform/OSXNativeCore.h
        src/platform/OSXWindow.h
        src/platform/OSXRenderTarget.h
        src/platform/OSXGraphics.h

        PARENT_SCOPE
    )

    set(
        MC_PLATFORM_SOURCES

        src/platform/OSXNativeCore.mm
        src/platform/OSXWindow.mm
        src/platform/OSXRenderTarget.mm
        src/platform/OSXGraphics.mm

        PARENT_SCOPE
    )
endif()
