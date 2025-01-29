//
// Файл создан getname 27.01.2025.
// Заголовочный файл для класса Shader, управляющего шейдерами в OpenGL.
//

#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glm/glm.hpp> // Используем GLM для работы с матрицами и векторами.

// Класс Shader инкапсулирует шейдерную программу OpenGL.
class Shader {
    unsigned int id; // Идентификатор шейдерной программы.
public:
    explicit Shader(unsigned int id);

    ~Shader();

    void use() const;

    void uniformMatrix(const std::string& name, glm::mat4 matrix) const;
};

extern Shader* loadShader(const std::string& vertexFile, const std::string& fragmentFile);

#endif // SHADER_H