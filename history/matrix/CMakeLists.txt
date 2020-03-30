project(Matrix_prj)

set(CMAKE_C_STANDARD 11)

add_executable(Matrix_prj main.c Matrix.c Matrix.h Matrix_functions.c Matrix_functions.h)

target_link_libraries(Matrix_prj m) # подключение математической библиотеки
