#version 330 core

out vec4 FragColor;

uniform int texArray_size;
uniform sampler2D texArray[];

in vec2 interpCoord;

void main()
{
    vec3 color = vec3(1.0);
    if (texArray_size > 0) {
        vec4 texColor = texture(texArray[0], interpCoord);
        color = color * (1-texColor.a) + vec3(texColor) * texColor.a;
    }
    if (texArray_size > 1) {
        vec4 texColor = texture(texArray[1], interpCoord);
        color = color * (1-texColor.a) + vec3(texColor) * texColor.a;
    }
    if (texArray_size > 2) {
        vec4 texColor = texture(texArray[2], interpCoord);
        color = color * (1-texColor.a) + vec3(texColor) * texColor.a;
    }
    if (texArray_size > 3) {
        vec4 texColor = texture(texArray[3], interpCoord);
        color = color * (1-texColor.a) + vec3(texColor) * texColor.a;
    }
    if (texArray_size > 4) {
        vec4 texColor = texture(texArray[4], interpCoord);
        color = color * (1-texColor.a) + vec3(texColor) * texColor.a;
    }
    if (texArray_size > 5) {
        vec4 texColor = texture(texArray[5], interpCoord);
        color = color * (1-texColor.a) + vec3(texColor) * texColor.a;
    }

    FragColor = vec4(color, 1.0);

}