cmake_minimum_required(VERSION 3.10)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release;MinSizeRel;RelWithDebInfo")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_definitions(-DUNICODE -D_UNICODE)

project(Monochrome)

include_directories(
    "src"
)

add_subdirectory(src)

add_library(
    ${PROJECT_NAME}

    ${MC_HEADERS}
    ${MC_SOURCES}
)

if (NOT MSVC)
    find_package(X11 REQUIRED)
    link_libraries(${X11_LIBRARIES})
    include_directories(${X11_INCLUDE_DIR})

    include_directories(
        /home/username/Cairo/cairoinstall/include/
        /home/username/Cairo/cairoinstall/include/cairo
    )

    link_directories(
        /home/username/Cairo/cairoinstall/lib/
    )

    target_link_libraries(${PROJECT_NAME} ${X11_LIBRARIES} "-lcairo")
endif()

add_subdirectory(tests)

if (MSVC)
    add_subdirectory(editor)
endif()
