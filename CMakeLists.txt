cmake_minimum_required(VERSION 3.10)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release;MinSizeRel;RelWithDebInfo")

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17 /EHsc")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
endif()

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

if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
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

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_subdirectory(editor)
endif()
