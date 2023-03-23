#version 330 core

in vec4 NORMAL;
in vec2 UV;
in vec2 UV1;
in vec2 UV2;
in vec2 FUV1;
in vec2 FUV2;
in vec4 FRAG_POS;
in mat4 TBN_MATRIX;
in float waveHeight;

//Directional shadows
uniform sampler2DShadow directionalShadowMap[4];
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
in vec4 FRAG_POS_LIGHT_SPACE[4];
in float ClipSpacePosZ;
//Normal mapping
uniform float normalStrength = 1.0;
//Textures
uniform sampler2D albedoTexture;
uniform sampler2D normalTexture;
uniform sampler2D ORMTexture;
uniform vec4 color = vec4(1.0);
uniform float roughnessMultiplier = 0.2;
uniform float metallicMultiplier = 0.0;
//Shading
uniform vec4 u_viewPosition;
uniform float specularPower;
uniform float specularStrength = 4.0;
uniform float transmission = 0.0;
uniform samplerCube environmentCubemap;
uniform float environmentStrength;

in vec4 TANGENT_CAM_POS;
in vec4 TANGENT_FRAG_POS;

in float YPos;

out vec4 fragColor;

#define PI 3.14159265359

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
    float bias = max((shadowBias + biasSplitM) * (1.0 - dot(NORMAL, u_sunDirection)),(shadowBias + biasSplitM) * 0.1);//Calculate bias based on surface normal to sun angle
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
        float currentDepth = projCoords.z;
        shadow += texture(directionalShadowMap[cascadeIndex], vec3(projCoords.xy + (poisson16[index] * float(4 - cascadeIndex))/3000.0, currentDepth - bias));//Vary softness based on cascade level
    }

    return shadow / shadowSamples;
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
} 

vec4 reflectance(vec3 lightDirection, vec3 lightColor, vec3 normal, vec3 albedo, vec3 viewDirection, float roughness, float metallic, float dist)
{
    float attenuation = 1.0 / (dist * dist);
    if (attenuation < 0.00001)
        return vec4(0.0,0.0,0.0,1.0);

    vec3 Lo = vec3(0.0);
    vec3 N = normal;
    vec3 L = lightDirection;
    vec3 V = viewDirection;
    vec3 H = normalize(V + L);
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    vec3 radiance = lightColor * attenuation;
    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);
    float NDF = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;

    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular = numerator / denominator;

    float NdotL = max(dot(N, L),0.0);
    Lo = (kD * albedo / PI + specular) * radiance * NdotL;
    return vec4(Lo,1.0);
}

vec4 ambientIrradiance(samplerCube cubemap, vec3 normal, vec4 albedo, float roughness, float metallic,  vec3 viewDirection)
{
    vec4 diffuseAmbient = pow(textureLod(cubemap, normal, 24.0), vec4(2.2)) * albedo;
    vec3 reflectionDir = normalize(reflect(-viewDirection, normal));
    vec4 specularAmbient = pow(textureLod(cubemap, reflectionDir, roughness * roughness * 32.0),vec4(2.2)) * mix(vec4(1.0), albedo, metallic);

    vec4 dialetricIrr = mix(specularAmbient, diffuseAmbient, roughness * 0.4 + 0.6);
    return mix(dialetricIrr, specularAmbient, metallic);
}

void main()
{
    vec4 albedo = color;//Texture gamma correction
    float foamT1 = texture(albedoTexture, FUV2).r * texture(albedoTexture, -FUV2 * 0.4).b;
    float foamT2 = texture(albedoTexture, FUV1).g * texture(albedoTexture, -FUV1).g;
    float foam =  foamT1 * foamT2 * clamp((waveHeight + 0.5) * 0.5, 0.0, 1.0) * 0.75;
    albedo = mix(albedo, vec4(1.0,1.0,1.0,1.0), foam);
    float shadow = 1.0;
    for (int i = 0; i < cascadeCount; i++)
    {
        if (ClipSpacePosZ < cascadePlaneDistances[i])
        {
            shadow = ShadowCalculation(i, FRAG_POS_LIGHT_SPACE[i]);//Calculate shadow at given cascade level
            break;
        }
    }
    //shadow = 1.0;
    vec3 orm = texture(ORMTexture, UV).rgb;
    float roughness = orm.g * roughnessMultiplier;
    float metallic = orm.b * metallicMultiplier;
    float ao = orm.r;

    //Normal mapping
    vec3 normal = mix(texture(normalTexture, UV1).xyz, texture(normalTexture, -UV1).xyz, 0.5);//Load normal map
    normal = mix(normal, texture(normalTexture, UV2).xyz, 0.25);
    normal = normalize(normal * 2.0 - 1.0);//Normalize normal map coefficients
    normal = normalize(TBN_MATRIX * vec4(normal,0.0)).xyz;
    normal *= normalStrength;

    vec3 vDir = normalize(u_viewPosition - FRAG_POS).xyz;
    vec3 sunColorM = u_sunColor.xyz * u_sunIntensity;
    vec4 PBRDirectional = reflectance(u_sunDirection.xyz, sunColorM, normal, albedo.rgb, vDir, roughness, metallic, 0.4);
    
    //Ambient
    vec4 ambient = ambientIrradiance(environmentCubemap, normal, albedo, roughness, metallic, vDir) * environmentStrength * ao;//Calculate simple ambient color using ambient cubemap

    fragColor = (shadow * PBRDirectional) + ambient;//Apply lighting
    fragColor *= pow(clamp((YPos/256.0) + 1.0 , 0.0, 1.0), 4.0);
    fragColor.a = albedo.a;
    //fragColor = vec4(1.0);
} 

