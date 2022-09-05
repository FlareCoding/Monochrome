cmake_minimum_required(VERSION 3.0)

add_executable(Demo demo.cpp)
target_link_libraries(Demo PUBLIC Monochrome)

add_executable(DockPanel_Demo dockpanel_demo.cpp)
target_link_libraries(DockPanel_Demo PUBLIC Monochrome)