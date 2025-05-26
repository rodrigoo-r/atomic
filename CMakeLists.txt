cmake_minimum_required(VERSION 3.30)
project(atomic C)

set(CMAKE_C_STANDARD 11)

add_library(atomic STATIC include/fluent/atomic/atomic.c)
