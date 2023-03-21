#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 UVs;
layout (location = 2) in vec4 normals;
layout (location = 3) in vec4 tangents;

//Transformation matrices

layout (std140) uniform Matrices
{
    uniform mat4 view;
    uniform mat4 projection;
};

uniform mat4 model;

//Directional light
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

void main()
{
    FRAG_POS = model * position;
    NORMAL = transpose(inverse(model)) * normals;
    UV = UVs * UVTiling;
    gl_Position = projection * view * model * position;
    for (int i = 0; i < cascadeCount; i++)
    {
        FRAG_POS_LIGHT_SPACE[i] = lightSpaceMatrix[i] * FRAG_POS;
    }

    ClipSpacePosZ = gl_Position.z;
    vec4 T = normalize(model * tangents);
    vec4 N = normalize(model * normals);
    T = normalize(T - dot(T,N) * N);

    vec4 B = vec4(cross(N.xyz, T.xyz),0.0);

    TBN_MATRIX = mat4(T,B,N, vec4(0.0,0.0,0.0,1.0));

    //TANGENT_EYE_DIR = TBN_MATRIX * FRAG_POS;
}

