#version 330 core

uniform sampler2D alphaTex;

in vec2 UV;

void main()
{
    if (texture(alphaTex, UV).w < 0.1)
        discard;
}

