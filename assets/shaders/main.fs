#version 440 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec4 FragPosLightSpace;

uniform vec3 viewPos;
uniform bool useColor;
uniform vec3 color;

struct DirLight 
{
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight 
{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

struct Material 
{
    sampler2D albedoTXT;
    sampler2D normalTXT;

    vec3 albedo;
    float metallic;
    float roughness;
    float ao;
}; 

uniform samplerCube irradianceMap;

uniform Material material;

#define NR_MAX_LIGHTS 8
const float PI = 3.14159265359;

uniform int currentPointLights;
uniform int currentSpotLights;

uniform DirLight dirLight;
uniform PointLight pointLights[NR_MAX_LIGHTS];
uniform SpotLight spotLights[NR_MAX_LIGHTS];

uniform sampler2D shadowMap;

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading 
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(normal, reflectDir), 0.0), 64.0);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), 0.5);
    // attenuation
    float lightDistance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * lightDistance + light.quadratic * (lightDistance * lightDistance));    
    // combine results
    vec3 ambient = light.ambient;
    vec3 diffuse = light.diffuse * diff;
    vec3 specular = light.specular * spec;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    //return (ambient + diffuse);
     return (ambient + diffuse + specular);
}

vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
   vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64.0);

    float lDistance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * lDistance + light.quadratic * (lDistance * lDistance));    

    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    
  	vec3 ambient = light.ambient;
    vec3 diffuse  = light.diffuse * diff;
    vec3 specular = light.specular * spec;  

	ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return (ambient + diffuse + specular);
}

float ShadowCalculation(vec4 fragPosLightSpace, DirLight light)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(light.direction - FragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

vec3 calculateDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 ambient = 0.3 * light.ambient;
    // diffuse
    vec3 lightDir = normalize(dirLight.direction - FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * light.diffuse;
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * light.specular;    
    // calculate shadow
    float shadow = ShadowCalculation(FragPosLightSpace, light);              

    vec3 lighting = (ambient + (1.0 - shadow)* (diffuse + specular)) * color;   

    return lighting; 
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

void main()
{

    //int numLights = 1 + currentPointLights + currentSpotLights;

    /*vec3 lightPositions[1 + NR_MAX_LIGHTS + NR_MAX_LIGHTS];
    lightPositions[0] = dirLight.direction;
    
    vec3 lightColors[1 + NR_MAX_LIGHTS + NR_MAX_LIGHTS];
    lightColors[0] = dirLight.ambient;*/

    vec3 lightPositions[4] = {
            vec3(-10.0f,  10.0f, 10.0f),
            vec3( 10.0f,  10.0f, 10.0f),
            vec3(-10.0f, -10.0f, 10.0f),
            vec3( 10.0f, -10.0f, 10.0f),
    };

    vec3 lightColors[] = {
        vec3(300.0f, 300.0f, 300.0f),
        vec3(300.0f, 300.0f, 300.0f),
        vec3(300.0f, 300.0f, 300.0f),
        vec3(300.0f, 300.0f, 300.0f)
    };

    int numLights = 4;

    vec3 N = normalize(Normal);
    vec3 V = normalize(viewPos - FragPos);
    vec3 R = reflect(-V, N);

    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, material.albedo, material.metallic);

    vec3 Lo = vec3(0.0);

    for (int i = 0; i < numLights; i++)
    {
        vec3 L = normalize(lightPositions[i] - FragPos);
        vec3 H = normalize(V + L);

        float dist = length(lightPositions[i] - FragPos);
        float attenuation = 1.0 / (dist * dist);
        vec3 radiance = lightColors[i] * attenuation;

        float NDF = DistributionGGX(N, H, material.roughness);   
        float G   = GeometrySmith(N, V, L, material.roughness);      
        vec3 F    = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);
           
        vec3 numerator    = NDF * G * F; 
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
        vec3 specular = numerator / denominator;

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;

        kD *= 1.0 - material.metallic;	  

        float NdotL = max(dot(N, L), 0.0);        
        Lo += (kD * material.albedo / PI + specular) * radiance * NdotL; 
    }

    vec3 kS = fresnelSchlick(max(dot(N, V), 0.0), F0);
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - material.metallic;	
    vec3 irradiance = texture(irradianceMap, N).rgb;
    vec3 diffuse = irradiance * material.albedo;
    vec3 ambient = (kD * diffuse) * material.ao;

    
    vec3 color = ambient + Lo;
    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0 / 2.2)); 

    /*vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result = calculateDirLight(dirLight, norm, viewDir);

    for(int i = 0; i < currentPointLights; i++)
    {
        result += calculatePointLight(pointLights[i], norm, FragPos, viewDir);
    }

    for(int i = 0; i < currentSpotLights; i++)
    {
        result += calculateSpotLight(spotLights[i], norm, FragPos, viewDir);
    }*/

	FragColor = vec4(color, 1.0);
    //FragColor = vec4(texture(material.albedoTXT, TexCoords));
}