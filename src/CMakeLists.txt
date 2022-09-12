set(CMAKE_CURRENT_SOURCE_DIR src)
set(cwd ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(core)
add_subdirectory(application)
add_subdirectory(platform)
add_subdirectory(window)
add_subdirectory(events)
add_subdirectory(utils)
add_subdirectory(widgets)
add_subdirectory(rendering)
add_subdirectory(mcx)

set(
    MONOCHROME_HEADERS

    ${CORE_HEADERS}
    ${APPLICATION_HEADERS}
    ${WINDOW_SYSTEM_HEADERS}
    ${EVENT_SYSTEM_HEADERS}
    ${UTILS_HEADERS}
    ${WIDGETS_HEADERS}
    ${RENDERING_HEADERS}
    ${MCX_HEADERS}

    ${PLATFORM_HEADERS}
    PARENT_SCOPE
)

set(
    MONOCHROME_SOURCES

    ${CORE_SOURCES}
    ${APPLICATION_SOURCES}
    ${WINDOW_SYSTEM_SOURCES}
    ${EVENT_SYSTEM_SOURCES}
    ${UTILS_SOURCES}
    ${WIDGETS_SOURCES}
    ${RENDERING_SOURCES}
    ${MCX_SOURCES}

    ${PLATFORM_SOURCES}
    PARENT_SCOPE
)