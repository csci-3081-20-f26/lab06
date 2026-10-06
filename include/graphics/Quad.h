#ifndef QUAD_H
#define QUAD_H

#include "ShaderProgram.h"

namespace csci3081 {

class Quad {
public:
  Quad();
  ~Quad();
  void draw() const;

private:
  unsigned int VAO, VBO, EBO;
};

} // namespace csci3081

#endif