cmake_minimum_required(VERSION 3.19)
project(cpp)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCE_FILES src/*.cpp deps/noise/noise.c src/*.h)

include_directories(deps/glad/include/)
include_directories(deps/glfw/include/)
include_directories(deps/glm)
include_directories(deps/noise/)
#include_directories(deps/stbi/)
#include_directories(deps/fmt/include/)

add_executable(app src/test_main.cpp src/TileBlock.cpp src/RenderableEntity.cpp src/OpenglBuffer.cpp deps/glad/src/glad.c
        src/CubicObject.cpp src/Text2D.cpp src/Character.cpp src/Sphere.cpp src/Item.cpp
        src/Chunk.cpp deps/noise/noise.c src/BlockMap.cpp)
add_subdirectory(deps/glfw)
target_link_libraries(app ${CMAKE_DL_LIBS} glfw)
