cmake_minimum_required(VERSION 3.29)
project(snowfall)

# Use CMake policies
cmake_policy(SET CMP0074 NEW)

set(CMAKE_CXX_STANDARD 20)

# Dependencies
include(FetchContent)

set(RAYLIB_VERSION 5.0)
FetchContent_Declare(
        raylib
        URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
        FIND_PACKAGE_ARGS ${RAYLIB_VERSION} EXACT
)

set(BUILD_EXAMPLES OFF CACHE INTERNAL "Disable raylib examples")

FetchContent_MakeAvailable(raylib)

add_executable(snowfall src/main.cpp
        src/sim.cpp
        src/sim.h
        src/grid.h
        src/render.cpp
        src/render.h)

# Link libraries
target_link_libraries(snowfall raylib)

# Include directories
target_include_directories(snowfall PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Find and link OpenGL
find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
    target_link_libraries(snowfall ${OPENGL_LIBRARIES})
endif()
