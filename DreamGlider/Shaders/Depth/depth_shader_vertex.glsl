#version 330 core

layout (location = 0) in vec4 ModelVertices;

uniform mat4 model;
uniform mat4 LSM;

void main()
{
    gl_Position = LSM * model * ModelVertices;
}

