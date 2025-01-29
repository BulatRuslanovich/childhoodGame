//
// Created by getname on 28.01.2025.
//

#include "Texture.h"

#include <GL/gl.h>

Texture::Texture(const int id, const int width,
                 const int height) : id(id), width(width), height(height) {
}

Texture::~Texture() {
    glDeleteTextures(1, &id);
}

void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, id);
}
