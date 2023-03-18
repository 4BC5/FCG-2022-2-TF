#version 330 core


uniform sampler2D directionalShadowMap[4];
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


in vec2 UV;
out vec4 color;

void main()
{
    color = texture(directionalShadowMap[0], UV);
} 

