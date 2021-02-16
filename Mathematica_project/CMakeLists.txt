cmake_minimum_required(VERSION 3.14)
project(Mathematica_project)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-fpermissive)

add_executable(Mathematica_project Vector_implementation.h main.cpp )

#fpermissive