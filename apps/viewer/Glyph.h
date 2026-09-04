#ifndef GLYPH_H
#define GLYPH_H

#include "Quad.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace csci3081 {

class Glyph {
public:
  Glyph(float x, float y, float w, float h, const Image &image);
  Glyph(float x, float y, float w, float h, const Image &image,
        ShaderProgram *shaderProgram);
  Glyph(float x, float y, float w, float h);
  Glyph(float x, float y, float w, float h,ShaderProgram *shaderProgram);
  virtual ~Glyph();

  void update(const Image &image);
  void setTextures(const std::vector<Texture*>& textures);
  void addTexture(Texture &texture);
  void removeTexture(Texture &texture);
  virtual void draw() const;
  void setQuad(float x, float y, float w, float h) {
    this->x = x; this->y = y; this->w = w; this->h = h;
  }

  Glyph(const Glyph &glyph) = delete;
  Glyph &operator=(const Glyph &glyph) = delete;
  void setModelMatrix(const glm::mat4& model) {
    this->model = model;
  }

protected:
  const ShaderProgram &getShaderProgram() const { return *shaderProgram; }

protected:
  float x, y, w, h;

private:
  struct TexturePtr {
    Texture* texture;
    bool weak = true;
  };

  std::vector<TexturePtr> textures;
  ShaderProgram *shaderProgram;
  Quad quad;
  glm::mat4 model;
};

} // namespace csci3081

#endif