cmake_minimum_required(VERSION 3.7)

project("ComplexPlotting")

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_subdirectory("src")