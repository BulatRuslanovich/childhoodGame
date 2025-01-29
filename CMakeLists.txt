cmake_minimum_required(VERSION 3.29)
project(LolEngine VERSION 0.1 LANGUAGES CXX)

# Настройка стандарта C++ и политик CMake
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Включение обработки зависимостей пакетов
include(GNUInstallDirs)

# Поиск необходимых зависимостей
find_package(glfw3 REQUIRED)
find_package(GLEW 2.2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(PNG REQUIRED)

# Настройка исполняемого файла
file(GLOB_RECURSE SOURCES
        "src/*.cpp"
        "src/*.h"
        "src/*/*.cpp"
        "src/*/*.h"
)

add_executable(LolEngine ${SOURCES})

# Настройка включаемых директорий
target_include_directories(LolEngine PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)

# Настройка линковки с зависимостями
target_link_libraries(LolEngine PRIVATE
        glfw                  # Современная цель GLFW
        GLEW::GLEW
        OpenGL::GL
        PNG::PNG              # Современное имя цели для libpng
)

# Дополнительные настройки компиляции
if(MSVC)
    target_compile_options(LolEngine PRIVATE /W4 /WX)
else()
    target_compile_options(LolEngine PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

# Оптимизация для релизных сборок
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE_UPPER)
if(BUILD_TYPE_UPPER STREQUAL "RELEASE")
    target_compile_options(LolEngine PRIVATE -O3)
endif()
