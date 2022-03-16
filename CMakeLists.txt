cmake_minimum_required(VERSION 3.20)
project(cs210-simple-amogus-shell C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(cs210-simple-amogus-shell
        main.c shell.c history.c functions.c)
