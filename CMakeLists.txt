cmake_minimum_required(VERSION 3.14)
project(chip8)

set(CMAKE_CXX_STANDARD 11)

find_package(SDL2 REQUIRED)

add_executable( chip8 Chip8.cpp main.cpp Platform.cpp)

target_compile_options(chip8 PRIVATE -Wall)

target_link_libraries(chip8 SDL2main SDL2)
