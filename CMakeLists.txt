cmake_minimum_required(VERSION 3.10)
project(Furier_transform C)

set(CMAKE_C_STANDARD 11)
set (CMAKE_C_FLAGS "-lm")

add_executable(Furier_transform main.c integratef/integratef.c integratef/integratef.h fourier_series/fs.c fourier_series/fs.h interpolation/interpolate.c interpolation/interpolate.h)
target_link_libraries(Furier_transform m)