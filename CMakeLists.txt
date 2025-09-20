cmake_minimum_required(VERSION 3.10)
project(snake_binding)

# Enable PYBIND11_FINDPYTHON to use modern FindPython
set(PYBIND11_FINDPYTHON ON)

# Find pybind11
find_package(pybind11 REQUIRED)

# Find Python development files explicitly
find_package(Python3 REQUIRED COMPONENTS Development)

# Add the source files (both snake.c and snake.cpp)
add_library(snake_module MODULE snake.cpp snake.c)

# Link against raylib and Python
find_library(RAYLIB raylib)
target_link_libraries(snake_module PRIVATE pybind11::module ${RAYLIB} Python3::Python)

# Include directories
target_include_directories(snake_module PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${Python3_INCLUDE_DIRS})

# Ensure the module has no prefix and correct extension
set_target_properties(snake_module PROPERTIES PREFIX "" SUFFIX ".so")
