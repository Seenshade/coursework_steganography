cmake_minimum_required(VERSION 3.17)
project(coursework)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(coursework main.cpp wav.cpp)