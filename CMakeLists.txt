cmake_minimum_required(VERSION 3.16.3)
project(Strassen C)

set(CMAKE_C_STANDARD 11)

add_executable(Strassen main.c
        strassen.c
        strassen.h)
