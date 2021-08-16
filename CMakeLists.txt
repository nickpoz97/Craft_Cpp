cmake_minimum_required(VERSION 3.19)
project(cpp)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCE_FILES src/*.cpp)

include_directories(deps/glad/include/)
include_directories(deps/glfw/include/)
include_directories(deps/glm)

add_executable(app ${SOURCE_FILES} src/OpenglBuffer.cpp src/OpenglBuffer.hpp src/CubeWireframe.cpp src/CubeWireframe.hpp src/GameObject.hpp src/Character.cpp src/Character.hpp src/Text.cpp src/Text.hpp src/Attrib.hpp src/Character3D.cpp src/Character3D.hpp src/Attrib.cpp src/costants.hpp)
