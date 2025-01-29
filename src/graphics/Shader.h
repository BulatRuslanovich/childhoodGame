//
// Created by getname on 27.01.2025.
//

#ifndef SHADER_H
#define SHADER_H
#include <string>
#include <glm/glm.hpp>


class Shader {
    unsigned int id;
public:
    explicit Shader(unsigned int id);
    ~Shader();

    void use() const;
    void uniformMatrix(const std::string& name, glm::mat4 matrix) const;
};

extern Shader* loadShader(const std::string& vertexFile, const std::string& fragmentFile);



#endif //SHADER_H
