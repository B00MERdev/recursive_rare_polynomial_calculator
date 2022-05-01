cmake_minimum_required(VERSION 3.19)
project(poprawka_duze_zadanie C)

set(CMAKE_C_STANDARD 11)

add_executable(poprawka_duze_zadanie poly.h poly.c calc.c calc.h input-output.c input-output.h data_structures.c data_structures.h poly_test.c)