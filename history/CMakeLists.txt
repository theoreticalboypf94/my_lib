cmake_minimum_required(VERSION 3.10)
project(Furier_transform C)

set(CMAKE_C_STANDARD 11)
set (CMAKE_C_FLAGS "-lm")

add_executable(Furier_transform main.c integratef/integratef.c integratef/integratef.h fourier_series/fs.c fourier_series/fs.h interpolation/interpolate.c interpolation/interpolate.h numerical_series/count_series.c numerical_series/count_series.h bpm/tmp_bpm.c bpm/graph_in_bpm.c bpm/graph_in_bpm.h ode/ode_RK_solutor.c ode/ode_RK_solutor.h)
target_link_libraries(Furier_transform m)