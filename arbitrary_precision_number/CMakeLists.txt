cmake_minimum_required(VERSION 3.14)
project(arbitrary_precision_number C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_FLAGS "-std=c11")
add_executable(arbitrary_precision_number main.c number.c )