cmake_minimum_required(VERSION 3.0)

set(
    MC_PLATFORM_HEADERS
    
    src/platform/WindowsWindow.h
    src/platform/WindowsRenderTarget.h
    src/platform/WindowsGraphics.h

    PARENT_SCOPE
)

set(
    MC_PLATFORM_SOURCES

    src/platform/WindowsWindow.cpp
    src/platform/WindowsRenderTarget.cpp
    src/platform/WindowsGraphics.cpp

    PARENT_SCOPE
)
