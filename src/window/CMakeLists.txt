cmake_minimum_required(VERSION 3.0)

set(
    MC_WINDOW_HEADERS
    
    src/window/SceneManager.h
    src/window/UIWindow.h
    src/window/WindowStyle.h

    PARENT_SCOPE
)

set(
    MC_WINDOW_SOURCES

    src/window/SceneManager.cpp
    src/window/UIWindow.cpp

    PARENT_SCOPE
)
