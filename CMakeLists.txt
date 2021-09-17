cmake_minimum_required(VERSION 3.19)
project(cpp)

set(CMAKE_CXX_STANDARD 17)

file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

file(GLOB SOURCE_FILES src/*.cpp deps/noise/noise.cpp deps/glad/src/*.c)
#file(GLOB SOURCE_FILES deps/noise/noise.c)

add_executable(app ${SOURCE_FILES} src/GLError.cpp src/GLError.hpp)
set_target_properties(app PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

include_directories(deps/glad/include/)
include_directories(deps/glfw/include/)
include_directories(deps/glm)
include_directories(deps/noise/)
include_directories(deps/stbi/)
include_directories(deps/fmt/include/)
include_directories(deps/catch2)
add_subdirectory(deps/glfw)
add_subdirectory(deps/fmt)

target_link_libraries(app ${CMAKE_DL_LIBS} glfw fmt)
