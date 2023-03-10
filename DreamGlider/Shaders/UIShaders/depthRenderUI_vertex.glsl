#version 330 core

// Atributos de vértice recebidos como entrada ("in") pelo Vertex Shader.
// Veja a função BuildTriangle() em "main.cpp".
layout (location = 0) in vec4 position;
layout (location = 1) in vec2 UVs;
layout (location = 2) in vec4 normals;
layout (location = 3) in vec4 tangents;
//layout (location = 4) in vec4 bitangents;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix[4];
uniform vec4 sunDirection = vec4(0.0,-1.0,0.0,0.0);

uniform vec2 UVTiling = vec2(1.0);

out vec2 UV;
out vec4 NORMAL;
out vec4 FRAG_POS;
out vec4 FRAG_POS_LIGHT_SPACE[4];
out float ClipSpacePosZ;

uniform int cascadeCount = 1;
uniform mat4 cascadeMatrices[4];

void main()
{
    FRAG_POS = model * position;
    NORMAL = transpose(inverse(model)) * normals;
    UV = UVs * UVTiling;
    gl_Position = projection * view * model * position;
    for (int i = 0; i < cascadeCount; i++)
    {
        FRAG_POS_LIGHT_SPACE[i] = cascadeMatrices[i] * FRAG_POS;
    }

    ClipSpacePosZ = gl_Position.z;
}

