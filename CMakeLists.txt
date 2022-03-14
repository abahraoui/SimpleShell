cmake_minimum_required(VERSION 3.20)
project(Shell7 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Shell7
        main.c shell.c history.c functions.c)
