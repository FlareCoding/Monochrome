cmake_minimum_required(VERSION 3.0)

add_executable(
    mcstudio

    main.cpp
    GlobalResgitrationSystem.h
    GlobalResgitrationSystem.cpp
    McxSourceManager.h
    McxSourceManager.cpp

    mcx/main.mcx
    mcx/editor-panel.mcx
    mcx/widget-toolbox.mcx
    mcx/widget-tree-view.mcx

    mcx/stylesheet.mcx
)
target_link_libraries(mcstudio PUBLIC Monochrome)

add_custom_command(
    TARGET mcstudio POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/mcstudio/mcx
        $<TARGET_FILE_DIR:mcstudio>/mcx
    COMMENT "Copying .mcx files"
)
