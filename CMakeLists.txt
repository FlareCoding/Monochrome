cmake_minimum_required(VERSION 3.0)
set(CMAKE_CONFIGURATION_TYPES "Debug;Release")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_definitions(-DUNICODE -D_UNICODE)

# set(SHOULD_BUNDLE_APP)

if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

if (APPLE)
    set(CMAKE_MACOSX_RPATH OFF)
endif()

project(Monochrome)
set(TARGET_NAME Monochrome)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if (APPLE OR LINUX)
	set(ENTRY_POINT_FILE main_unix.cpp)
else()
	set(ENTRY_POINT_FILE main_windows.cpp)
endif()

include_directories(
	${CMAKE_SOURCE_DIR}
    src/
)

if (SHOULD_BUNDLE_APP)
    if (APPLE)
        set(MACOSX_BUNDLE_ICON_FILE app-icon.icns)
        set(APP_ICON_PATH ${CMAKE_CURRENT_SOURCE_DIR}/resources/app-icon.icns)
        set_source_files_properties(${APP_ICON_PATH} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

        set(PLATFORM_BUNDLING MACOSX_BUNDLE ${APP_ICON_PATH})

    elseif(MSVC)
        set(APP_ICON_DIR ${CMAKE_CURRENT_SOURCE_DIR}/resources)
        set(APP_ICON_PATH ${APP_ICON_DIR}/app-icon.rc)
        set(PLATFORM_BUNDLING ${APP_ICON_DIR} ${APP_ICON_PATH})

    else()
        set(PLATFORM_BUNDLING )
    endif()
endif()

add_subdirectory(src)

add_library(
	${TARGET_NAME}
    ${PLATFORM_BUNDLING}

    ${MONOCHROME_HEADERS} ${MONOCHROME_SOURCES}
)

if (APPLE)
    find_library(COCOA_LIBRARY Cocoa)

    target_link_libraries(${TARGET_NAME} "-framework Cocoa")
    add_definitions("-x objective-c++")

    if (SHOULD_BUNDLE_APP)
        set_target_properties(${TARGET_NAME} PROPERTIES
            MACOSX_BUNDLE True
            MACOSX_BUNDLE_GUI_IDENTIFIER cpp.app.${TARGET_NAME}
            MACOSX_BUNDLE_BUNDLE_NAME ${TARGET_NAME}
            MACOSX_BUNDLE_BUNDLE_VERSION "1.0.0"
            MACOSX_BUNDLE_SHORT_VERSION_STRING "0.1"
        )
    endif()

elseif (LINUX)
    target_link_libraries(${TARGET_NAME} dl)

elseif(MSVC)
    target_link_libraries(${TARGET_NAME} d3d11.lib)
    
endif()

add_subdirectory(tests)
