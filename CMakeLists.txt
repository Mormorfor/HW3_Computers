cmake_minimum_required(VERSION 3.21)
project(lab C)

set(CMAKE_C_STANDARD 11)
add_compile_options(-Wall -g)

add_executable(lab driver.c clock_windows.c fcyc.c kernels.c)