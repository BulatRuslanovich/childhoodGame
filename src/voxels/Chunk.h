//
// Created by getname on 31.01.2025.
//

#ifndef _CHUNK_H
#define _CHUNK_H

#define CHUNK_W 16
#define CHUNK_H 16
#define CHUNK_D 16
#define CHUNK_VOL (CHUNK_W * CHUNK_H * CHUNK_D)

class voxel;

class Chunk {
public:
    voxel* voxels;
    Chunk();
    ~Chunk();
};



#endif //_CHUNK_H
