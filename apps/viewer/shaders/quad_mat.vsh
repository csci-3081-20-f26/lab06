#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 coord;

uniform vec3 offset;
uniform vec3 scale;
uniform mat4 model;
out vec2 interpCoord;

void main() {
   interpCoord = coord;
   gl_Position = model*vec4(aPos.xyz,1.0);
}