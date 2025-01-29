//
// Created by getname on 28.01.2025.
//

#ifndef TEXTURE_H
#define TEXTURE_H
#include <string>


class Texture {
public:
    unsigned int id;
    unsigned int width;
    unsigned int height;

    Texture(int id, int width, int height);
    ~Texture();

    void bind() const;
};

extern Texture* loadTexture(const std::string &filename);

#endif //TEXTURE_H
