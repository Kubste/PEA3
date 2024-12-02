cmake_minimum_required(VERSION 3.28)
project(PEAProjekt3)

set(CMAKE_CXX_STANDARD 17)

add_executable(PEAProjekt3
        File_manager.hpp
        File_manager.cpp
        Main.cpp
        TSP.cpp)

set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -std=c++11")
