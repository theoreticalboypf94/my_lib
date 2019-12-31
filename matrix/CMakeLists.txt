
project(alloc_test C)

set(CMAKE_C_STANDARD 11)

add_executable(alloc_test main.c Matrix.c Matrix.h Matrix_functions.c Matrix_functions.h)