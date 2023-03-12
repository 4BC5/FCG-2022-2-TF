#version 330

uniform sampler2D tex;

in vec2 texCoords;
out vec4 fragColor;

void main()
{
    fragColor = vec4(0, 0, 0, texture(tex, texCoords).r);
}