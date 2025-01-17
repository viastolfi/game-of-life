cmake_minimum_required(VERSION 3.20)

project(my_game)

set(CMAKE_CXX_STANDARD 20)
set(SOURCES src/main.c)

find_package(SDL2 REQUIRED COMPONENTS SDL2)
add_executable(gol ${SOURCES})
target_link_libraries(gol PRIVATE SDL2::SDL2)
