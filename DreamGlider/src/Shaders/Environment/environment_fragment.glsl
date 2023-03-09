#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube cubemap;
uniform float environmentStrength = 1.0;

void main()
{    
    FragColor = pow(texture(cubemap, TexCoords) * environmentStrength,vec4(2.2));
}

