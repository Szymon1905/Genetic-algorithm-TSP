cmake_minimum_required(VERSION 3.26)
project(PEA_genetyczny)

set(CMAKE_CXX_STANDARD 23)

add_executable(PEA_genetyczny main.cpp
        genetyczny.cpp
        genetyczny.h
        )
