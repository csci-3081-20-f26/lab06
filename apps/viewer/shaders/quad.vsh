#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 coord;

uniform vec3 offset;
uniform vec3 scale;
out vec2 interpCoord;

void main() {
   interpCoord = coord;
   gl_Position = vec4(aPos.xyz*scale + offset, 1.0);
}