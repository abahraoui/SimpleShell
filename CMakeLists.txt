cmake_minimum_required(VERSION 3.20)
project(cs210_simple_amogus_shell C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(cs210_simple_amogus_shell
        main.c shell.c)
