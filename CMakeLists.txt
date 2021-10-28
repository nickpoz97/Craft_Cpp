cmake_minimum_required(VERSION 3.19)
project(CppCraft)
enable_testing()

set(CMAKE_CXX_STANDARD 17)
file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

set(CMAKE_CXX_FLAGS_SANITIZER "${CMAKE_CXX_FLAGS_DEBUG} -O1 -fsanitize=address -fno-omit-frame-pointer")
set(CMAKE_EXE_LINKER_FLAGS_SANITIZER "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer")

#add_compile_options(-pg)
#add_link_options(-pg)
message(STATUS "Current path: ${CMAKE_CURRENT_BINARY_DIR}}")

add_subdirectory(deps/glfw)
add_subdirectory(deps/fmt)

link_libraries(${CMAKE_DL_LIBS} glfw fmt)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

file(GLOB_RECURSE INTERNAL_SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
set(EXTERNAL_SOURCES deps/noise/noise.cpp deps/glad/src/glad.c)

set(SOURCE_FILES ${INTERNAL_SOURCES} ${EXTERNAL_SOURCES})
add_executable(app ${SOURCE_FILES})

file(GLOB CATCH_TEST_FILES ${PROJECT_SOURCE_DIR}/test/*.cpp)

set(TEST_MAIN test/main_test.cpp)

add_executable(catch_test ${SOURCE_FILES} ./deps/noise/noise.cpp
        ${TEST_MAIN})

include_directories(deps/glad/include/)
include_directories(deps/glfw/include/)
include_directories(deps/glm/)
include_directories(deps/noise/)
include_directories(deps/stbi/)
include_directories(deps/fmt/include/)
include_directories(deps/catch2)
include_directories(src)

add_test(NAME catch_test COMMAND "./catch_test" WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
