cmake_minimum_required(VERSION 3.20)
project(Conwoy_s_game_of_life C)

set(CMAKE_C_STANDARD 99)

add_executable(Conwoy_s_game_of_life main.c Conway_life.c ConwayFunc.h)