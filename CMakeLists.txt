cmake_minimum_required(VERSION 3.19)
project(cpp)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCE_FILES src/*.cpp)

include_directories(deps/glad/include/)
include_directories(deps/glfw/include/)
include_directories(deps/glm)

add_executable(app ${SOURCE_FILES} src/frustum.cpp src/frustum.hpp)
