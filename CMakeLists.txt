# CMake version
cmake_minimum_required(VERSION 3.17.2 FATAL_ERROR)

# Declare project
project(Ray-Tracing-The-Next-Week)

# Set ouput directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

# Excutables
add_executable(Ray-Tracing-The-Next-Week src/main.cpp)