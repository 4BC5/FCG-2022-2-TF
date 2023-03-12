#version 330 core

layout (location = 0) in vec4 ModelVertices;
layout (location = 1) in vec2 UVs;
layout (location = 2) in vec4 ModelNormals;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
//uniform mat4 lightSpaceMatrix;

//out vec2 UV;
//out vec4 NORMAL;
//out mat4 VIEW_MATRIX;
//out vec4 FRAG_POS;
//out vec4 FRAG_POS_LIGHT_SPACE;

void main()
{
    gl_Position = projection * view * model * ModelVertices;
    //UV = UVs;
    //NORMAL = ModelNormals;
    //VIEW_MATRIX = view;
    //FRAG_POS = model * ModelVertices;
    //FRAG_POS_LIGHT_SPACE = lightSpaceMatrix * FRAG_POS;
}

