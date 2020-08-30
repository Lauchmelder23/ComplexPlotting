cmake_minimum_required(VERSION 3.7)

project("ComplexPlotting")

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_subdirectory("src")