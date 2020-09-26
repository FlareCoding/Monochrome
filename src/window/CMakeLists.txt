cmake_minimum_required(VERSION 3.0)

set(
    MC_WINDOW_HEADERS
    
    src/window/SceneManager.h
    src/window/UIWindow.h
    src/window/WindowStyle.h
    src/window/FileMenu.h
    src/window/FileMenuBar.h

    PARENT_SCOPE
)

set(
    MC_WINDOW_SOURCES

    src/window/SceneManager.cpp
    src/window/FileMenu.cpp
    src/window/FileMenuBar.cpp

    PARENT_SCOPE
)
