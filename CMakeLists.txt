cmake_minimum_required(VERSION 3.29)
project(LolEngine)

set(CMAKE_CXX_STANDARD 20)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(PNG REQUIRED)

add_executable(LolEngine src/main.cpp
        src/window/Window.cpp
        src/window/Window.h
        src/window/Events.cpp
        src/window/Events.h
        src/graphics/Shader.cpp
        src/graphics/Shader.h
        src/graphics/Texture.cpp
        src/graphics/Texture.h
        src/loader/PngLoader.cpp
        src/loader/PngLoader.h
        src/window/Camera.cpp
        src/window/Camera.h
)
target_link_libraries(LolEngine glfw GLEW::GLEW OpenGL::GL png)
