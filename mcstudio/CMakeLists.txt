cmake_minimum_required(VERSION 3.0)

add_executable(
    mcstudio

    main.cpp
    GlobalResgitrationSystem.h
    GlobalResgitrationSystem.cpp
    McxSourceManager.h
    McxSourceManager.cpp
    Editor.h
    Editor.cpp

    controllers/WidgetTreeController.h
    controllers/WidgetTreeController.cpp
    controllers/EditorCanvasController.h
    controllers/EditorCanvasController.cpp
    controllers/WidgetPropertiesPanelController.h
    controllers/WidgetPropertiesPanelController.cpp

    user_widgets/BaseWidgetPropertiesPanel.h
    user_widgets/BaseWidgetPropertiesPanel.cpp
    user_widgets/RegularPropertyTextField.h
    user_widgets/RegularPropertyTextField.cpp
    user_widgets/BooleanPropertyField.h
    user_widgets/BooleanPropertyField.cpp
    user_widgets/DockAnchorPropertyField.h
    user_widgets/DockAnchorPropertyField.cpp
    user_widgets/AlignmentPropertyField.h
    user_widgets/AlignmentPropertyField.cpp

    mcx/main.mcx
    mcx/editor-panel.mcx
    mcx/widget-toolbox.mcx
    mcx/widget-tree-view.mcx
    mcx/widget-properties-panel.mcx

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

add_custom_command(
    TARGET mcstudio POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/mcstudio/icons
        $<TARGET_FILE_DIR:mcstudio>/icons
    COMMENT "Copying icon files"
)
