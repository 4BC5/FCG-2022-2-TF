#version 330 core


uniform sampler2D directionalShadowMap[4];
layout (std140) uniform DirectionalLight
{
    uniform mat4 lightSpaceMatrix[4];
    uniform float cascadePlaneDistances[4];
    uniform vec4 u_sunDirection;
    uniform float u_sunIntensity;
    uniform vec4 u_sunColor;
    uniform int cascadeCount;
    uniform float farPlane;
    uniform float shadowBias;
    uniform int shadowSamples;
    uniform float shadowBlur;
};

in vec2 UV;
out vec4 color;

void main()
{
    color = texture(directionalShadowMap[0], UV);
} 

