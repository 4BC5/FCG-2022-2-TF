#version 330 core

// Atributos de vértice recebidos como entrada ("in") pelo Vertex Shader.
// Veja a função BuildTriangle() em "main.cpp".
layout (location = 0) in vec4 model_coefficients;
layout (location = 1) in vec2 UVs;
layout (location = 2) in vec4 normals;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

out vec2 UV;
out vec4 NORMAL;
out vec4 FRAG_POS;
out vec4 FRAG_POS_LIGHT_SPACE;
out mat4 VIEW_MATRIX;

// Variável booleana no código C++ também enviada para a GPU

void main()
{
    FRAG_POS = model * model_coefficients;
    NORMAL = transpose(inverse(model)) * normals;
    UV = UVs;
    FRAG_POS_LIGHT_SPACE = lightSpaceMatrix * FRAG_POS;
    gl_Position = projection * view * model * model_coefficients;
}

