#version 330 core

in vec2 UV;
in vec4 NORMAL;
in mat4 VIEW_MATRIX;
in vec4 FRAG_POS;
in vec4 FRAG_POS_LIGHT_SPACE;
in mat3 TBN_MATRIX;
in vec4 TANGENT_SUN_DIR;
//in vec4 TANGENT_EYE_DIR;

out vec4 color;
uniform sampler2D shadowMap;
uniform vec4 sunDirection = vec4(0.0,-1.0,0.0,0.0);
uniform float normalStrength = 1.0;


uniform sampler2D albedoTexture;
uniform sampler2D normalTexture;
uniform sampler2D roughnessTexture;

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

float ShadowCalculation(vec4 fragPosLightSpace, vec3 lightDir, vec3 normal)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    if(projCoords.z > 1.0)
        return 0.0;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float bias = max(0.005 * (1.0 - dot(normal, lightDir)), 0.0005);

    float shadow = 0.0;

    for (int i = 0; i<4; i++)
    {
        int index = int(16.0 * rndNum(vec4(gl_FragCoord.xyy,float(i))))%16;
        float closestDepth = texture(shadowMap, projCoords.xy + poisson16[index]/1400.0).r; 
        if(currentDepth - bias > closestDepth)
        {
            shadow += 0.25;
        }
    }

    return shadow;
} 

void main()
{
    color = texture(albedoTexture, UV);
    if (color.w < 0.1)
        discard;

    float shadow = ShadowCalculation(FRAG_POS_LIGHT_SPACE, vec3(0.0,-1.0,0.0), NORMAL.xyz);
    shadow = 1.0 - shadow;
    vec4 ambient = vec4(0.2,0.3,0.4,1.0);
    vec3 sunDir = vec3(0.0,-1.0,0.0);
    
    vec3 normal = texture(normalTexture, UV).xyz;
    normal = mix(vec3(0.5,0.5,1.0), normal, normalStrength);
    normal = normalize(normal * 2.0 - 1.0);

    float diffuse = max(-dot(normal, TANGENT_SUN_DIR.xyz) * shadow, 0.0);

    //color = vec4(0.7);
    color *= (diffuse + ambient);
} 

