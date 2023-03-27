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
    float TIME;
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

float heightDisplacement(float time, float speed, float tiling, vec2 pos)
{
    vec2 tiledPos = pos * tiling;
    float alpha1 = sin(time * speed * 0.6 + tiledPos.x) * 0.75;
    float alpha2 = cos(time * speed + (tiledPos.x + tiledPos.y) * 2.0) * 0.6;
    float alpha3 = sin(-time * speed * 0.8 + (tiledPos.x * 0.4 + tiledPos.y) * 1.4) * 0.6;
    return alpha1 + alpha2 + alpha3;
}

vec4 calcNorms(float time, float speed, float tiling, vec2 pos, float dist)
{
    vec2 offset = vec2(dist, 0.0);
    float xp = heightDisplacement(time, speed, tiling, pos + offset);
    float xm = heightDisplacement(time, speed, tiling, pos - offset);
    float zp = heightDisplacement(time, speed, tiling, pos + offset.yx);
    float zm = heightDisplacement(time, speed, tiling, pos - offset.yx);
    return normalize(vec4(xp - xm, 1.0, zp - zm, 0.0));
}

void main()
{
    vec4 modPos = position + vec4(0.0,1.0,0.0,0.0) * heightDisplacement(TIME, 1.2, UVTiling.x, position.xz) * 0.05 * UVTiling.y;
    vec4 dispNorm = calcNorms(TIME, 1.2, UVTiling.x, position.xz, -0.015 / UVTiling.x);
    FRAG_POS = model * modPos;
    NORMAL = model * dispNorm;
    UV = UVs * UVTiling;
    gl_Position = projection * view * model * modPos;
    for (int i = 0; i < cascadeCount; i++)
    {
        FRAG_POS_LIGHT_SPACE[i] = lightSpaceMatrix[i] * FRAG_POS;
    }

    ClipSpacePosZ = gl_Position.z;
    vec4 T = normalize(model * tangents);
    vec4 N = normalize(model * dispNorm);
    T = normalize(T - dot(T,N) * N);

    vec4 B = vec4(cross(N.xyz, T.xyz),0.0);

    TBN_MATRIX = mat4(T,B,N, vec4(0.0,0.0,0.0,1.0));

    //TANGENT_EYE_DIR = TBN_MATRIX * FRAG_POS;
}

