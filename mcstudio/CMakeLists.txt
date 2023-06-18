cmake_minimum_required(VERSION 3.0)

add_executable(
    mcstudio

    main.cpp
)
target_link_libraries(mcstudio PUBLIC Monochrome)
