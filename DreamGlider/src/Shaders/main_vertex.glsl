#version 330 core

// Atributos de vértice recebidos como entrada ("in") pelo Vertex Shader.
// Veja a função BuildTriangle() em "main.cpp".
layout (location = 0) in vec4 model_coefficients;
layout (location = 1) in vec2 UVs;
layout (location = 2) in vec4 normals;
layout (location = 3) in vec4 tangents;
//layout (location = 4) in vec4 bitangents;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;
uniform vec4 sunDirection = vec4(0.0,-1.0,0.0,0.0);

out vec2 UV;
out vec4 NORMAL;
out vec4 FRAG_POS;
out vec4 FRAG_POS_LIGHT_SPACE;
out mat4 VIEW_MATRIX;
out mat4 TBN_MATRIX;

out vec4 TANGENT_SUN_DIR;
//out vec4 TANGENT_EYE_DIR;

void main()
{
    FRAG_POS = model * model_coefficients;
    NORMAL = transpose(inverse(model)) * normals;
    UV = UVs;
    FRAG_POS_LIGHT_SPACE = lightSpaceMatrix * FRAG_POS;

    vec4 T = normalize(model * tangents);
    vec4 N = normalize(model * normals);
    T = normalize(T - dot(T,N) * N);

    vec4 B = vec4(cross(N.xyz, T.xyz),0.0);

    TBN_MATRIX = mat4(T,B,N,vec4(0.0,0.0,0.0,1.0));

    TANGENT_SUN_DIR = TBN_MATRIX * sunDirection;
    //TANGENT_EYE_DIR = TBN_MATRIX * FRAG_POS;

    gl_Position = projection * view * model * model_coefficients;
}

