cmake_minimum_required(VERSION 3.10)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release;MinSizeRel;RelWithDebInfo")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_definitions(-DUNICODE -D_UNICODE)

project(Monochrome)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include_directories(
    "src"
)

add_subdirectory(src)

add_library(
    ${PROJECT_NAME}

    ${MC_HEADERS}
    ${MC_SOURCES}
)

if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    find_package(X11 REQUIRED)
    link_libraries(${X11_LIBRARIES})
    include_directories(${X11_INCLUDE_DIR})

    find_package(Cairo)
    link_libraries(${CAIRO_LIBRARIES})
    include_directories(${CAIRO_INCLUDE_DIRS})

    find_package(Pango)
    link_libraries(${Pango_LIBRARIES})
    include_directories(${Pango_INCLUDE_DIRS})

    find_package(PkgConfig REQUIRED)
    pkg_search_module(GLIB REQUIRED glib-2.0)
    include_directories(${GLIB_INCLUDE_DIRS})
    link_libraries(${GLIB_LDFLAGS})

    target_link_libraries(
        ${PROJECT_NAME} 
        ${X11_LIBRARIES} 
        ${CAIRO_LIBRARIES} 
        ${Pango_LIBRARIES} "-lpangocairo-1.0"
        ${GLIB_LDFLAGS}
        "-lpthread"
    )
endif()

if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
endif()

add_subdirectory(tests)

if (MSVC)
    add_subdirectory(editor)
endif()
