#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 UVs;
layout (location = 2) in vec4 normals;
layout (location = 3) in vec4 tangents;

//Transformation matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
//Directional light
uniform mat4 lightSpaceMatrix[4];
uniform vec4 sunDirection = vec4(0.0,-1.0,0.0,0.0);
//UVs
uniform vec2 UVTiling = vec2(1.0);
//Directional shadows
out vec4 FRAG_POS_LIGHT_SPACE[4];
out float ClipSpacePosZ;
//Normal mapping
out mat4 TBN_MATRIX;
//General
out vec2 UV;
out vec4 NORMAL;
out vec4 FRAG_POS;

uniform int cascadeCount = 1;
uniform mat4 cascadeMatrices[4];

out vec4 TANGENT_SUN_DIR;
//out vec4 TANGENT_EYE_DIR;

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
    vec4 T = normalize(model * tangents);
    vec4 N = normalize(model * normals);
    T = normalize(T - dot(T,N) * N);

    vec4 B = vec4(cross(N.xyz, T.xyz),0.0);

    TBN_MATRIX = transpose(mat4(T,B,N, vec4(0.0,0.0,0.0,1.0)));

    TANGENT_SUN_DIR = normalize(TBN_MATRIX * sunDirection);
    //TANGENT_EYE_DIR = TBN_MATRIX * FRAG_POS;
}

