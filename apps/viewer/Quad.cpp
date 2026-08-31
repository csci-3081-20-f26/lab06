#include "Quad.h"

// Include glad graphics
#include <glad/glad.h>

namespace csci3081 {

Quad::Quad() {
    float vertices[] = {
         1.0f,  1.0f, 0.0f,  // top right
         1.0f, -1.0f, 0.0f,  // bottom right
        -1.0f, -1.0f, 0.0f,  // bottom left
        -1.0f,  1.0f, 0.0f   // top left 
    };
    float coords[] = {
         1.0f,  0.0f,  // top right
         1.0f, 1.0f,  // bottom right
        0.0f, 1.0f,  // bottom left
        0.0f,  0.0f   // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);
    // Add the data to the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(coords), (void*)0, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), &vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(coords), &coords);
    // Add the elements to the EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // Setup vertex attributes to be used in shader
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(sizeof(vertices)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 
}

Quad::~Quad(){
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Quad::draw() const {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

}