cmake_minimum_required(VERSION 3.0)

set(CMAKE_BUILD_TYPE=Release)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")

project(Monochrome)

set(
    COMMON_FILES
    src/main.cpp
)

if (APPLE)
    set(CMAKE_EXE_LINKER_FLAGS "-framework Foundation -w")

    set(
        PLATFORM_FILES
    )
elseif (WIN32)
    set(
        PLATFORM_FILES
        src/platform/window/Win32Window.cpp
    )
elseif(UNIX)
    set(
        PLATFORM_FILES 
    )
endif()

include_directories(
    "src"
)

add_executable(
    Monochrome
    ${PLATFORM_FILES}
    ${COMMON_FILES}
)
