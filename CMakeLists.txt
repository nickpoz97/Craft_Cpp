cmake_minimum_required(VERSION 3.19)
project(cpp)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCE_FILES src/*.cpp deps/noise/noise.c stc/*.h)

include_directories(deps/glad/include/)
include_directories(deps/glfw/include/)
include_directories(deps/glm)
include_directories(deps/noise/)
include_directories(deps/stbi/)

add_executable(app ${SOURCE_FILES} src/UniformsWrapper.hpp src/GameObject.cpp src/Item.cpp src/Item.hpp src/ActionHandler.cpp src/ActionHandler.hpp deps/stbi/stb_image.h)
