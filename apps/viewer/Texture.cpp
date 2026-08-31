#include "Texture.h"

// Include glad graphics
#include <glad/glad.h>

namespace csci3081 {


Texture::Texture(const Image& image) {
    width = image.getWidth();
    height = image.getHeight();

    glGenTextures(1, &texture);  
    glBindTexture(GL_TEXTURE_2D, texture);  

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getWidth(), image.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getData());
    glGenerateMipmap(GL_TEXTURE_2D);
}

Texture::~Texture() {
    glDeleteTextures(1, &texture);
}

void Texture::use() const {
    glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture::copyToGPU(const Image& image) {
    width = image.getWidth();
    height = image.getHeight();

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getWidth(), image.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getData());
    glGenerateMipmap(GL_TEXTURE_2D);
}

}