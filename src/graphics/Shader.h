//
// Created by getname on 27.01.2025.
//

#ifndef SHADER_H
#define SHADER_H
#include <string>


class Shader {
    unsigned int id;
public:
    explicit Shader(unsigned int id);
    ~Shader();

    void use() const;
};

extern Shader* loadShader(const std::string& vertexFile, const std::string& fragmentFile);



#endif //SHADER_H
