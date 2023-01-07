cmake_minimum_required(VERSION 3.12)

project(advent-of-code CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O3")

add_executable(advent-of-code
        "main.cpp"
        "get_file_content.cpp"
        Day_17/day_seventeen.cpp)