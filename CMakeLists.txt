cmake_minimum_required(VERSION 3.19)
project(cpp)

set(CMAKE_CXX_STANDARD 17)

add_executable(app src/cube.cpp src/main.cpp src/data_structures.cpp src/data_structures.hpp src/item.hpp src/cube.hpp src/Plant.cpp src/Plant.hpp)
#add_executable(src/Cube.cpp)