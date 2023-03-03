#version 330 core

layout (location = 0) in vec4 ModelVertices;

uniform mat4 model;
uniform mat4 lightSpaceMatrix;

void main()
{
    gl_Position = lightSpaceMatrix * model * ModelVertices;
}

