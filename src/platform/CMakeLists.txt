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
else()
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
endif()
