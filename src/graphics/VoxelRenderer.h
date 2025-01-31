//
// Created by getname on 01.02.2025.
//

#ifndef VOXELRENDERER_H
#define VOXELRENDERER_H

#include <cstddef>

class Chunk;
class Mesh;

class VoxelRenderer {
    float* buffer;
    size_t capacity;
public:
    VoxelRenderer(size_t capacity);
    ~VoxelRenderer();

    Mesh* render(Chunk* chunk) const;
};



#endif //VOXELRENDERER_H
