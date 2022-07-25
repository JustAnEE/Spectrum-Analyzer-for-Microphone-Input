#version 330 core
layout (location = 2) in vec3 textPos;
layout (location = 3) in vec2 textCoords;
out vec2 TexCoords;

void main()
{
    gl_Position = vec4(textPos, 1.0);
    TexCoords = textCoords;
}  