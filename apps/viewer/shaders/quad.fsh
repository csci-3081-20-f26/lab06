#version 330 core

out vec4 FragColor;

uniform sampler2D tex;

in vec2 interpCoord;

void main()
{
    vec4 color = texture(tex, interpCoord); 
    if (color.a < 0.9) {discard;} 
    FragColor = color;

}