cmake_minimum_required(VERSION 3.19)
project(cpp)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCE_FILES src/*.cpp deps/noise/noise.c stc/*.h)

include_directories(deps/glad/include/)
include_directories(deps/glfw/include/)
include_directories(deps/glm)
include_directories(deps/noise/)

add_executable(app ${SOURCE_FILES} src/UniformsWrapper.hpp)
