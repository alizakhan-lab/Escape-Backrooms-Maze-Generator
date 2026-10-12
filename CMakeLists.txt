cmake_minimum_required(VERSION 3.16)
project(backrooms_maze_generator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(maze_demo main.cpp maze.h)
