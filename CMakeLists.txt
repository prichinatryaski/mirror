cmake_minimum_required(VERSION 3.10)
project(TCPServer)

set(CMAKE_C_STANDARD 99)

find_library(EV_LIBRARY NAMES ev)
find_path(EV_INCLUDE_DIR NAMES ev.h)

include_directories(${EV_INCLUDE_DIR})

add_executable(tcp_server server.c)
target_link_libraries(tcp_server ${EV_LIBRARY})