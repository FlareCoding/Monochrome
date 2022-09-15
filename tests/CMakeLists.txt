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

add_executable(Image_Demo image_demo.cpp)
target_link_libraries(Image_Demo PUBLIC Monochrome)