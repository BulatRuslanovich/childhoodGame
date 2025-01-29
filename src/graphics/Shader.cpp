//
// Created by getname on 27.01.2025.
//

#include "Shader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const unsigned int id) : id(id) {
}

Shader::~Shader() {
    glDeleteProgram(this->id);
}

void Shader::use() const {
    glUseProgram(this->id);
}

void Shader::uniformMatrix(const std::string &name, glm::mat4 matrix) const {
    const GLuint transformLoc = glGetUniformLocation(this->id, name.c_str());
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(matrix));
}

Shader* loadShader(const std::string& vertexFile, const std::string& fragmentFile) {
    // Объявляем строки для хранения кода вершинного и фрагментного шейдеров.
    std::string vertexCode;
    std::string fragmentCode;

    // Создаем потоки для чтения файлов шейдеров.
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    // Устанавливаем исключения для потоков, чтобы отлавливать ошибки открытия файлов.
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        // Открываем файлы шейдеров.
        vShaderFile.open(vertexFile);
        fShaderFile.open(fragmentFile);

        // Создаем потоковые объекты для считывания содержимого файлов.
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf(); // Читаем содержимое вершинного шейдера.
        fShaderStream << fShaderFile.rdbuf(); // Читаем содержимое фрагментного шейдера.

        // Закрываем файлы после считывания.
        vShaderFile.close();
        fShaderFile.close();

        // Получаем строки кода шейдеров из потоков.
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch (std::ifstream::failure e) {
        // Обработка ошибок при открытии или чтении файлов.
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESS" << std::endl;
        return nullptr; // Возвращаем nullptr в случае ошибки.
    }

    // Преобразуем строки кода в указатели на символы для OpenGL.
    const GLchar* vShaderCode = vertexCode.c_str();
    const GLchar* fShaderCode = fragmentCode.c_str();

    GLuint vertex, fragment; // Объявляем переменные для хранения идентификаторов шейдеров.
    GLint success; // Переменная для проверки статуса компиляции.
    GLchar infoLog[512]; // Массив для хранения сообщений об ошибках.

    // Создаем вершинный шейдер.
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr); // Устанавливаем код шейдера.
    glCompileShader(vertex); // Компилируем шейдер.
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success); // Проверяем статус компиляции.

    // Проверяем, успешно ли скомпилирован вершинный шейдер.
    if (!success) {
        glGetShaderInfoLog(vertex, 512, nullptr, infoLog); // Получаем информацию об ошибке.
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        return nullptr; // Возвращаем nullptr в случае ошибки компиляции.
    }

    // Создаем фрагментный шейдер.
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr); // Устанавливаем код шейдера.
    glCompileShader(fragment); // Компилируем шейдер.
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success); // Проверяем статус компиляции.

    // Проверяем, успешно ли скомпилирован фрагментный шейдер.
    if (!success) {
        glGetShaderInfoLog(fragment, 512, nullptr, infoLog); // Получаем информацию об ошибке.
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        return nullptr; // Возвращаем nullptr в случае ошибки компиляции.
    }

    // Создаем программу шейдера.
    GLuint id = glCreateProgram();
    glAttachShader(id, vertex); // Прикрепляем вершинный шейдер к программе.
    glAttachShader(id, fragment); // Прикрепляем фрагментный шейдер к программе.
    glLinkProgram(id); // Линкуем программу.

    glGetProgramiv(id, GL_LINK_STATUS, &success); // Проверяем статус линковки.

    // Проверяем, успешно ли слинкована программа шейдера.
    if (!success) {
        glGetProgramInfoLog(id, 512, nullptr, infoLog); // Получаем информацию об ошибке.
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;

        // Очищаем ресурсы в случае ошибки.
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        glDeleteProgram(id);
        return nullptr; // Возвращаем nullptr в случае ошибки линковки.
    }

    // Очищаем ресурсы, так как они уже привязаны к программе.
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    // Возвращаем указатель на новый объект Shader с идентификатором программы.
    return new Shader(id);
}



