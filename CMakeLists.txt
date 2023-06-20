cmake_minimum_required(VERSION 3.0)
set(CMAKE_CONFIGURATION_TYPES "Debug;Release")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_definitions(-DUNICODE -D_UNICODE -D_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING)

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

if (MSVC)
    # Enable extremely aggressive optimization for Win32 Release builds:
	# - Runtime library: Multi-threaded (/MT) as opposed to default 'Multi-threaded DLL' - static runtime library allows better LTCG inlining opportunities.
	# - Optimization: Full Optimization (/Ox)
	# - Inline Function Expansion: Any Suitable (/Ob2)
	# - Enable Intrinsic Functions: Yes (/Oi)
	# - Omit Frame Pointers: Yes (/Oy)
	# - Favor Size Or Speed: Favor fast code (/Ot)
	# - Enable Fiber-Safe Optimizations: Yes (/GT)
	# - Enable String Pooling: Yes (/GF)
	# - Buffer Security Check: No (/GS-)
    # - Whole Program Optimization: Yes (/GL)
	# - Build with Multiple Processes (/MP)
	set(OPTFLAGS "/O2 /Ob2 /Oi /Oy /Ot /GT /GF /GS- /GL /MT /MP")

    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} ${OPTFLAGS}")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${OPTFLAGS}")

    SET(CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_MODULE_LINKER_FLAGS_RELEASE} /LTCG")
    SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /LTCG")
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
add_subdirectory(monodev)
add_subdirectory(mcstudio)
add_subdirectory(messenger-demo)
