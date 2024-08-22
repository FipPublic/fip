cmake_minimum_required(VERSION 3.28)
project(fip)

set(CMAKE_CXX_STANDARD 14)

# 平台lib添加函数
function(add_platform_library LIB_NAME LIB_PATH)
    if(WIN32)
        FILE(GLOB PLATFORM_SOURCES ${LIB_PATH}/*_windows.cpp)
    elseif(UNIX)
        FILE(GLOB PLATFORM_SOURCES ${LIB_PATH}/*_linux.cpp)
    elseif(APPLE)
        FILE(GLOB PLATFORM_SOURCES ${LIB_PATH}/*_mac.cpp)
    endif()

    add_library(${LIB_NAME} ${PLATFORM_SOURCES})
endfunction()

# 依赖库
add_platform_library(tcp lib/tcp)

# 服务器入口
add_executable(fip main.cpp)


# tcp server example
add_executable(example_tcp_server example/tcp_server/main.cpp)
target_link_libraries(example_tcp_server tcp)
