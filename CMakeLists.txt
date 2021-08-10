cmake_minimum_required(VERSION 3.19)
project(cpp)

set(CMAKE_CXX_STANDARD 17)

add_executable(app src/cube.cpp src/main.cpp src/Vec.cpp src/Vec.hpp src/item.hpp src/cube.hpp src/Plant.cpp src/Plant.hpp src/Block.cpp src/Block.hpp src/Matrix.cpp src/Matrix.hpp src/GameObject.cpp src/GameObject.hpp src/Player.cpp src/Player.hpp src/Sphere.cpp src/Sphere.hpp src/Vertex.cpp src/Vertex.hpp src/World.cpp src/World.hpp src/BlockMap.cpp src/BlockMap.hpp src/Chunk.cpp src/Chunk.hpp src/Sign.cpp src/Sign.hpp src/Worker.cpp src/Worker.hpp src/Model.cpp src/Model.hpp)

include_directories(deps/glew/include)
include_directories(deps/glfw/include)
