cmake_minimum_required(VERSION 3.0)

add_executable(MessengerDemo main.cpp tcp_friend/tcp_friend.cpp messenger_window.mcx)
target_link_libraries(MessengerDemo PUBLIC Monochrome)

add_executable(MessengerDemoServer server.cpp tcp_friend/tcp_friend.cpp)

add_custom_command(
        TARGET MessengerDemo POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/messenger-demo/messenger_window.mcx
                ${CMAKE_CURRENT_BINARY_DIR}/messenger_window.mcx)