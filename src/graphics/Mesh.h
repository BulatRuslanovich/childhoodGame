//
// Created by getname on 31.01.2025.
//

#ifndef MESH_H
#define MESH_H

#include <cstddef>

class Mesh {
    unsigned int vao;
    unsigned int vbo;
    size_t vertices;
public:
    Mesh(const float* buffer, size_t vertices, const int* attrs);
    ~Mesh();

    void draw(unsigned int primitive) const;
};



#endif //MESH_H
