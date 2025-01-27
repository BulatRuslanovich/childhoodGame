cmake_minimum_required(VERSION 3.29)
project(LolEngine)

set(CMAKE_CXX_STANDARD 20)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(LolEngine main.cpp
        window/Window.cpp
        window/Window.h
        window/Events.cpp
        window/Events.h)
target_link_libraries(LolEngine glfw GLEW::GLEW OpenGL::GL)
