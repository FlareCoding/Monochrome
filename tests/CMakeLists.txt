cmake_minimum_required(VERSION 3.0)

add_executable(Demo demo.cpp)
target_link_libraries(Demo PUBLIC Monochrome)

add_executable(DockPanel_Demo dockpanel_demo.cpp)
target_link_libraries(DockPanel_Demo PUBLIC Monochrome)

add_executable(TreeView_Demo treeview_demo.cpp)
target_link_libraries(TreeView_Demo PUBLIC Monochrome)

add_executable(McxApp_Demo
    mcxapp_demo.cpp
    mcxapp_demo.mcx
    mcxapp_custom_panel.mcx
)
target_link_libraries(McxApp_Demo PUBLIC Monochrome)

add_custom_command(
    TARGET McxApp_Demo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/tests/mcxapp_demo.mcx
        ${CMAKE_SOURCE_DIR}/tests/mcxapp_custom_panel.mcx

        $<TARGET_FILE_DIR:McxApp_Demo>/
    COMMENT "Copying .mcx files"
)

add_executable(Image_Demo image_demo.cpp)
target_link_libraries(Image_Demo PUBLIC Monochrome)

add_executable(Splitter_Demo
    splitter_demo.cpp
    splitterpanel_example1.mcx
    splitterpanel_example2.mcx
    splitterpanel_example3.mcx
    splitterpanel_example4.mcx
)
target_link_libraries(Splitter_Demo PUBLIC Monochrome)

add_custom_command(
    TARGET Splitter_Demo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/tests/splitterpanel_example1.mcx
        ${CMAKE_SOURCE_DIR}/tests/splitterpanel_example2.mcx
        ${CMAKE_SOURCE_DIR}/tests/splitterpanel_example3.mcx
        ${CMAKE_SOURCE_DIR}/tests/splitterpanel_example4.mcx

        $<TARGET_FILE_DIR:Splitter_Demo>/
    COMMENT "Copying .mcx files"
)

add_executable(TabView_Demo
    tabview_demo.cpp
    tabview_demo.mcx
)
target_link_libraries(TabView_Demo PUBLIC Monochrome)

add_custom_command(
    TARGET TabView_Demo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/tests/tabview_demo.mcx

        $<TARGET_FILE_DIR:TabView_Demo>/
    COMMENT "Copying .mcx files"
)
