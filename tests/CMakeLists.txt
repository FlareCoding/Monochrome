cmake_minimum_required(VERSION 3.0)

add_executable(Demo demo.cpp)
target_link_libraries(Demo PUBLIC Monochrome)

add_executable(MultipleWindows multiple_windows.cpp)
target_link_libraries(MultipleWindows PUBLIC Monochrome)

add_executable(WindowControl window_control.cpp)
target_link_libraries(WindowControl PUBLIC Monochrome)

add_executable(FileChooserTest file_chooser_test.cpp)
target_link_libraries(FileChooserTest PUBLIC Monochrome)
