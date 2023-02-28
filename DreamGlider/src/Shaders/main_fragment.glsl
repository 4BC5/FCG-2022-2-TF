#version 330 core

// Atributos de fragmentos recebidos como entrada ("in") pelo Fragment Shader.
// Neste exemplo, este atributo foi gerado pelo rasterizador como a
// interpolação da cor de cada vértice, definidas em "shader_vertex.glsl" e
// "main.cpp" (array color_coefficients).
in vec2 UV;
in vec4 NORMAL;

// O valor de saída ("out") de um Fragment Shader é a cor final do fragmento.
out vec4 color;

void main()
{
    // Definimos a cor final de cada fragmento utilizando a cor interpolada
    // pelo rasterizador.
    color = vec4(0.0,0.0,0.0,1.0);// * max(dot(NORMAL, vec4(0.0,-1.0,0.0,0.0)),0.0);
} 

