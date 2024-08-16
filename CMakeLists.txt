cmake_minimum_required(VERSION 3.28)
project(fip)

set(CMAKE_CXX_STANDARD 14)

add_library(tcp ${PLATFORM_SOURCES})
add_executable(fip main.cpp)

if (WIN32)
    File(GLOB PLATFORM_SOURCES *_windows.cpp)
elseif (LINUX)
    file(GLOB PLATFORM_SOURCES *_linux.cpp)
elseif (MAC)
    file(GLOB PLATFORM_SOURCES *_mac.cpp)
endif()
