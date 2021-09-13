cmake_minimum_required(VERSION 3.19)
project(cpp)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCE_FILES src/*.cpp deps/noise/*.c deps/glad/src/*.c)

include_directories(deps/glad/include/)
include_directories(deps/glfw/include/)
include_directories(deps/glm)
include_directories(deps/noise/)
include_directories(deps/stbi/)
include_directories(deps/fmt/include/)

add_executable(app ${SOURCE_FILES})
add_subdirectory(deps/glfw)
add_subdirectory(deps/fmt)
target_link_libraries(app ${CMAKE_DL_LIBS} glfw fmt)
