//
// Файл создан getname 28.01.2025.
// Заголовочный файл для класса Texture, управляющего текстурами в OpenGL.
//

#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

// Класс Texture инкапсулирует текстуру OpenGL.
class Texture {
public:
    unsigned int id;    // Идентификатор текстуры в OpenGL.
    unsigned int width; // Ширина текстуры в пикселях.
    unsigned int height; // Высота текстуры в пикселях.

    Texture(int id, int width, int height);

    ~Texture();

    void bind() const;
};

extern Texture* loadTexture(const std::string &filename);

#endif // TEXTURE_H