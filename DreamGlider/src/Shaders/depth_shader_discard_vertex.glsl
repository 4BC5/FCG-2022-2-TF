#version 330 core

layout (location = 0) in vec4 ModelVertices;
layout (location = 1) in vec2 UVs;

uniform mat4 model;
uniform mat4 lightSpaceMatrix;
uniform vec2 UVtiling = vec2(1.0);

out vec2 UV;

void main()
{
    gl_Position = lightSpaceMatrix * model * ModelVertices;
    UV = UVs * UVtiling;
}

