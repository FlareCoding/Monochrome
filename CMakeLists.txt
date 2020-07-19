cmake_minimum_required(VERSION 3.0)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release;MinSizeRel;RelWithDebInfo")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17 /EHsc")

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

add_subdirectory(tests)
