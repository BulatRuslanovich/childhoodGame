//
// Файл создан getname 27.01.2025.
// Реализация класса Shader для работы с шейдерами в OpenGL.
//

#include "Shader.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

// Конструктор: инициализирует шейдер с указанным идентификатором программы.
Shader::Shader(const unsigned int id) : id(id) {
}

// Деструктор: освобождает ресурсы шейдерной программы.
Shader::~Shader() {
    glDeleteProgram(this->id);
}

// Активирует шейдерную программу для использования.
void Shader::use() const {
    glUseProgram(this->id);
}

// Устанавливает значение uniform-матрицы в шейдере.
void Shader::uniformMatrix(const std::string &name, glm::mat4 matrix) const {
    const GLuint transformLoc = glGetUniformLocation(this->id, name.c_str());
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(matrix));
}

// Загружает и компилирует шейдеры из файлов, возвращает объект Shader.
Shader *loadShader(const std::string &vertexFile, const std::string &fragmentFile) {
    std::string vertexCode, fragmentCode; // Хранят исходный код шейдеров.
    std::ifstream vShaderFile, fShaderFile; // Потоки для чтения файлов.

    // Включаем обработку исключений для потоков.
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        // Открываем и читаем файлы шейдеров.
        vShaderFile.open(vertexFile);
        fShaderFile.open(fragmentFile);
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf(); // Чтение вершинного шейдера.
        fShaderStream << fShaderFile.rdbuf(); // Чтение фрагментного шейдера.
        vShaderFile.close();
        fShaderFile.close();

        // Сохраняем код шейдеров в строки.
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch (std::ifstream::failure &e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESS" << std::endl;
        return nullptr; // Ошибка при чтении файлов.
    }

    // Преобразуем код шейдеров в C-строки для OpenGL.
    const GLchar *vShaderCode = vertexCode.c_str();
    const GLchar *fShaderCode = fragmentCode.c_str();

    GLuint vertex, fragment; // Идентификаторы шейдеров.
    GLint success; // Статус компиляции/линковки.
    GLchar infoLog[512]; // Лог ошибок.

    // Компиляция вершинного шейдера.
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        return nullptr; // Ошибка компиляции.
    }

    // Компиляция фрагментного шейдера.
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        return nullptr; // Ошибка компиляции.
    }

    // Создание и линковка шейдерной программы.
    GLuint id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(id, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;

        // Очистка ресурсов в случае ошибки.
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        glDeleteProgram(id);
        return nullptr; // Ошибка линковки.
    }

    // Освобождаем шейдеры, так как они уже привязаны к программе.
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    // Возвращаем новый объект Shader.
    return new Shader(id);
}
