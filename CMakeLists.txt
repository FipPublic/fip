cmake_minimum_required(VERSION 3.28)
project(fip)

set(CMAKE_CXX_STANDARD 14)

add_library(tcp
        lib/tcp/tcp_linux.cpp
        lib/tcp/tcp_windows.cpp
        lib/tcp/tcp_mac.cpp
        lib/tcp/tcp.h
)


add_executable(fip main.cpp)

add_executable(example_tcp_server example/tcp_server/main.cpp)