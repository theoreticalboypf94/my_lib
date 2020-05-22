cmake_minimum_required(VERSION 3.14)
project(Number C)

set(CMAKE_C_STANDARD 11)

add_executable(Number main.c Number.c Number.h)