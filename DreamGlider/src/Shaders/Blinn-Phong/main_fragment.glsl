#version 330 core

in vec2 UV;
in vec4 NORMAL;
in vec4 FRAG_POS;

//Directional shadows
uniform sampler2D directionalShadowMap[4];
uniform vec4 sunDirection = vec4(0.0,-1.0,0.0,0.0);
in vec4 FRAG_POS_LIGHT_SPACE[4];
in float ClipSpacePosZ;
uniform float cascadePlaneDistances[4];
uniform int cascadeCount = 1;
uniform float farPlane = 300.0;
uniform float shadowBias = 0.001;
uniform float biasSplitMultiplier = 1.4;
uniform int shadowSamples = 4;
//Normal mapping
uniform float normalStrength = 1.0;
in vec4 TANGENT_SUN_DIR;
in vec4 TANGENT_DOWN;
//Textures
uniform sampler2D albedoTexture;
uniform sampler2D normalTexture;
uniform sampler2D roughnessTexture;
//Shading
uniform vec4 color = vec4(1.0);
uniform float specularPower;
uniform float specularStrength = 4.0;
uniform float transmission = 0.0;
uniform samplerCube environmentCubemap;
uniform float environmentStrength;

in vec4 TANGENT_CAM_POS;
in vec4 TANGENT_FRAG_POS;

out vec4 fragColor;

const vec2 poisson16[] = vec2[](    // These are the Poisson Disk Samples
                                vec2( -0.94201624,  -0.39906216 ),
                                vec2(  0.94558609,  -0.76890725 ),
                                vec2( -0.094184101, -0.92938870 ),
                                vec2(  0.34495938,   0.29387760 ),
                                vec2( -0.91588581,   0.45771432 ),
                                vec2( -0.81544232,  -0.87912464 ),
                                vec2( -0.38277543,   0.27676845 ),
                                vec2(  0.97484398,   0.75648379 ),
                                vec2(  0.44323325,  -0.97511554 ),
                                vec2(  0.53742981,  -0.47373420 ),
                                vec2( -0.26496911,  -0.41893023 ),
                                vec2(  0.79197514,   0.19090188 ),
                                vec2( -0.24188840,   0.99706507 ),
                                vec2( -0.81409955,   0.91437590 ),
                                vec2(  0.19984126,   0.78641367 ),
                                vec2(  0.14383161,  -0.14100790 )
                               );

float rndNum(vec4 seed4)
{
    float dot_product = dot(seed4, vec4(12.9898,78.233,45.164,94.673));
    return fract(sin(dot_product) * 43758.5453);
}

float ShadowCalculation(int cascadeIndex, vec4 lightSpacePos)
{

    float shadow = 0.0;
    vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;//Perspective divide
    projCoords = projCoords * 0.5 + 0.5;//Bring to 0 to 1

    float biasSplitM = biasSplitMultiplier * cascadeIndex * shadowBias;
    float bias = max((shadowBias + biasSplitM) * (1.0 - dot(NORMAL, sunDirection)),(shadowBias + biasSplitM) * 0.1);//Calculate bias based on surface normal to sun angle
    if (cascadeIndex == cascadeCount)//Modify bias based on cascade level
    {
        bias *= 1/(farPlane * 0.5f);
    }
    else
    {
        bias *= 1/(cascadePlaneDistances[cascadeIndex] * 0.25f);
    }
    for (int i = 0; i < shadowSamples; i++)//Poisson PCF sampling
    {
        int index = int(16.0 * rndNum(vec4(gl_FragCoord.xyy,i)))%16;//Random index
        float closestDepth = texture(directionalShadowMap[cascadeIndex], projCoords.xy + (poisson16[index] * float(4 - cascadeIndex))/3000.0).r;//Vary softness based on cascade level
        float currentDepth = projCoords.z;
        shadow += currentDepth - bias < closestDepth ? 1.0 : 0.0;
    }

    return shadow / shadowSamples;
}

void main()
{
    float shadow = 0.0;
    for (int i = 0; i < cascadeCount; i++)
    {
        if (ClipSpacePosZ < cascadePlaneDistances[i])
        {
            shadow = ShadowCalculation(i, FRAG_POS_LIGHT_SPACE[i]);//Calculate shadow at given cascade level
            break;
        }
    }
    //shadow = 1.0 - shadow;//Invert shadow
    
    vec3 normal = texture(normalTexture, UV).xyz;//Load normal map
    normal = normalize(normal * 2.0 - 1.0);//Normalize normal map coefficients
    normal *= normalStrength;

    float diffuse = max(mix(dot(normal, TANGENT_SUN_DIR.xyz), abs(dot(normal, TANGENT_SUN_DIR.xyz)), transmission), 0.0);//Calculate diffuse lighting

    //Specular
    vec4 viewDir = normalize(TANGENT_CAM_POS - TANGENT_FRAG_POS);
    vec4 halfway = normalize(TANGENT_SUN_DIR + viewDir);
    float roughness = 1.0 - texture(roughnessTexture, UV).g;
    float specular = roughness * specularStrength * pow(max(dot(normal, halfway.xyz), 0.0), specularPower * max(roughness, 0.1) * 100.0);
    //Specular reflections
    //vec3 I = normalize(FRAG_POS, CAMERA);

    //Ambient
    vec4 ambient = textureLod(environmentCubemap, NORMAL.xyz, 6.0) * environmentStrength;//Calculate simple ambient color using ambient cubemap

    fragColor = pow(texture(albedoTexture, UV),vec4(2.2)) * color;//Texture gamma correction
    fragColor = (fragColor * (shadow * diffuse + ambient)) + (shadow * specular);//Apply lighting
} 

