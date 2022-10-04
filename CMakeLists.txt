cmake_minimum_required(VERSION 3.20)

project(openglpaska)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(glfw3 REQUIRED)

add_library(glad include/glad/glad.h src/glad.c)
target_include_directories(glad PUBLIC include)

add_executable(${PROJECT_NAME} src/main.cpp
                               src/timer.hpp
                               src/timer.cpp
                               src/shader.hpp
                               src/shader.cpp
              )
target_link_libraries(${PROJECT_NAME} dl glfw glad)
