#version 330 core

in vec2 TexCoords;
out vec4 textOutColor;

uniform sampler2D text;
uniform vec3 textColor;

void main()
{    
    textOutColor = texture(text, TexCoords) * vec4(textColor, 1.0);
}  