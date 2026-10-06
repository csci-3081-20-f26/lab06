#include "Glyph.h"

// Include glad graphics
#include <glad/glad.h>


namespace csci3081 {

Glyph::Glyph(float x, float y, float w, float h, const Image& image) : x(x), y(y), w(w), h(h) {
    Texture* texture = new Texture(image);
    TexturePtr texPtr;
    texPtr.texture = texture;
    texPtr.weak = false;
    textures.push_back(texPtr);
    shaderProgram = new ShaderProgram("apps/viewer/shaders/quad.vsh", "apps/viewer/shaders/quad.fsh");
}

Glyph::Glyph(float x, float y, float w, float h, const Image& image, ShaderProgram* shaderProgram) : x(x), y(y), w(w), h(h), shaderProgram(shaderProgram) {
    Texture* texture = new Texture(image);
    TexturePtr texPtr;
    texPtr.texture = texture;
    texPtr.weak = false;
    textures.push_back(texPtr);
}

Glyph::~Glyph() {
    for (int i = 0; i < textures.size(); i++) {
        if (!textures[i].weak) {
            delete textures[i].texture;
        }
    }
    delete shaderProgram;
}

Glyph::Glyph(float x, float y, float w, float h, ShaderProgram* shaderProgram) : x(x), y(y), w(w), h(h), shaderProgram(shaderProgram) {
}

Glyph::Glyph(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {
    shaderProgram = new ShaderProgram("src/graphics/shaders/quad.vsh", "src/graphics/shaders/quad.fsh");
}

void Glyph::update(const Image& image) {
    if (textures.size() > 0) {
        textures[0].texture->copyToGPU(image);
    }
}

void Glyph::setTextures(const std::vector<Texture*>& texArray) {
    for (int i = 0; i < textures.size(); i++) {
        if (!textures[i].weak) {
            delete textures[i].texture;
        }
    }

    textures.clear();

    for (int i = 0; i < texArray.size(); i++) {
        Texture* texture = texArray[i];
        TexturePtr texPtr;
        texPtr.texture = texture;
        texPtr.weak = true;
        textures.push_back(texPtr);
    }
}

void Glyph::addTexture(Texture &texture) {
    TexturePtr texPtr;
    texPtr.texture = &texture;
    texPtr.weak = true;
    textures.push_back(texPtr);
}

void Glyph::removeTexture(Texture &texture) {
    for (auto it = textures.begin(); it != textures.end(); it++) {
        if (it->texture == &texture) {
            textures.erase(it);
            break;
        }
    }
}

void Glyph::draw() const {
    const ShaderProgram& program = getShaderProgram();
    program.use();
    if (textures.size() > 0) {
        program.setTexture("tex", *(textures[0].texture));
    }
    std::vector<Texture*> texArray;
    for (int i = 0; i < textures.size(); i++) {
        texArray.push_back(textures[i].texture);
    }
    program.setTextures("texArray", texArray);
    //program.setVec3("scale", w, h, 1.0f);
    //program.setVec3("offset", x*2.0-1.0 + w, 1.0 - h - y*2.0, 0.0f);

    unsigned int transformLoc = glGetUniformLocation(getShaderProgram().getId(), "model");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model));

    quad.draw();
}

}