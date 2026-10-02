#include "ShaderProgram.h"

// Include glad graphics
#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <sstream>


namespace csci3081 {

std::string ShaderProgram::load_shader_file(const std::string& shader) {
    std::ifstream file(shader.c_str());
	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

ShaderProgram::ShaderProgram(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) {
    std::string vertexShaderSourceStr = load_shader_file(vertexShaderPath);
    std::string fragmentShaderSourceStr = load_shader_file(fragmentShaderPath);
    compile(vertexShaderSourceStr, fragmentShaderSourceStr);
}

void ShaderProgram::compile(const std::string& vertexShaderSourceStr, const std::string& fragmentShaderSourceStr) {
    const char* vertexShaderSource = vertexShaderSourceStr.c_str();
    const char* fragmentShaderSource = fragmentShaderSourceStr.c_str();
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    if (compiled) {
        glDeleteProgram(shaderProgram);
    }
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    compiled = true;
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(shaderProgram);
}

void ShaderProgram::use() const {
    glUseProgram(shaderProgram);
}

void ShaderProgram::setFloat(const std::string& name, float value) const {
    int loc = glGetUniformLocation(shaderProgram, name.c_str());
    glUniform1f(loc, value);
}

void ShaderProgram::setVec3(const std::string& name, float x, float y, float z) const {
    int loc = glGetUniformLocation(shaderProgram, name.c_str());
    glUniform3f(loc, x, y, z);
}

void ShaderProgram::setTexture(const std::string& name, const Texture& texture, int index) const {
    glActiveTexture(GL_TEXTURE0 + index);
    texture.use();
    int texLoc = glGetUniformLocation(shaderProgram, name.c_str());
    glUniform1i(texLoc, index);
    std::string res = name + "_res";
    int texResLoc = glGetUniformLocation(shaderProgram, res.c_str());
    glUniform2f(texResLoc, texture.getWidth(), texture.getHeight());
}

void ShaderProgram::setTextures(const std::string& name, const std::vector<Texture*> textures) const {
    std::vector<GLint> texArray;
    for (int i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        textures[i]->use();
        texArray.push_back(i);
    }
    int texLoc = glGetUniformLocation(shaderProgram, name.c_str());
    glUniform1iv(texLoc, texArray.size(), texArray.data());
    std::string size = name + "_size";
    int texSizeLoc = glGetUniformLocation(shaderProgram, size.c_str());
    glUniform1i(texSizeLoc, texArray.size());
}

}