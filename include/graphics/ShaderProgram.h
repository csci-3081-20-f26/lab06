#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <string>
#include "Texture.h"
#include <vector>

namespace csci3081 {

class ShaderProgram {
public:
  ShaderProgram(const std::string &vertexShaderPath,
                const std::string &fragmentShaderPath);
  ~ShaderProgram();
  void use() const;
  unsigned int getId() const { return shaderProgram; }

  void setFloat(const std::string& name, float value) const;
  void setVec3(const std::string& name, float x, float y, float z) const;
  void setTexture(const std::string& name, const Texture& texture, int index = 0) const;
  void setTextures(const std::string& name, const std::vector<Texture*> textures) const;

protected:
  ShaderProgram() {} 
  void compile(const std::string& vertexShaderSourceStr, const std::string& fragmentShaderSourceStr);
  std::string load_shader_file(const std::string& shader);

private:
  unsigned int shaderProgram;
  bool compiled = false;
};

class TrackShader : public ShaderProgram {
public:
  TrackShader() {
    vertexShaderSourceStr = load_shader_file("src/graphics/shaders/quad.vsh");
    std::string fragmentShaderSourceStr = load_shader_file("src/graphics/shaders/composite.fsh");
    compile(vertexShaderSourceStr, fragmentShaderSourceStr);
  }

  void update(const vector<std::string>& trackFilters) {
    std::string fragmentShaderSourceStr = 
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform float duration;\n"
    "uniform float timeSinceStart;\n"
    "uniform int texArray_size;\n"
    "uniform sampler2D texArray[];\n"
    "in vec2 interpCoord;\n"
    "void main()\n"
    "{\n"
    "    vec3 color = vec3(1.0);\n";

    for (int i = 0; i < trackFilters.size(); i++) {
      fragmentShaderSourceStr +=
      "     {\n"
      "        float time = timeSinceStart/duration;\n"
      "        vec2 pos = interpCoord;\n"
      "        vec4 aggregateColor = vec4(color, 1.0);\n"
      "        vec4 trackColor = texture(texArray[" + std::to_string(i) + "], interpCoord);\n";

      fragmentShaderSourceStr += trackFilters[i] +
      "        color = vec3(aggregateColor) * (1-trackColor.a) + vec3(trackColor) * trackColor.a;\n"
      "     }\n";
    }

    fragmentShaderSourceStr +=
    "   FragColor = vec4(color, 1.0);\n"
    "}\n";
    compile(vertexShaderSourceStr, fragmentShaderSourceStr);
  }

private:
  std::string vertexShaderSourceStr;
};

} // namespace csci3081

#endif