cmake_minimum_required(VERSION 3.10)
project(cpp)

set(CMAKE_CXX_STANDARD 17)

add_executable(cpp main.cpp kernel/kernel.cpp kernel/kernel.h UI/menu.cpp UI/menu.h kernel/Objects.cpp kernel/Objects.h)
