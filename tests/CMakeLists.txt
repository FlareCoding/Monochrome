cmake_minimum_required(VERSION 3.0)

add_executable(Demo demo.cpp)
target_link_libraries(Demo PUBLIC Monochrome)

add_executable(UITabViewTest uitabview_test.cpp)
target_link_libraries(UITabViewTest PUBLIC Monochrome)

add_executable(WindowControlTest window_control_test.cpp)
target_link_libraries(WindowControlTest PUBLIC Monochrome)

add_executable(WindowEventsTest window_events_test.cpp)
target_link_libraries(WindowEventsTest PUBLIC Monochrome)

add_executable(PerformanceTest performance_test.cpp)
target_link_libraries(PerformanceTest PUBLIC Monochrome)

add_executable(MenuBarTest menu_bar_test.cpp)
target_link_libraries(MenuBarTest PUBLIC Monochrome)

add_executable(UIImageTest uiimage_test.cpp)
target_link_libraries(UIImageTest PUBLIC Monochrome)

add_executable(FileChooserTest file_chooser_test.cpp)
target_link_libraries(FileChooserTest PUBLIC Monochrome)

add_executable(MultipleWindowsTest multiple_windows_test.cpp)
target_link_libraries(MultipleWindowsTest PUBLIC Monochrome)

add_executable(AnimationsTest animations_test.cpp)
target_link_libraries(AnimationsTest PUBLIC Monochrome)

add_executable(DockingDemo docking_demo.cpp)
target_link_libraries(DockingDemo PUBLIC Monochrome)
