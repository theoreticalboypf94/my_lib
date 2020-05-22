cmake_minimum_required(VERSION 3.14)
project(AVL_tree C)

set(CMAKE_C_STANDARD 11)

add_executable(AVL_tree main.c AVL_tree.c AVL_tree.h)