cmake_minimum_required(VERSION 3.10)

# set the project name
project(Http)

add_executable(Http server_linux.cpp http_tcpserver_linux.cpp)
target_include_directories(Http PRIVATE .)

# target_link_libraries(Http libwsock32.a)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)