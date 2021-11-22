cmake_minimum_required(VERSION 3.17)
project(CppCraft)
enable_testing()

set(CMAKE_CXX_STANDARD 17)
file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

if (${UNIX})
    find_package(X11 REQUIRED)

    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g --coverage")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -g --coverage")

    set(CMAKE_CXX_FLAGS_SANITIZER "${CMAKE_CXX_FLAGS_DEBUG} -O1 -fsanitize=address -fno-omit-frame-pointer")
    set(CMAKE_EXE_LINKER_FLAGS_SANITIZER "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer")

    add_custom_target(
            valgrind
            COMMAND valgrind
            --leak-check=full
            --show-leak-kinds=all
            --log-file=${PROJECT_SOURCE_DIR}/doc/valgrind_out.txt
            ${PROJECT_SOURCE_DIR}/bin/app
    )

    set(COVERAGE_SCRIPT "${PROJECT_SOURCE_DIR}/script/coverage.sh")
    message("Coverage script path: ${COVERAGE_SCRIPT}")
    add_custom_target(coverage COMMAND ${COVERAGE_SCRIPT} $(PWD) ${PROJECT_SOURCE_DIR})
endif()

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(deps/glfw)
add_subdirectory(deps/fmt)

link_libraries(${CMAKE_DL_LIBS} glfw fmt)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
file(GLOB_RECURSE INTERNAL_SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
set(APP_MAIN "${PROJECT_SOURCE_DIR}/src/App.cpp")
list(FILTER INTERNAL_SOURCES EXCLUDE REGEX "${APP_MAIN}")
set(EXTERNAL_SOURCES deps/noise/noise.cpp deps/glad/src/glad.c)
set(SOURCE_FILES ${INTERNAL_SOURCES} ${EXTERNAL_SOURCES})

add_executable(app ${APP_MAIN} ${SOURCE_FILES})

file(GLOB CATCH_TEST_FILES ${PROJECT_SOURCE_DIR}/test/*.cpp)

file(GLOB_RECURSE TEST_FILES test/*.cpp)

add_executable(catch_test ${SOURCE_FILES} ${TEST_FILES})

set(INCLUDE_DIRS deps/ include/)

include_directories(${INCLUDE_DIRS})

add_test(NAME test_and_coverage COMMAND catch_test WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(DOXYGEN_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/doc/doxygen)
    set(DOXYGEN_EXTRACT_STATIC YES)
    set(DOXYGEN_EXTRACT_ALL YES)
    doxygen_add_docs(doc include ALL)
endif()
