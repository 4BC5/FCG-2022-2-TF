#version 330 core

// Atributos de fragmentos recebidos como entrada ("in") pelo Fragment Shader.
// Neste exemplo, este atributo foi gerado pelo rasterizador como a
// interpolação da cor de cada vértice, definidas em "shader_vertex.glsl" e
// "main.cpp" (array color_coefficients).
in vec2 UV;
in vec4 NORMAL;
in mat4 VIEW_MATRIX;

// O valor de saída ("out") de um Fragment Shader é a cor final do fragmento.
out vec4 color;


void main()
{

    color = vec4(UV.x, UV.y, 0.0f, 1.0f) * max(-dot(NORMAL, normalize(vec4(0.0f,-1.0f,-0.75f,0.0f))),0.2f);
} 

