cmake_minimum_required(VERSION 3.17)
project(CppCraft)
enable_testing()
set(CMAKE_CXX_STANDARD 17)

### build and link external dependencies ###
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(deps/glfw)
add_subdirectory(deps/fmt)
link_libraries(${CMAKE_DL_LIBS} glfw fmt)

### collect list of source files ###
file(GLOB_RECURSE INTERNAL_SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
set(APP_MAIN "${PROJECT_SOURCE_DIR}/src/App.cpp")
list(FILTER INTERNAL_SOURCES EXCLUDE REGEX "${APP_MAIN}")
set(EXTERNAL_SOURCES deps/noise/noise.cpp deps/glad/src/glad.c)
set(SOURCE_FILES ${INTERNAL_SOURCES} ${EXTERNAL_SOURCES})

### collect list of header files ###
set(INCLUDE_DIRS deps/ include/)
include_directories(${INCLUDE_DIRS})

### set app target ###
add_executable(app ${APP_MAIN} ${SOURCE_FILES})

### collect test sources and set test ###
file(GLOB_RECURSE TEST_FILES test/*.cpp)
add_executable(catch_test ${SOURCE_FILES} ${TEST_FILES})
add_test(NAME test COMMAND catch_test WORKING_DIRECTORY ${PROJECT_BINARY_DIR})

if (${UNIX})
    ### compile internal sources with warning flags ###
    set_source_files_properties(
            ${INTERNAL_SOURCES}
            PROPERTIES
            COMPILE_FLAGS "-Wall -Wextra -Werror -Wpedantic"
    )

    ### needed for GLFW ###
    find_package(X11 REQUIRED)

    ### set flags for specific build type ###
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g --coverage")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -g --coverage")

    set(CMAKE_CXX_FLAGS_ADDRESS_SANITIZER "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer")
    set(CMAKE_EXE_LINKER_FLAGS_ADDRESS_SANITIZER "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer")

    set(CMAKE_CXX_FLAGS_UNDEFINED_SANITIZER "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=undefined -fno-omit-frame-pointer")
    set(CMAKE_EXE_LINKER_FLAGS_UNDEFINED_SANITIZER "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=undefined -fno-omit-frame-pointer")

    ### execute demo with valgrind and print log in file ###
    add_custom_target(
            valgrind
            COMMAND valgrind
            --leak-check=full
            --show-leak-kinds=all
            --track-fds=yes
            --track-origins=yes
            --log-file=${PROJECT_SOURCE_DIR}/doc/valgrind_out.txt
            ${PROJECT_BINARY_DIR}/app
    )
endif()

### set coverage target ###
set(COVERAGE_DIR ${PROJECT_SOURCE_DIR}/doc/coverage)
add_custom_target(
        coverage
        COMMAND mkdir -p ${COVERAGE_DIR}
        COMMAND gcovr -r ${PROJECT_SOURCE_DIR}/src
        -e ${PROJECT_SOURCE_DIR}/src/App.cpp -e ${PROJECT_SOURCE_DIR}/src/Interaction/Scene.cpp
        -e ${PROJECT_SOURCE_DIR}/src/Geometry/Sphere.cpp -e ${PROJECT_SOURCE_DIR}/src/Geometry/Item.cpp
        ${PROJECT_BINARY_DIR} --html-details ${COVERAGE_DIR}/coverage.html
)

### set doxygen target ###
find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(DOXYGEN_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/doc/doxygen)
    set(DOXYGEN_EXTRACT_STATIC YES)
    set(DOXYGEN_EXTRACT_ALL YES)
    set(DOXYGEN_GENERATE_LATEX NO)
    doxygen_add_docs(doc include ALL)
endif()

### set absolute paths for texture and shaders dir ###
set(TEXTURES_DIR "${PROJECT_SOURCE_DIR}/data/textures/")
set(SHADERS_DIR "${PROJECT_SOURCE_DIR}/data/shaders/")
add_definitions( -DSHADERS_DIR=${SHADERS_DIR} )
add_definitions( -DTEXTURES_DIR=${TEXTURES_DIR} )
configure_file( ${PROJECT_SOURCE_DIR}/src/Paths.hpp.in ${PROJECT_SOURCE_DIR}/src/Paths.hpp)

### install rules ###
install(TARGETS app CONFIGURATIONS Release RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin/Release)
install(TARGETS app CONFIGURATIONS Debug RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin/Debug)
install(TARGETS app CONFIGURATIONS Undefined_Sanitizer RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin/UndSan)
install(TARGETS app CONFIGURATIONS Address_Sanitizer RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin/AddSan)