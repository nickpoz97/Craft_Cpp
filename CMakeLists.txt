cmake_minimum_required(VERSION 3.19)
project(cpp)

set(CMAKE_CXX_STANDARD 17)

file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

file(GLOB GEOMETRY_FILES src/Geometry/*.cpp)
file(GLOB INTERACTION_FILES src/Interaction/*.cpp)
file(GLOB RENDERING_FILES src/Rendering/*.cpp deps/glad/src/*.c)

file(GLOB SOURCE_FILES src/*.cpp ${GEOMETRY_FILES} ${INTERACTION_FILES} ${RENDERING_FILES}
        deps/noise/noise.cpp deps/glad/src/*.c)

add_executable(app ${SOURCE_FILES} src/Interaction/GLError.cpp src/Interaction/GLError.hpp)
add_executable(rendering_test test/rendering_test.cpp
        ${INTERACTION_FILES} ${RENDERING_FILES} ${GEOMETRY_FILES}
        ./src/map_utils.cpp ./deps/noise/noise.cpp)

set_target_properties(app PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
set_target_properties(rendering_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/test")

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
target_link_libraries(rendering_test ${CMAKE_DL_LIBS} glfw)
