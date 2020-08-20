cmake_minimum_required(VERSION 3.0)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release;")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17 /EHsc")

add_definitions(-DUNICODE -D_UNICODE)

project(MonochromePythonBindings)

include_directories(
    "C:\\Users\\alber\\Desktop\\Monochrome\\src\\"
    "C:\\Users\\alber\\AppData\\Local\\Programs\\Python\\Python38-32\\include\\"

    "src"
)

add_library(
    ${PROJECT_NAME}
    SHARED

    # Module
    src/monochrome_module.cpp
    
    # Window
    src/window/uiwindow_bindings.h
    src/window/uiwindow_bindings.cpp

    # UI
    src/ui/iresponder_bindings.h
    src/ui/iresponder_bindings.cpp
    
    src/ui/uiview_bindings.h
    src/ui/uiview_bindings.cpp

    src/ui/uilabel_bindings.h
    src/ui/uilabel_bindings.cpp
    
    src/ui/uibutton_bindings.h
    src/ui/uibutton_bindings.cpp
)

# Monochrome lib debug path
find_library(
    MONOCHROME_LIB_DEBUG
    NAMES Monochrome
    HINTS
    "C:\\Users\\alber\\Desktop\\Monochrome\\build\\Debug\\"
)

# Monochrome lib release path
find_library(
    MONOCHROME_LIB_RELEASE
    NAMES Monochrome
    HINTS 
    "C:\\Users\\alber\\Desktop\\Monochrome\\build\\Release\\"
)

target_link_libraries(${PROJECT_NAME} debug ${MONOCHROME_LIB_DEBUG})
target_link_libraries(${PROJECT_NAME} optimized ${MONOCHROME_LIB_RELEASE})

# Python lib debug path
find_library(
    PYTHON_LIB_DEBUG
    NAMES python38_d
    HINTS
    "C:\\Users\\alber\\AppData\\Local\\Programs\\Python\\Python38-32\\libs\\"
)

# Python lib release path
find_library(
    PYTHON_LIB_RELEASE
    NAMES python38
    HINTS
    "C:\\Users\\alber\\AppData\\Local\\Programs\\Python\\Python38-32\\libs\\"
)

target_link_libraries(${PROJECT_NAME} debug ${PYTHON_LIB_DEBUG})
target_link_libraries(${PROJECT_NAME} optimized ${PYTHON_LIB_RELEASE})
