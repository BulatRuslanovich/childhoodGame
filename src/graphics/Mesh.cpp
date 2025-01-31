//
// Created by getname on 31.01.2025.
//

#include "Mesh.h"

#include <GL/glew.h>

Mesh::Mesh(const float *buffer, const size_t vertices, const int *attrs) : vertices(vertices) {
    int vertexSize = 0;

    for (int i = 0; attrs[i]; i++) {
        vertexSize += attrs[i];
    }

    // Настройка графического конвейера (VAO/VBO)
    // VAO - Vertex Array Object (контейнер для настроек вершин)
    // VBO - Vertex Buffer Object (буфер с данными вершин)
    glGenVertexArrays(1, &vao); // Создаем VAO
    glGenBuffers(1, &vbo); // Создаем VBO

    // Привязываем VAO и VBO для настройки
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Копируем данные вершин в видеопамять
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * vertexSize * vertices, buffer, GL_STATIC_DRAW);

    // Настройка атрибутов вершин:
    int offset = 0;

    for (int i = 0; attrs[i]; i++) {
        const int size = attrs[i];
        glVertexAttribPointer(i, size, GL_FLOAT, GL_FALSE, vertexSize * sizeof(float),
                              reinterpret_cast<GLvoid *>(offset * sizeof(float)));
        glEnableVertexAttribArray(i);
        offset += size;
    }


    glBindVertexArray(0); // Отвязываем VAO
}

Mesh::~Mesh() {
    glDeleteBuffers(1, &vbo); // Удаление VBO
    glDeleteVertexArrays(1, &vao); // Удаление VAO
}

void Mesh::draw(const unsigned int primitive) const {
    glBindVertexArray(vao); // Привязка вершинных данных
    glDrawArrays(primitive, 0, this->vertices);
    glBindVertexArray(0); // Отвязка VAO
}
