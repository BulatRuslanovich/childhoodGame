//
// Файл создан getname 28.01.2025.
// Реализация класса Texture для работы с текстурами в OpenGL.
//

#include "Texture.h"
#include <GL/gl.h> // Подключение заголовочного файла OpenGL.

// Конструктор: инициализирует текстуру с указанным идентификатором, шириной и высотой.
Texture::Texture(const int id, const int width, const int height)
    : id(id), width(width), height(height) {
    // Здесь можно добавить дополнительную инициализацию, если необходимо.
}

// Деструктор: освобождает ресурсы текстуры.
Texture::~Texture() {
    glDeleteTextures(1, &id); // Удаление текстуры из OpenGL.
}

// Привязывает текстуру для использования в OpenGL.
void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, id); // Привязка текстуры к цели GL_TEXTURE_2D.
}