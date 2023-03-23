#version 330 core

// Atributos de vértice recebidos como entrada ("in") pelo Vertex Shader.
// Veja a função BuildTriangle() em "main.cpp".
layout (location = 0) in vec4 position;
layout (location = 1) in vec2 UVs;
layout (location = 2) in vec4 normals;
layout (location = 3) in vec4 tangents;
//layout (location = 4) in vec4 bitangents;

uniform mat4 model;

layout (std140) uniform Matrices
{
    uniform mat4 view;
    uniform mat4 projection;
    float TIME;
};

layout (std140) uniform DirectionalLight
{
    uniform float u_sunIntensity;
    uniform vec4 u_sunColor;
    uniform vec4 u_sunDirection;
};

layout (std140) uniform directionalShadows
{
    uniform mat4 lightSpaceMatrix[4];
    uniform float cascadePlaneDistances[4];
    uniform int cascadeCount;
    uniform float farPlane;
    uniform float shadowBias;
    uniform int shadowSamples;
    uniform float shadowBlur;
    uniform float biasSplitMultiplier;
};


out vec2 UV;


void main()
{
    gl_Position = projection * view * model * position;
    UV = UVs;
}

