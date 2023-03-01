#version 330 core

// Atributos de vértice recebidos como entrada ("in") pelo Vertex Shader.
// Veja a função BuildTriangle() em "main.cpp".
layout (location = 0) in vec4 ModelVertices;
layout (location = 1) in vec2 UVs;
layout (location = 2) in vec4 ModelNormals;

// Atributos de vértice que serão gerados como saída ("out") pelo Vertex Shader.
// ** Estes serão interpolados pelo rasterizador! ** gerando, assim, valores
// para cada fragmento, os quais serão recebidos como entrada pelo Fragment
// Shader. Veja o arquivo "shader_fragment.glsl".
out vec2 UV;
out vec4 NORMAL;
out mat4 VIEW_MATRIX;

// Matrizes computadas no código C++ e enviadas para a GPU
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Variável booleana no código C++ também enviada para a GPU
uniform bool render_as_black;

void main()
{
    gl_Position = projection * view * model * ModelVertices;

    UV = UVs;
    NORMAL = ModelNormals;
    VIEW_MATRIX = view;
}

