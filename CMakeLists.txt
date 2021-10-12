cmake_minimum_required(VERSION 3.19)
project(cpp)

set(CMAKE_CXX_STANDARD 17)
file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

add_compile_options(-pg)
add_link_options(-pg)

#set(CMAKE_BUILD_TYPE Release)
#add_compile_definitions(DEBUG)

add_subdirectory(deps/glfw)
add_subdirectory(deps/fmt)

link_libraries(${CMAKE_DL_LIBS} glfw fmt)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

file(GLOB GEOMETRY_FILES src/Geometry/*.cpp)
set(INTERACTION_FILES src/Interaction/CameraControl.cpp
        src/Interaction/Camera.cpp
        src/Interaction/GameView.cpp
        src/Interaction/Scene.cpp
)
file(GLOB RENDERING_FILES src/Rendering/*.cpp deps/glad/src/*.c)
file(GLOB CATCH_TEST_FILES ${PROJECT_SOURCE_DIR}/test/*.cpp)
file(GLOB RENDERING_TEST_FILES ${PROJECT_SOURCE_DIR}/rendering_test/*)
set(SOURCE_FILES ${GEOMETRY_FILES} ${INTERACTION_FILES} ${RENDERING_FILES} deps/noise/noise.cpp)
set(MAIN src/App.cpp)

add_executable(app ${SOURCE_FILES} ${MAIN})
#add_executable(rendering_test ${SOURCE_FILES} rendering_test/crosshair_test.cpp)
#add_executable(item_rendering ${SOURCE_FILES} rendering_test/item_rendering.cpp)
add_executable(chunk_rendering ${SOURCE_FILES} rendering_test/chunk_rendering.cpp)

#target_compile_options(chunk_rendering PUBLIC ${CMAKE_CXX_FLAGS})
#target_link_options(chunk_rendering PUBLIC ${CMAKE_CXX_FLAGS})

set(TEST_MAIN test/main_test.cpp)

#add_executable(player_catch ${INTERACTION_FILES} ${RENDERING_FILES} ${GEOMETRY_FILES} ./deps/noise/noise.cpp
#        ${TEST_MAIN} test/player_test.cpp)
#add_executable(tileblock_test ${INTERACTION_FILES} ${RENDERING_FILES} ${GEOMETRY_FILES} ./deps/noise/noise.cpp
#        ${TEST_MAIN} test/tileblock_test.cpp)
add_executable(catch_test ${INTERACTION_FILES} ${RENDERING_FILES} ${GEOMETRY_FILES} ./deps/noise/noise.cpp
        ${TEST_MAIN} test/player_test.cpp test/tileblock_test.cpp test/cubic_object_test.cpp test/chunk_test.cpp)

include_directories(deps/glad/include/)
include_directories(deps/glfw/include/)
include_directories(deps/glm/)
include_directories(deps/noise/)
include_directories(deps/stbi/)
include_directories(deps/fmt/include/)
include_directories(deps/catch2)
include_directories(src)

enable_testing()
#add_test(NAME player_catch COMMAND ./player_catch WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
#add_test(NAME tileblock_catch COMMAND ./tileblock_test WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
add_test(NAME catch_test COMMAND "./catch_test" WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
#add_test(NAME tileblock_catch COMMAND "./catch_test WORKING_DIRECTORY" ${PROJECT_SOURCE_DIR}/bin)
