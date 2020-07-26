cmake_minimum_required(VERSION 3.0)

add_executable(Demo demo.cpp)
target_link_libraries(Demo PUBLIC Monochrome)

add_executable(MultipleWindows multiple_windows.cpp)
target_link_libraries(MultipleWindows PUBLIC Monochrome)
