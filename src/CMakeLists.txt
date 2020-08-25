cmake_minimum_required(VERSION 3.0)

set(CMAKE_CURRENT_SOURCE_DIR src)

add_subdirectory(core)
add_subdirectory(events)
add_subdirectory(graphics)
add_subdirectory(ui)
add_subdirectory(window)
add_subdirectory(platform)

set(
    MC_HEADERS

    ${MC_CORE_HEADERS}
    ${MC_EVENTS_HEADERS}
    ${MC_GRAPHICS_HEADERS}
    ${MC_UI_HEADERS}
    ${MC_WINDOW_HEADERS}
    ${MC_PLATFORM_HEADERS}

    src/Monochrome.h

    PARENT_SCOPE
)

set(
    MC_SOURCES

    ${MC_EVENTS_SOURCES}
    ${MC_GRAPHICS_SOURCES}
    ${MC_UI_SOURCES}
    ${MC_WINDOW_SOURCES}
    ${MC_PLATFORM_SOURCES}

    PARENT_SCOPE
)


