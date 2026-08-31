#ifndef TEXTURE_H
#define TEXTURE_H

#include "Image.h"

namespace csci3081 {

class Texture {
public:
    Texture(const Image& image);
    ~Texture();
    void use() const;
    void copyToGPU(const Image& image);
    int getWidth() const { return width; }
    int getHeight() const { return height; }

private:
    unsigned int texture;
    int width;
    int height;
};

}

#endif