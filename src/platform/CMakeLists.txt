set(CMAKE_CURRENT_SOURCE_DIR src/platform)
set(cwd ${CMAKE_CURRENT_SOURCE_DIR})

if (MSVC)
    add_subdirectory(windows)
elseif (APPLE)
    add_subdirectory(macos)
elseif (LINUX)
else()
    message("Undefined platform found, cannot find native implementation")
endif()

set(
    PLATFORM_HEADERS

    ${PLATFORM_NATIVE_HEADERS}

    PARENT_SCOPE
)

set(
    PLATFORM_SOURCES

    ${PLATFORM_NATIVE_SOURCES}

    PARENT_SCOPE
)
